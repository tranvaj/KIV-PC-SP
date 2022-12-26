#ifndef __HASHTABLE__
#define __HASHTABLE__
//Pocatecni kapacita hash tabulky
#define HASHTABLE_CAPACITY_INIT 100
//Konstanta urcujici o kolik se zvysi kapacita hashtabulky pri rehashovani (aby se udrzela konstantni amortizovana slozitost)
#define INCREASE_CONSTANT 2
//Max velikost jednoho kybliku hash tabulky, tj. kolik max prvku muze byt na jednom indexu hash tabulky
#define MAX_BUCKET_SIZE 50

typedef unsigned int uint;

typedef struct node node;

/**
 * Prvek (uzel), ktery bude ukladan do hash tabulky
*/
struct node{
    //Nazev slova
    char *key;
    //Frekvence slova
    uint freq;
    //Pravdepodobnost, ze se slovo tohoto uzlu vyskytne v dokumentu za podminky klasifikace do tridy spam
    double p_spam;
    //Pravdepodobnost, ze se slovo tohoto uzlu vyskytne v dokumentu za podminky klasifikace do tridy ham
    double p_ham;
    //Ukazovatel na dalsi uzel
    node *next;
};

/**
 * Tabulka s rozptylenymi polozkami, ktera vyuziva zretezeni podle spojoveho seznamu
*/
typedef struct hashtable{
    //Ukazovatel na ukazovatele uzlu, zde jsou ulozeny vsechny prvky tabulky
    node **arr;  
    //Kapacita tabulky
    uint capacity;
    //Pocet prvku v tabulce
    uint count; 
    //Unikatni pocet prvku v tabulce
    uint uq_item_cnt; 
} hashtable;

/**
 * Vytvori tabulku s roztylenimi polozkami s danou kapacitou
*/
hashtable *create_hashtable(uint capacity);

/**
 * Uvolni pamet vyuzitou hashovaci tabulkou
 * Po uvolneni se ukazatel na hash tabulku nastavi na NULL 
*/
void free_hashtable(hashtable **h);

/**
 * Vlozi novy uzel do hash tabulky s danym klicem/slovem
 * Pokud uzel s danym klicem/slovem uz existuje, zmeni se frekvence slova tohoto uzlu o +1
 * Provede se rehash pokud velikost kybliku bude presahnuta
 * Vraci 0 pokud byla metoda neuspesna
 * Vraci 1 pokud byla metoda uspesna
*/
int add_item(hashtable *h, const char *key);

/**
 * Hashovaci funkce djb2 od Dan Bernstein
 * Prevzato z http://www.cse.yorku.ca/~oz/hash.html
*/
uint hash_func(const char *key, uint size);

/**
 * Vraci frekvenci slova v hash tabulce
*/
uint get_freq(hashtable *h, char *key);

/**
 * Vraci uzel, ktery ma stejny nazev klice jako retezec argumentu
*/
node *get_node(hashtable *h, char *key);

/**
 * Zvysi kapacitu "ukazatele na ukazatele" o k-krat, kde k je dana konstantou INCREASE_CONSTANT
 * Kapacita tabulky se k-krat navysi
 * Puvodni prvky se presunou do noveho "ukazatele na ukazatele" (nevytvari se znova)
 * Puvodni "ukazatel na ukazatele" se uvolni
*/
int rehash(hashtable *h);

#endif