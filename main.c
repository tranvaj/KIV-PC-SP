#include <stdio.h>       //header section
#include "hashtable.h"
#include "bayes.h"

int main()             //main section
{
    int count = 0;
    load_file("data/train/ham1.txt", &count);

    hashTable * h = create_hashtable(HASHTABLE_CAPACITY);

    add_item(h,"koza");
    add_item(h,"koza");
    add_item(h,"koza");
    add_item(h,"koza");
    add_item(h,"koza");
    add_item(h,"koza");
    add_item(h,"koza");
    add_item(h,"koza");
    add_item(h,"koza");
    add_item(h,"kozacka");
    add_item(h,"kozacka");
    add_item(h,"bayoern");
    int freq = get_freq(h,"bayoerns"); //t
    printf("Frekvence: %d\n", freq);
    free_hashtable(&h);
    return 0;
}