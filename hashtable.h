#ifndef __HASHTABLE__
#define __HASHTABLE__
typedef unsigned int uint;

typedef struct node node;

struct node{
    char *key;
    uint freq;
    double p_spam;
    double p_ham;
    node *next;
};

typedef struct hashTable{
    node **arr;  
    uint capacity;
    uint count; //pocet prvku v tabulce
    uint uq_item_cnt; //unikatni pocet prvku v tabulce
} hashTable;

hashTable *create_hashtable();
void free_hashtable(hashTable **h);
int add_item(hashTable *h, const char *key);
uint hash_func(const char *key, uint size);
uint get_freq(hashTable *h, char *key);
node *get_node(hashTable *h, char *key);

#endif