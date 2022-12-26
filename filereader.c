#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "filereader.h"

int load_words(const char filename[], int *count, hashtable *h){
    FILE *f;
    char *line, *word, *delim = LINE_DELIMS;
    int wc, line_len;
     
    *count = 0;
    if (!filename || !*filename) {
        return 0;
    }
    
    f = fopen(filename, "r");
    if(!f) {
        printf("Error opening file '%s': %s\n", filename, strerror(errno));
        return 0;
    }

    wc = 0;
    line_len = get_first_line_len(filename);
    if(line_len <= 1){
        printf("Error empty file: '%s'\n", filename);
        fclose(f);
        return 0;
    }

    line = (char *) calloc(line_len, sizeof(char));
    if (fgets(line, line_len, f)){
        if(*line && strcmp(line, "\n")) {
            word = strtok(line, delim);
            while(word){
                //printf("%s\n",word);
                add_item(h,word);
                word = strtok(NULL, delim);
                wc++;
            }
        } else {}
    }

    *count = wc;
    fclose(f);
    free(line);
    return 1;
}

int load_words_from_vzor_files(const char vzor[], int N, hashtable *h){
    int i, count = 0;
    char *vzor_name;
    
    for(i = 1; i <= N; i++){
        //zde prohiha zretezeni pripony a cisla ke vzoru
        vzor_name = create_vzor_name(vzor,i);

        if(!load_words(vzor_name, &count, h)){
            free(vzor_name);
            return 0;
        }
        free(vzor_name);
    }
    return 1;
}

int get_first_line_len(const char filename[]){
    FILE *f;
    int character, count = 0;

    if (!filename || !*filename) {
        return 0;
    }
    
    f = fopen(filename, "r");
    if(!f) {
        printf("Error opening file '%s': %s\n", filename, strerror(errno));
        return 0;
    }

    while(1) {
        character = fgetc(f);
        if(character == EOF || character == '\n'){
            count++;
            break;
        }
        count++;
    }
    fclose(f);
    return count;
}

char *create_vzor_name(const char vzor[], int vzor_num){
    int vzor_num_len = 0;
    char *vzor_tmp, *vzor_num_str, *final_vzor, *suffix = FILE_SUFFIX;
    
    vzor_tmp = (char *) malloc(strlen(vzor) + 1);
    strcpy(vzor_tmp, vzor);

    vzor_num_len = snprintf(NULL, 0, "%d", vzor_num);
    vzor_num_str = (char *) malloc(vzor_num_len+1);
    snprintf(vzor_num_str, vzor_num_len + 1, "%d", vzor_num);

    final_vzor = (char *) malloc(strlen(vzor_tmp) + vzor_num_len + strlen(suffix) + 1);
    strcpy(final_vzor, vzor_tmp);
    strcat(final_vzor, vzor_num_str);
    strcat(final_vzor, suffix);
    
    //uz nevyuzivame tyto retezce, muzeme uvolnit
    free(vzor_tmp);
    free(vzor_num_str);
    return final_vzor;
}