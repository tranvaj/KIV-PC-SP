#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashtable.h"

hashtable *create_hashtable(uint capacity){
    hashtable *ht;
    node **temp;

    if(!capacity) {
        return NULL;
    }

    temp = (node **) calloc(capacity, sizeof(node *));
    ht = (hashtable *) calloc(1,sizeof(hashtable));

    if(!temp || !ht) {
        //Nevime zda napriklad temp projde ale ht neprojde, v tomto pripade musime uvolnit oboji
        //Prohledaval jsem internet a free(NULL) pry nevadi, muzem takto zkratit kod
        free(temp);
        free(ht);
        return NULL;
    }

    ht->capacity = capacity;
    ht->arr = temp;
    ht->count = 0;
    return ht;
}

int rehash(hashtable *h){
    int i, index;
    uint new_capacity;
    node *curr, **temp, *curr_next;

    //zvetseni kapacity o konstantu-krat
    new_capacity = h->capacity * INCREASE_CONSTANT;
    temp = (node **) calloc(new_capacity, sizeof(node *));

    if(!h || !temp) {
        return 0;
    }

    //prochazime vsema uzlama tabulky
    for(i = 0; i < h->capacity; i++){
        curr = h->arr[i];
        while(curr){
            //nejdrive si ulozime nasledovnika tohoto uzlu
            curr_next = curr->next;
            index = hash_func(curr->key, new_capacity);
            
            if(temp[index]){
                //vlozime ukazatele do noveho ukazetele na ukazetele
                curr->next = temp[index];
                temp[index] = curr;
            } else {
                //pokud na tomto indexu neni prazdno
                //vlozime ukazatele do prvni pozice a jeho nasledovnik bude predchozi ukazatel na prvni pozici
                //poznamka: frekvence slov zustavaji nezmeneny, vkladani na prvni pozici v tomto pripade nevadi
                temp[index] = curr;
                temp[index]->next = NULL;
            }
            //nasledovnik jiz byl ulozen predem, vyuzijeme ho
            curr = curr_next;
        }
    }
    
    //uvolnime stary ukazatel na ukazatele
    free(h->arr);
    h->arr = temp;
    h->capacity = new_capacity;
    
    return 1;
}

int add_item(hashtable *h, const char *key){
    node *temp, *n;
    int index;

    if((double)(h->count+1) / h->capacity > MAX_BUCKET_SIZE){
        //Chceme udrzet konstantni amortizovanou slozitost pro vkladani a hledani prvku 
        rehash(h);
        //printf("rehashed\n");
    }

    if(!h || !key || !*key || !h->arr) {
        return 0;
    }

    index = hash_func(key, h->capacity);

    temp = (node *) malloc(sizeof(node));
    if(!temp) {
        return 0;
    }

    temp->key = (char *) malloc(sizeof(char) * (strlen(key)+1));
    if(!temp->key) {
        return 0;
    }

    strcpy(temp->key,key);

    temp->freq = 1;
    temp->next = NULL;

    if(!h->arr[index]){
        h->arr[index] = temp;
    } else{
        n = h->arr[index];
        do{
            if(!strcmp(n->key,key)){
                //pokud jsou si retezce shodne, zvysime frekvenci slova a ukoncime cyklus vyhledavani
                n->freq++;
                free(temp->key);
                free(temp);
                h->uq_item_cnt--;
                break;
            }
            if(!n->next){
                n->next = temp;
                break;  
            }
            n = n->next;
        } while(n);
    }
    h->count++;
    h->uq_item_cnt++;
    return 1;
}

uint get_freq(hashtable *h, char *key){
    node *n = get_node(h,key);
    if(!n) {
        return 0;
    }
    return n->freq;
}

node *get_node(hashtable *h, char *key){
    int index;
    node *temp;
    index = hash_func(key, h->capacity);
    temp = h->arr[index];
    while(temp){
        //hledame prvek se stejnym nazvem klice
        if(!strcmp(temp->key,key)){
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}


uint hash_func(const char *key, uint size){
    //djb2 hash function http://www.cse.yorku.ca/~oz/hash.html
    unsigned long hash = 5381;
    int c;

    while ((c = *key++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash%size;
    /*
    //starsi polynomial roll hash fce
    const int p = 31;
    int hash = 0;
    long p_pow = 1;
    for (int i = 0 ; key[i] != '\0' ; i++)
    {
        hash = (hash + (key[i] - 'a' + 1) * p_pow) % size;
        p_pow = (p_pow * p) % size;
    }
    
    return hash;
    */
}

void free_hashtable(hashtable **h){
    int i;
    node *curr, *prev;

    if(!h || !*h) return;

    for(i = 0; i < (*h)->capacity; i++){
        curr = (*h)->arr[i];
        while(curr){
            prev = curr;
            curr = curr->next;
            free(prev->key);
            free(prev);
        }
    }
    free((*h)->arr);
    free((*h));
    *h = NULL;
}