#ifndef __NBAYES__
#include "hashtable.h"
#define __NBAYES__

typedef struct set{
    hashTable *dict;
    uint dict_file_cnt;
    double probability;
} set;

typedef struct trainset{
    set **sets;
    uint set_cnt;
} trainset;

int load_words(const char filename[], int *count, hashTable *h);
int create_vzor_dictionary(const char vzor[], int N, hashTable *h);
void NB_learn_text(trainset *t);
void free_dictionary(trainset **t);
trainset *create_dictionary(const char spam_vzor[], int spam_file_count, const char ham_vzor[], int ham_file_count);
int NB_classify_text(const char doc_filename[], trainset *t);
void NB_classify_vzor_text(const char vzor[], int vzor_count, trainset *t, const char output[]);



#endif