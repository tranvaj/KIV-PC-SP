#ifndef __FREADER__
#include "hashtable.h"
#define __FREADER__

//Znak ktery oddeluje slova v dokumentech
#define LINE_DELIMS " "
//Pripona nacitanych dokumentu
#define FILE_SUFFIX ".txt"
//Predpona vsech dokumentu (misto, kde se nachazi trenovaci dokumenty a testovaci dokumenty)
#define DATA_FOLDER "data/"
/**
 * Pridava do dane hash tabulky slova ze souboru
 * Do count se ulozi pocet nactenych slov
 * Metoda vraci 1 pokud se uspesne provedla nebo 0 pokud nekde selhala
*/
int load_words(const char filename[], int *count, hashtable *h);

/**
 * Nacte do hash tabulky vsechny slova ze souboru s nazvem:
 * <vzor><1-N><pripona>
 * kde vzor a N jsou dane parametrama a pripona je dana konstantou FILE_SUFFIX, ktera je definovana v bayes.h
 * Vychozi hodnota FILE_SUFFIX je ".txt" 
*/
int load_words_from_vzor_files(const char vzor[], int N, hashtable *h);

/**
 * Vytvori pointer na retezec, ktery bude vypadat takto:
 * <vzor><vzor_num><FILE_SUFFIX>
 * kde vzor a vzor_num jsou argumenty a FILE_SUFFIX je konstanta definovana v bayes.h
 * 
 * Vraceny retezec se musi manualne UVOLNIT! Retezec zabira alokovanou pamet 
*/
char *create_vzor_name(const char vzor[], int vzor_num);

/**
 * Vraci delku 1. radky textoveho souboru
*/
int get_first_line_len(const char filename[]);

#endif