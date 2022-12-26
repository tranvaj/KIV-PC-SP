#include <stdio.h>
#include <stdlib.h>   
#include <string.h>
#include "hashtable.h"
#include "bayes.h"
#include "filereader.h"

int main(int argc, char *argv[])
{

    if(argc != 8){
		printf("Error: Missing arguments!\nUsage: %s <spam> <spam-cnt> <ham> <ham-cnt> <test> <test-cnt> <out-file>\n", argv[0]);
		exit(EXIT_FAILURE);	
	}

    trainset *t;
    char *out_file;

    //3 rozdilne promenne pro testovaci ucely 
    const char *def_fldr_spam_train = DATA_FOLDER;
    const char *def_fldr_ham_train = DATA_FOLDER;
    const char *def_fldr_test = DATA_FOLDER;

    //musime provest zretezeni, jelikoz soubory jsou v "data/" slozce 
    int spam_vzor_size = strlen(argv[1]) + strlen(def_fldr_spam_train) + 1;
    int ham_vzor_size = strlen(argv[3]) + strlen(def_fldr_ham_train) + 1;
    int test_size = strlen(argv[5]) + strlen(def_fldr_test) + 1;

    char *spam_vzor = (char *) calloc(spam_vzor_size, sizeof(char));
    char *ham_vzor = (char *) calloc(ham_vzor_size, sizeof(char));
    char *test = (char *) calloc(test_size, sizeof(char));

    int spam_cnt = atoi(argv[2]);
    int ham_cnt = atoi(argv[4]);
    int test_cnt = atoi(argv[6]);

    snprintf(spam_vzor, spam_vzor_size, "%s%s", def_fldr_spam_train, argv[1]);
    snprintf(ham_vzor, ham_vzor_size, "%s%s", def_fldr_ham_train, argv[3]);
    snprintf(test, test_size, "%s%s", def_fldr_test, argv[5]);

    out_file = argv[7];

    if(spam_cnt <= 0 || ham_cnt <= 0 || test_cnt <= 0){
        printf("Error: Arguments <spam-cnt>, <ham-cnt>, <test-cnt> have to be >0!\nYour input was: %d, %d, %d\n", spam_cnt, ham_cnt, test_cnt);
        free(spam_vzor);
        free(ham_vzor);
        free(test);
		exit(EXIT_FAILURE);	
    }

    t = create_trainingset(spam_vzor, spam_cnt, ham_vzor, ham_cnt);
    if(t){
        NB_learn_text(t);
        NB_classify_vzor_text(test,test_cnt,t,out_file);
        free_trainingset(&t);
        free(spam_vzor);
        free(ham_vzor);
        free(test);
        exit(EXIT_SUCCESS);
    }
    free(spam_vzor);
    free(ham_vzor);
    free(test);
    exit(EXIT_FAILURE);
}