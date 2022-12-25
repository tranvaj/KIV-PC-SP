#include <stdio.h>
#include <stdlib.h>   
#include <string.h>
#include "hashtable.h"
#include "bayes.h"

int main(int argc, char *argv[])
{
    //index 0 spam, index 1 ham 

    if(argc != 8){
		printf("Error: Missing arguments!\nUsage: %s <spam> <spam-cnt> <ham> <ham-cnt> <test> <test-cnt> <out-file>\n", argv[0]);
		exit(EXIT_FAILURE);	
	}

    //3 rozdilne promenne pro testovaci ucely 
    const char *def_file_spam_train = "data/";
    const char *def_file_ham_train = "data/";
    const char *def_file_test = "data/";

    char spam_vzor[strlen(argv[1]) + strlen(def_file_spam_train) + 1];
    snprintf(spam_vzor, sizeof(spam_vzor), "%s%s", def_file_spam_train, argv[1]);
    int spam_cnt = atoi(argv[2]);

    char ham_vzor[strlen(argv[3]) + strlen(def_file_ham_train) + 1];
    snprintf(ham_vzor, sizeof(ham_vzor), "%s%s", def_file_ham_train, argv[3]);
    int ham_cnt = atoi(argv[4]);

    char test[strlen(argv[5]) + strlen(def_file_test) + 1];
    snprintf(test, sizeof(test), "%s%s", def_file_test, argv[5]);
    int test_cnt = atoi(argv[6]);

    char *out_file = argv[7];

    if(spam_cnt <= 0 || ham_cnt <= 0 || test_cnt <= 0){
        printf("Error: Arguments <spam-cnt>, <ham-cnt>, <test-cnt> have to be >0!\nYour input was: %d, %d, %d\n", spam_cnt, ham_cnt, test_cnt);
		exit(EXIT_FAILURE);	
    }

    //printf("%s\n",test);

    trainset *t = create_dictionary(spam_vzor, spam_cnt, ham_vzor, ham_cnt);
    if(t){
        NB_learn_text(t);
        NB_classify_vzor_text(test,test_cnt,t,out_file);
        free_dictionary(&t);
        exit(EXIT_SUCCESS);
    }
    
    exit(EXIT_FAILURE);
}