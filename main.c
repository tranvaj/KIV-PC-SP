#include <stdio.h>   
#include "hashtable.h"
#include "bayes.h"

int main()
{
    //index 0 spam, index 1 ham

    const char *spam_vzor = "data/custom/spam";
    const char *ham_vzor = "data/custom/ham";
    const char *test = "data/custom/test";

    trainset *t = create_dictionary(spam_vzor, 1, ham_vzor, 1);
    /*printf("spam_cnt: %d, spam_unq_cnt: %d, spam_file_cnt: %d, ham_cnt: %d, ham_unq_cnt: %d, ham_file_cnt: %d\n", 
    t->sets[0]->dict->count, 
    t->sets[0]->dict->uq_item_cnt, 
    t->sets[0]->dict_file_cnt,
    t->sets[1]->dict->count, 
    t->sets[1]->dict->uq_item_cnt, 
    t->sets[1]->dict_file_cnt);*/
    NB_learn_text(t);
    NB_classify_vzor_text(test,1,t,"result.txt");
    //int classification = NB_classify_text(test,t);
    //printf("%s : %d\n", test, classification);
    //printf("freq league: %d\n", get_freq(t->sets[0]->dict,"league"));
    free_dictionary(&t);
    return 0;
}