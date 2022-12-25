#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <float.h>
#include <math.h>
#include "hashtable.h"
#include "bayes.h"
#define LINE_DELIMS " "
#define LINE_LEN 50000
#define TRAINING_SET_CNT 3
#define SPAM_INDEX 0
#define HAM_INDEX 1
#define TOTAL_INDEX 2

int load_words(const char filename[], int *count, hashTable *h){
    FILE *f;
    char line[LINE_LEN] = {0}, *word, *delim = LINE_DELIMS;
    int lc, wc;
     
    *count = 0;
    if (!filename || !*filename) return 0;
    
    f = fopen(filename, "r");
    if(!f) {
        printf("Error opening file '%s': %s\n", filename, strerror(errno));
        return 0;
    }

    lc = 0;
    wc = 0;
    while (!feof(f)){
        if(!fgets(line, LINE_LEN, f) || !*line || !strcmp(line, "\n")) continue;
        word = strtok(line, delim);
        while(word){
            //printf("%s\n",word);s
            add_item(h,word);
            word = strtok(NULL, delim);
            wc++;
        }
        lc++;
    }

    *count = wc;
    fclose(f);
    return 1;
}

int create_vzor_dictionary(const char vzor[], int N, hashTable *h){
    int i, count, i_len = 0;
    char *a, *b, *c, *suffix = ".txt";
    int stop = 0;
    
    for(i = 1; i <= N; i++){
        a = (char *) malloc(strlen(vzor) + 1);
        strcpy(a, vzor);

        i_len = snprintf(NULL, 0, "%d", i);
        b = (char *) malloc(i_len+1);
        snprintf(b, i_len + 1, "%d", i);

        c = (char *) malloc(strlen(a) + i_len + strlen(suffix) + 1);
        strcpy(c, a);
        strcat(c, b);
        strcat(c, suffix);
        //printf("%s\n",c);

        if(!load_words(c, &count, h)){
            stop = 1;
        }

        free(a);
        free(b);
        free(c);

        if(stop){
            return 0;
        }
    }
    return 1;
}

trainset *create_dictionary(const char spam_vzor[], int spam_file_count, const char ham_vzor[], int ham_file_count){
    hashTable *ham, *spam, *total;
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

        free(ham); //ham a spam jsou NULL
        free(spam);
        free(total);
        free(t->sets);
        return NULL;
    };
    t->set_cnt = TRAINING_SET_CNT - 1; //chceme cyklovat mezi spam mnozinou a ham
    t->sets[SPAM_INDEX] = spam_set;
    t->sets[HAM_INDEX] = ham_set;
    t->sets[TOTAL_INDEX] = total_set;

    ham_set->dict = ham;
    ham_set->dict_file_cnt = ham_file_count;

    spam_set->dict = spam;
    spam_set->dict_file_cnt = spam_file_count;

    total_set->dict = total;
    total_set->dict_file_cnt = ham_file_count + spam_file_count;

    if(!create_vzor_dictionary(spam_vzor, spam_file_count, spam_set->dict) 
        || !create_vzor_dictionary(ham_vzor, ham_file_count, ham_set->dict)
        || !create_vzor_dictionary(spam_vzor,spam_file_count, total_set->dict)
        || !create_vzor_dictionary(ham_vzor, ham_file_count, total_set->dict)
        ){
        free_dictionary(&t);
    }
    return t;
}

void free_dictionary(trainset **t){
    uint i;

    for(i = 0; i < (*t)->set_cnt + 1; i++){ //chceme uvolnit i TOTAL hash tabulku
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
    hashTable *dict;
    for(i = 0; i < t->set_cnt; i++){
        trainset_cnt += t->sets[i]->dict_file_cnt;
    }

    uq_total = t->sets[TOTAL_INDEX]->dict->uq_item_cnt;

    for(i = 0; i < t->set_cnt; i++){
        set = t->sets[i];
        set->probability = (double)set->dict_file_cnt / (double)trainset_cnt;

        n = set->dict->count;
        dict = set->dict;

        for(j = 0; j < t->sets[TOTAL_INDEX]->dict->capacity; j++){
            curr = t->sets[TOTAL_INDEX]->dict->arr[j];
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
    hashTable *h;
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
        return -1;
    }

    for(i = 0; i < t->set_cnt; i++){
        sum = 0; //resetovat sumu
        for(j = 0; j < h->capacity; j++){
            curr_doc_word = h->arr[j];
            while(curr_doc_word){
                curr_set_word = get_node(t->sets[TOTAL_INDEX]->dict, curr_doc_word->key);
                if(curr_set_word){
                    if(i == SPAM_INDEX){
                        sum += log(curr_set_word->p_spam);
                        //printf("probspam: %s %f\n",curr_set_word->key,curr_set_word->p_spam);
                    } else {
                        sum += log(curr_set_word->p_ham);
                        //printf("probham: %s %f\n",curr_set_word->key,curr_set_word->p_ham);
                    }
                }
                curr_doc_word = curr_doc_word->next;
            }
        }

        //printf("i: %d | cnb: %f ,prob_set: %f, sum_set: %f\n", i,t->sets[i]->probability * sum, t->sets[i]->probability, sum);
        //printf("cnt: %d\n",t->sets[i]->dict->count);
        if(t->sets[i]->probability * sum > c_nb){
            c_nb = t->sets[i]->probability * sum;
            index_type = i;
        }
    }

    free_hashtable(&h);
    return index_type;
}

void NB_classify_vzor_text(const char vzor[], int vzor_count, trainset *t, const char output[]){
    int i, i_len = 0;
    char *a, *b, *c, *suffix = ".txt";
    int res = -1;

    FILE *f;
    f = fopen(output,"w");
    if(!f) {
        printf("Error modyifing file '%s': %s\n", output, strerror(errno));
        return;
    }
    
    for(i = 1; i <= vzor_count; i++){
        a = (char *) malloc(strlen(vzor) + 1);
        strcpy(a, vzor);

        i_len = snprintf(NULL, 0, "%d", i);
        b = (char *) malloc(i_len+1);
        snprintf(b, i_len + 1, "%d", i);

        c = (char *) malloc(strlen(a) + i_len + strlen(suffix) + 1);
        strcpy(c, a);
        strcat(c, b);
        strcat(c, suffix);
        //printf("%s\n",c);
 
        res = NB_classify_text(c, t);
        if(res == HAM_INDEX){
            fprintf(f,"%s\tH\n",c);
        } else if(res == SPAM_INDEX){
            fprintf(f,"%s\tS\n",c);
        } else {
            fprintf(f,"%s\tUNKNOWN\n",c);
        }
        free(a);
        free(b);
        free(c);
    }
    fclose(f);
}

