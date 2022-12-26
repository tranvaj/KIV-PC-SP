#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <float.h>
#include <math.h>
#include "hashtable.h"
#include "bayes.h"

int get_first_line_len(const char filename[]){
    FILE *f;
    int character, count = 0;

    if (!filename || !*filename) {
        return 0;
    }
    
    f = fopen(filename, "r");
    if(!f) {
        printf("Error opening file '%s': %s\n", filename, strerror(errno));
        return 0;
    }

    while(1) {
        character = fgetc(f);
        if(character == EOF || character == '\n'){
            count++;
            break;
        }
        count++;
    }
    fclose(f);
    return count;
}

int load_words(const char filename[], int *count, hashtable *h){
    FILE *f;
    char *line, *word, *delim = LINE_DELIMS;
    int wc, line_len;
     
    *count = 0;
    if (!filename || !*filename) {
        return 0;
    }
    
    f = fopen(filename, "r");
    if(!f) {
        printf("Error opening file '%s': %s\n", filename, strerror(errno));
        return 0;
    }

    wc = 0;
    line_len = get_first_line_len(filename);
    line = (char *) calloc(line_len, sizeof(char));
    while (!feof(f)){
        if(!fgets(line, line_len, f) || !*line || !strcmp(line, "\n")) {
            continue;
        }

        word = strtok(line, delim);
        while(word){
            //printf("%s\n",word);s
            add_item(h,word);
            word = strtok(NULL, delim);
            wc++;
        }
    }

    *count = wc;
    fclose(f);
    free(line);
    return 1;
}

int load_words_from_vzor_files(const char vzor[], int N, hashtable *h){
    int i, count = 0;
    char *vzor_name;
    
    for(i = 1; i <= N; i++){
        //zde prohiha zretezeni pripony a cisla ke vzoru
        vzor_name = create_vzor_name(vzor,i);

        if(!load_words(vzor_name, &count, h)){
            free(vzor_name);
            return 0;
        }
        free(vzor_name);
    }
    return 1;
}

trainset *create_trainingset(const char spam_vzor[], int spam_file_count, const char ham_vzor[], int ham_file_count){
    hashtable *ham, *spam, *total;
    set *spam_set, *ham_set,  *total_set;
    trainset *t;

    t = (trainset *) calloc(1,sizeof(trainset));
    t->sets = (set **) malloc(TRAINING_SET_CNT*sizeof(set*));

    spam_set = (set *) malloc(sizeof(set));
    ham_set = (set *) malloc(sizeof(set));
    total_set = (set *) malloc(sizeof(set));

    if(!t 
        || !t->sets 
        || !spam_set 
        || !ham_set 
        || !(ham = create_hashtable(HASHTABLE_CAPACITY_INIT)) 
        || !(spam = create_hashtable(HASHTABLE_CAPACITY_INIT)) 
        || !(total = create_hashtable(HASHTABLE_CAPACITY_INIT))){
        free(t);
        free(spam_set);
        free(ham_set);
        free(total_set);

        free(ham); 
        free(spam);
        free(total);
        free(t->sets);
        //opet nevime kde alokovani proslo a kde ne, uvolnime vsechno pro jistotu, protoze free(NULL) by nemelo vadit podle stackoverflow
        //muzem to individualne kontrolovat, ale takto zkratime kod
        return NULL;
    };

    t->set_cnt = TRAINING_SET_CNT - 1; //odecitame 1 protoze chceme vyuzivat jenom mnozinu HAM a SPAM, na indexu 2 je jejich sjednoceni, to vyuzijeme pro jine ucely
    t->sets[SPAM_INDEX] = spam_set;
    t->sets[HAM_INDEX] = ham_set;
    t->sets[TOTAL_INDEX] = total_set;

    ham_set->dict = ham;
    ham_set->dict_file_cnt = ham_file_count;

    spam_set->dict = spam;
    spam_set->dict_file_cnt = spam_file_count;

    total_set->dict = total;
    total_set->dict_file_cnt = ham_file_count + spam_file_count;

    if(!load_words_from_vzor_files(spam_vzor, spam_file_count, spam_set->dict) 
        || !load_words_from_vzor_files(ham_vzor, ham_file_count, ham_set->dict)
        || !load_words_from_vzor_files(spam_vzor,spam_file_count, total_set->dict)
        || !load_words_from_vzor_files(ham_vzor, ham_file_count, total_set->dict)
        ){
        free_trainingset(&t);
    }
    return t;
}

void free_trainingset(trainset **t){
    uint i;

    for(i = 0; i < (*t)->set_cnt + 1; i++){ //pricteme 1 k set_cnt protoze chceme uvolnit i mnozinu sjednoceni HAM a SPAM
        free_hashtable(&((*t)->sets[i]->dict));
        free((*t)->sets[i]);
    }
    free((*t)->sets);
    free(*t);
    *t = NULL;
}

