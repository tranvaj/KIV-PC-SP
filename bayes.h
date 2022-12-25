#ifndef __NBAYES__
#include "hashtable.h"
#define __NBAYES__

//Znak ktery oddeluje slova v dokumentech
#define LINE_DELIMS " "
//Maximalni delka radky v dokumentu, nelze vyuzit metodu fgets bez teto delky
#define LINE_LEN 50000
//Pocet mnozin v trenovaci mnozine vcetne mnoziny sjednoceni HAM a SPAM
#define TRAINING_SET_CNT 3
//Index, ktery je souvisly s mnozinou SPAM
#define SPAM_INDEX 0
//Index, ktery je souvisly s mnozinou HAM
#define HAM_INDEX 1
//Index, ktery je souvisly s mnozinou sjednoceni SPAM a HAM
#define TOTAL_INDEX 2
//Pripona nacitanych dokumentu
#define FILE_SUFFIX ".txt"

/**
 * Tato struktura reprezentuje jednu mnozinu 
 * (V teto semestralni praci se pracuje jenom s mnozinami HAM a SPAM)
 * Jedna mnozina obsahuje slovnik, pocet dokumentu, ktere byly zpracovany a apriorni pravdepodobnost teto mnoziny
*/
typedef struct set{
    //Slovnik teto mnoziny
    hashTable *dict;
    //Pocet nactenych dokumentu
    uint dict_file_cnt;
    //Apriorni pravdepobnost mnoziny
    double probability;
} set;

/**
 * Tato struktura reprezentuje trenovaci mnozinu
 * V teto semestralni praci bude trenovaci mnozina obsahovat 3 mnoziny
 * Mnozinu HAM, SPAM a mnozinu jejich sjednoceni
 *  
 * index mnoziny SPAM = 0
 * index mnoziny HAM = 1
 * index mnoziny sjednoceni HAM a SPAM = 2
 * 
 * set_cnt udava pocet prvku v trenovaci mnozine
*/
typedef struct trainset{
    //Mnoziny v trenovaci mnozine
    set **sets;
    //Pocet prvku v trenovaci mnozine
    uint set_cnt;
} trainset;

/**
 * Pridava do dane hash tabulky slova ze souboru
 * Do count se ulozi pocet nactenych slov
 * Metoda vraci 1 pokud se uspesne provedla nebo 0 pokud nekde selhala
*/
int load_words(const char filename[], int *count, hashTable *h);

/**
 * Nacte do hash tabulky vsechny slova ze souboru s nazvem:
 * <vzor><1-N><pripona>
 * kde vzor a N jsou dane parametrama a pripona je dana konstantou FILE_SUFFIX, ktera je definovana v bayes.h
 * Vychozi hodnota FILE_SUFFIX je ".txt" 
*/
int create_vzor_dictionary(const char vzor[], int N, hashTable *h);

/**
 * Faze uceni, zde se vypocitaji pravdepodobnosti podle algoritmu ze zadani semestralni prace
*/
void NB_learn_text(trainset *t);

/**
 * Uvolnuje pamet vyuzitou trenovaci mnozinou
 * Po uvolneni se ukazatel na trenovaci mnozinu nastavi na NULL
*/
void free_dictionary(trainset **t);

/**
 * Vytvori trenovaci mnozinu pomoci metody create_vzor_dictionary()
 * Tato trenovaci mnozina bude obsahovat mnoziny SPAM, HAM a jejich sjednoceni
 * Pocet prvku v trenovaci mnozine je nastaveny na 2 z implementacnich duvodu
*/
trainset *create_dictionary(const char spam_vzor[], int spam_file_count, const char ham_vzor[], int ham_file_count);

/**
 * Faze klasifikace podle zadani semestralni prace
 * Nacte se soubor a podle dane trenovaci mnoziny se urci zda je tento soubor SPAM nebo HAM
 * 
 * Metoda vraci: 
 * 0 pokud je soubor SPAM
 * 1 pokud je HAM
 * -1 pokud nelze soubor klasifikovat
*/
int NB_classify_text(const char doc_filename[], trainset *t);

/**
 * Faze klasifikace podle zadani semestralni prace
 * 
 * Klasifikuji se vsechny soubory s nazvem:
 * 
 * <vzor><1-N><pripona>
 * 
 * kde vzor a N jsou dane jako argumenty a pripona je dana konstantou FILE_SUFFIX, ktera je definovana v bayes.h
 * 
 * Vysledky se ulozi do souboru s nazvem danym jako argument ve stylu:
 * 
 * <nazev_souboru1>\\t<H|S|UNKNOWN>\n
 * <nazev_souboru2>\\t<H|S|UNKNOWN>\n
 * ...
 * 
 * kde 
 * H je klasifikace HAM
 * S je klasifikace SPAM
 * UNKNOWN je klasifikace neznama
*/
void NB_classify_vzor_text(const char vzor[], int vzor_count, trainset *t, const char output[]);



#endif