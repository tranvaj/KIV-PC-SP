#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashtable.h"
#define WORD_LEN 256
#define INCREASE_CONSTANT 2
#define MAX_BUCKET_SIZE 50

hashTable *create_hashtable(uint capacity){
    hashTable *ht;
    node **temp;
    if(!capacity) return NULL;
    temp = (node **) calloc(capacity, sizeof(node *));
    ht = (hashTable *) malloc(sizeof(hashTable));
    if(!temp || !ht) return NULL;
    ht->capacity = capacity;
    ht->arr = temp;
    ht->count = 0;
    return ht;
}

int rehash(hashTable *h){
    int i, index;
    uint new_capacity;
    node *curr, **temp, *curr_next;

    new_capacity = h->capacity * INCREASE_CONSTANT;
    temp = (node **) calloc(new_capacity, sizeof(node *));

    if(!h || !temp) return 0;

    for(i = 0; i < h->capacity; i++){
        curr = h->arr[i];
        while(curr){
            curr_next = curr->next;
            index = hash_func(curr->key, new_capacity);
            
            if(temp[index]){
                curr->next = temp[index];
                temp[index] = curr;
            } else {
                temp[index] = curr;
                temp[index]->next = NULL;
            }
            curr = curr_next;
        }
    }
    free(h->arr);
    h->arr = temp;
    h->capacity = new_capacity;
    
    return 1;
}

int add_item(hashTable *h, const char *key){
    node *temp, *n;
    int index;

    if((double)(h->count+1) / h->capacity > MAX_BUCKET_SIZE){
        //printf("%f\n", (double)(h->count+1) / h->capacity);ss
        rehash(h);
        //printf("rehashed\n");
    }

    if(!h || !key || !*key || !h->arr) {
        return -1;
    }

    index = hash_func(key, h->capacity);

    temp = (node *) malloc(sizeof(node));
    if(!temp) return -1;

    temp->key = (char *) malloc(sizeof(char) * WORD_LEN);
    if(!temp->key) return -1;

    strcpy(temp->key,key);

    temp->freq = 1;
    temp->next = NULL;

    if(!h->arr[index]){
        h->arr[index] = temp;
    } else{
        n = h->arr[index];
        do{
            if(!strcmp(n->key,key)){
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

uint get_freq(hashTable *h, char *key){
    node *n = get_node(h,key);
    if(!n) {
        return 0;
    }
    return n->freq;
}

node *get_node(hashTable *h, char *key){
    int index;
    node *temp;
    index = hash_func(key, h->capacity);
    temp = h->arr[index];
    while(temp){
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

void free_hashtable(hashTable **h){
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