void NB_learn_text(trainset *t){
    uint i, j, trainset_cnt = 0, n, n_k, uq_total;
    set *set;
    node *curr;
    hashtable *dict;
    for(i = 0; i < t->set_cnt; i++){
        trainset_cnt += t->sets[i]->dict_file_cnt;
    }

    uq_total = t->sets[TOTAL_INDEX]->dict->uq_item_cnt;

    //podle pseudokodu NBK, faze uceni
    for(i = 0; i < t->set_cnt; i++){
        set = t->sets[i];
        set->probability = (double)set->dict_file_cnt / (double)trainset_cnt;

        n = set->dict->count;
        dict = set->dict; //slovnik mnoziny bud spam nebo ham

        for(j = 0; j < t->sets[TOTAL_INDEX]->dict->capacity; j++){
            curr = t->sets[TOTAL_INDEX]->dict->arr[j]; //slovnik mnoziny sjednoceni ham a spam
            while(curr){
                n_k = get_freq(dict, curr->key);
                if(i == SPAM_INDEX){
                    curr->p_spam = (double)(n_k + 1)/(double)(n + uq_total);
                    //printf("%d %d\n", n_k + 1, n + uq_total);
                } else {
                    curr->p_ham = (double)(n_k + 1)/(double)(n + uq_total);
                    //printf("%d %d\n", n_k + 1, n + uq_total);
                }
                curr = curr->next;
            }
        }
    }
    
}

int NB_classify_text(const char doc_filename[], trainset *t){
    hashtable *h;
    double c_nb = -DBL_MAX, sum = 0;
    int i, j, index_type = -1;
    int count;
    node *curr_doc_word, *curr_set_word;
    //hashTable *curr_set;

    h = create_hashtable(HASHTABLE_CAPACITY_INIT);
    if(!h){
        return -1;
    }

    if(!load_words(doc_filename, &count, h)){
        free_hashtable(&h);
        return -1; //klasifikujeme jako -1 = UNKNOWN
    }

    //ARGMAX podle zadani SP
    for(i = 0; i < t->set_cnt; i++){
        sum = 0; //resetovat sumu
        for(j = 0; j < h->capacity; j++){
            curr_doc_word = h->arr[j]; //slovo z dokumentu, ktery se momentalne klasifikuje
            while(curr_doc_word){
                curr_set_word = get_node(t->sets[TOTAL_INDEX]->dict, curr_doc_word->key); //odpovidajici slovo z mnoziny bud SPAM nebo HAM
                if(curr_set_word){
                    if(i == SPAM_INDEX){
                        sum += log(curr_set_word->p_spam);
                    } else {
                        sum += log(curr_set_word->p_ham);
                    }
                }
                curr_doc_word = curr_doc_word->next; //dalsi slovo z dokumentu, ktery se momentalne klasifikuje
            }
        }

        if(t->sets[i]->probability * sum > c_nb){
            c_nb = t->sets[i]->probability * sum;
            index_type = i;
        }
    }

    free_hashtable(&h);
    return index_type;
}

char *create_vzor_name(const char vzor[], int vzor_num){
    int vzor_num_len = 0;
    char *vzor_tmp, *vzor_num_str, *final_vzor, *suffix = FILE_SUFFIX;
    
    vzor_tmp = (char *) malloc(strlen(vzor) + 1);
    strcpy(vzor_tmp, vzor);

    vzor_num_len = snprintf(NULL, 0, "%d", vzor_num);
    vzor_num_str = (char *) malloc(vzor_num_len+1);
    snprintf(vzor_num_str, vzor_num_len + 1, "%d", vzor_num);

    final_vzor = (char *) malloc(strlen(vzor_tmp) + vzor_num_len + strlen(suffix) + 1);
    strcpy(final_vzor, vzor_tmp);
    strcat(final_vzor, vzor_num_str);
    strcat(final_vzor, suffix);
    
    //uz nevyuzivame tyto retezce, muzeme uvolnit
    free(vzor_tmp);
    free(vzor_num_str);
    return final_vzor;
}

void NB_classify_vzor_text(const char vzor[], int vzor_count, trainset *t, const char output[]){
    int i = 0, res = -1;
    char *vzor_name, *vzor_without_prefix;

    FILE *f;
    f = fopen(output,"w");
    if(!f) {
        printf("Error modyifing file '%s': %s\n", output, strerror(errno));
        return;
    }
    
    for(i = 1; i <= vzor_count; i++){
        vzor_name = create_vzor_name(vzor,i);
        vzor_without_prefix = vzor_name + strlen(DATA_FOLDER);
 
        //klasifikace jednoho vzoru
        res = NB_classify_text(vzor_name, t);
        if(res == HAM_INDEX){
            fprintf(f,"%s\tH\n",vzor_without_prefix);
        } else if(res == SPAM_INDEX){
            fprintf(f,"%s\tS\n",vzor_without_prefix);
        } else {
            fprintf(f,"%s\tUNKNOWN\n",vzor_without_prefix);
        }
        free(vzor_name);
    }
    fclose(f);
}

