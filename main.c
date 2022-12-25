#include <stdio.h>   
#include "hashtable.h"
#include "bayes.h"

int main()
{
    //index 0 spam, index 1 ham

    const char *spam_vzor = "data/train/spam";
    const char *ham_vzor = "data/train/ham";
    const char *test = "data/test/ham";

    trainset *t = create_dictionary(spam_vzor, 300, ham_vzor, 300);
    if(t){
        NB_learn_text(t);
        NB_classify_vzor_text(test,100,t,"result.txt");
        free_dictionary(&t);
    }
    
    return 0;
}