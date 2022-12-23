#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "bayes.h"
#include "hashtable.h"
#define LINE_DELIMS ' '
#define LINE_LEN 50000

int load_file(const char filename[], int *count){
    FILE *f;
    char line[LINE_LEN] = {0}, *word, delim = LINE_DELIMS;
    int lc, wc;
     
    *count = 0;
    if (!filename || !*filename) return -1;
    
    f = fopen(filename, "r");
    if(!f) {
        printf("Error opening file '%s': %s\n", filename, strerror(errno));
        return -1;
    }

    lc = 0;
    wc = 0;
    while (!feof(f)){
        if(!fgets(line, LINE_LEN, f) || !*line || !strcmp(line, "\n")) continue;

        word = strtok(line, &delim);
        while(word){
            printf("%s\n",word);
            word = strtok(NULL, &delim);
            wc++;
        }
        lc++;
    }

    *count = wc;
    printf("%d\n",*count);
    fclose(f);
    return 1;
}