#ifndef __HASHTABLE__
#define __HASHTABLE__
#define HASHTABLE_CAPACITY 257
#define WORD_LEN 256
typedef unsigned int uint;

typedef struct node node;

struct node{
    char *key;
    uint freq;
    node *next;
};

typedef struct hashTable{
    node **arr;  
    uint capacity;
    uint count; //pocet prvku v tabulce
} hashTable;

hashTable *create_hashtable(uint capacity);
void free_hashtable(hashTable **h);
int add_item(hashTable *h, const char *key);
uint hash_func(const char *key, uint size);
uint get_freq(hashTable *h, char *key);
#endif