#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX 100
typedef struct {
    char name[MAX];
    char **sites;
    int siteCounter;
    int i;
} anahtarKelime;

typedef struct {
    anahtarKelime* dizi;
    int elemanSay;
}distinctWords;

char **allocate_dynamic_matrix_int(int row, int col)
{
    char **ret_val;
    int i;

    ret_val = (char **)calloc(row, sizeof(char *) * row);
    if (ret_val == NULL) {
        perror("memory allocation failure");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < row; ++i) {
        ret_val[i] = (char *)calloc(col, sizeof(char));
        if (ret_val[i] == NULL) {
            perror("memory allocation failure");
            exit(EXIT_FAILURE);
        }
    }

    return ret_val;
}




unsigned int hash(char *name){
    unsigned int hashVal = 0;
    char character;
    for (character = *name; character != '\0'; character = *++name){
        int tmp = character - 'A' + 1;
        hashVal = 31 * hashVal + tmp;
    }
    return hashVal;
}
void init_hash_table(anahtarKelime *hash_table[], int table_size){
    int i;
    for (i = 0; i<table_size; i++){
        hash_table[i] = NULL;
    }
}
int hashInsert(anahtarKelime *hashTable[], anahtarKelime *k, int tableSize, int searchEnable){
    int i = 0;
    unsigned int index = hash(k->name);
    while (1){
        int try = (i+index) % tableSize;
        if (hashTable[try] != NULL && strcmp(hashTable[try]->name,k->name) != 0 && i<tableSize){
            i++;
        }
        else{
            if(i<tableSize){
                if(hashTable[try] == NULL){
                    if(searchEnable == 1)
                        return -2;
                    k->i = i;
                    hashTable[try] = k;
                    return 1;
                }
                else{
                    return try;
                }
            }
            else {
                printf("table full");
                return -1;
            }
        }
    }
}
void printHash(anahtarKelime *hashTable[], int tableSize){
    int i,j;
    printf("\n");
    for (i = 0; i<tableSize; i++){
        if(hashTable[i] == NULL)
            printf("\t%d\t---\n", i);
        else {
            printf("\t%d\t%s\n", i, hashTable[i]->name);
        }
    }
}
char **kelimeleriAyir(char *cumle, char **sepCumle){
    int i, j, k = 0;
    for(i = 0; i<strlen(cumle); i++){
        if(cumle[i] == ' ' || cumle[i] == '\0' || cumle[i] == '\n'){
            j = 0;
            k++;

        }
        else{
            sepCumle[k][j] = cumle[i];
            j++;
        }
    }
    return sepCumle;
}

char **inputOku(){
    char cumle[30];
    char **sepCumle;

    sepCumle = allocate_dynamic_matrix_int(3, 30);
    fflush(stdin);
    printf("\nAramak istediginiz anahtarKelime/kelimeleri giriniz--buyuk kucuk hassasiyetli--(Cikis icin 0 giriniz): ");


    if (fgets(cumle, sizeof cumle, stdin) == NULL ) {
        exit(1);
    }
    return kelimeleriAyir(cumle, sepCumle);

}


void siteAraYazdir(anahtarKelime *hashTable[], unsigned int arananIndex, char **cumle) {
    int i;
    if (strcmp(hashTable[arananIndex]->name, cumle[0]) == 0) {
        printf("\n\tKelime: %s\n", hashTable[arananIndex]->name);
        printf("\n\tBulundugu siteler: \n");
        for (i = 0; i <  hashTable[arananIndex]->siteCounter; i++)
            printf("\t%d\t%s\n",i+1, hashTable[arananIndex]->sites[i]);
    }
}
void printIntersect(anahtarKelime *hashTable[], unsigned int aranan1Index, unsigned int aranan2Index){
    int i, j, k = 0;
    printf("\n\tOrtak Siteler:\n");
    for (i = 0; i <  hashTable[aranan1Index]->siteCounter; ++i) {
        for (j = 0; j < hashTable[aranan2Index]->siteCounter; ++j) {
            if (strcmp(hashTable[aranan1Index]->sites[i], hashTable[aranan2Index]->sites[j]) == 0) {
                printf("\t%d\t%s\n",k+1, hashTable[aranan1Index]->sites[i]);
                k++;
            }
        }
    }
    if (k == 0)
        printf("\t%s ve %s kelimelerinin ortak olarak bulundugu site bulunamadi.\n", hashTable[aranan1Index]->name, hashTable[aranan2Index]->name);
}
void printUnion(anahtarKelime *hashTable[], unsigned int aranan1Index, unsigned int aranan2Index){
    int i, j, isFound,k;
    printf("\n\tBirlesmis Siteler:\n");
    printf("\t%s:\n", hashTable[aranan1Index]->name);
    for (i = 0; i < hashTable[aranan1Index]->siteCounter; ++i) {
        printf("\t%d\t%s\n",i+1, hashTable[aranan1Index]->sites[i]);
        k = i+1;
    }
    k++;
    printf("\t%s:\n", hashTable[aranan2Index]->name);
    for (i = 0; i < hashTable[aranan2Index]->siteCounter; ++i) {
        isFound = 0;
        for (j = 0; j < hashTable[aranan1Index]->siteCounter; ++j) {
            if (strcmp(hashTable[aranan1Index]->sites[j], hashTable[aranan2Index]->sites[i]) == 0) {
                isFound = 1;
            }
        }
        if (isFound == 0) {
            printf("\t%d\t%s\n", k, hashTable[aranan2Index]->sites[i]);
            k++;
        }
    }
}

int kelimeAra(anahtarKelime *hashTable[], char *cumle, int tableSize){
    anahtarKelime *aranan;
    aranan = (anahtarKelime *) malloc(sizeof(anahtarKelime));
    strcpy(aranan->name, cumle);
    return hashInsert(hashTable, aranan, tableSize, 1);
}
void araVeYazdir(anahtarKelime *hashTable[], char **cumle, int tableSize) {
    if (strcmp(cumle[1], "veya") != 0 && strcmp(cumle[1], "ve") != 0) {
        int index = kelimeAra(hashTable, cumle[0], tableSize);
        if (index != -2) {
            siteAraYazdir(hashTable, index, cumle);
        } else {
            printf("\tBu kelimenin gectigi bir site yoktur.\n");
        }
    } else {
        int indexA = kelimeAra(hashTable, cumle[0], tableSize);
        int indexB = kelimeAra(hashTable, cumle[2], tableSize);
        if (indexA != -2 && indexB != -2) {
            if (strcmp(cumle[1], "ve") == 0) {
                printIntersect(hashTable, indexA, indexB);
            } else if (strcmp(cumle[1], "veya") == 0) {
                printUnion(hashTable, indexA, indexB);
            }
        } else{
            if (indexA == -2)
                printf("\t%s kelimesinin gectigi bir site bulunamadi \n", cumle[0]);
            if(indexB == -2)
                printf("\t%s kelimesinin gectigi bir site bulunamadi \n", cumle[2]);
        }

    }
}
distinctWords *dosyaOku(char fileName[]){
    int i, isNew, j;
    distinctWords *readWords;
    readWords =(distinctWords *) malloc(sizeof(distinctWords));
    char word[MAX];
    char site[MAX];

    FILE *in_file;

    readWords->dizi = (anahtarKelime *)malloc(sizeof(anahtarKelime) * 100);
    for (i = 0; i<100; i++){
        readWords->dizi[i].sites = allocate_dynamic_matrix_int(30, 50);
    }

    if (readWords == NULL) {
        perror("memory allocation failure");
        exit(EXIT_FAILURE);
    }
    readWords->elemanSay = 0;

    in_file = fopen(fileName, "r");

    int m = 0;
    if (in_file == NULL){
        perror("cannot open file");
        exit(EXIT_FAILURE);
    }
    else {
        while (fscanf(in_file, "%s", word) != EOF)
        {
            m++;
            isNew = 0;
            if(!strstr(word, "http")){

                for(j = 0; j < readWords->elemanSay + 1; j++){
                    if(strcmp(readWords->dizi[j].name, word) == 0){


                        strcpy(readWords->dizi[j].sites[readWords->dizi[j].siteCounter], site);
                        (readWords->dizi[j].siteCounter)++;
                        isNew = 1;

                    }
                }

                if (isNew !=1){
                    strcpy(readWords->dizi[readWords->elemanSay].name, word);
                    readWords->dizi[readWords->elemanSay].siteCounter = 0;
                    strcpy(readWords->dizi[readWords->elemanSay].sites[readWords->dizi[readWords->elemanSay].siteCounter], site);
                    readWords->dizi[readWords->elemanSay].siteCounter += 1;
                    readWords->elemanSay++;
                }
            }

            else {
                strcpy(site, word);
            }
        }
        fclose(in_file);
    }
    return readWords;
}
int min(int num1, int num2)
{
    return (num1 > num2 ) ? -num2 : num1;
}
int isPrime(int N)
{
    int i;
    for (i = 2; i <= sqrt(N); i++) {
        if (N % i == 0)
            return 0;
    }
    return 1;
}

int findTableSize(int elemanSay, float loadFactor) {
    int tableSize = elemanSay / loadFactor;
    int run = 1;
    if (tableSize == 0)
        return 2;
    else if (tableSize == 1)
        return 1;
    else if (isPrime(tableSize))
        return 0;

    int aboveN, belowN;
    int n1;

    n1 = tableSize + 1;
    while (run) {
        if (isPrime(n1)) {
            aboveN = n1;
            run = 0;
        } else
            n1++;
    }

    n1 = tableSize - 1;
    run = 1;
    while (run) {
        if (isPrime(n1)) {
            belowN = n1;
            run = 0;
        } else
            n1--;
    }

    int diff1 = aboveN - tableSize;
    int diff2 = tableSize - belowN;
    tableSize = min(diff1, diff2) + tableSize;
    if (tableSize >= elemanSay)
        return tableSize;
    else {
        tableSize = diff1 + tableSize;
    }
    return tableSize;
}

int main() {
    char fileName[20] = "Sample.txt";
    int i, run =1, mod, j;
    distinctWords * words;
    words = dosyaOku(fileName);
    float load_factor;
    printf("Load Factor'u giriniz: ");
    scanf("%f", &load_factor);
    int table_size = findTableSize(words->elemanSay, load_factor);
    anahtarKelime *hashTable[table_size];
    init_hash_table(hashTable, table_size);
    printf("Mod(0-Normal, 1-Detayli): ");
    scanf("%d", &mod);
    if(mod == 1){
        printf("\n**********************************************************\n");
        printf("\n\tMod: \tDetayli Mod");
        printf("\n\tHash tablosunun uzunlugu: \t%d\n", table_size);
        printf("\n**********************************************************\n");
        printf("----------------------------------------------------------\n");
    }
    for(i = 0; i<words->elemanSay; i++){
        hashInsert(hashTable, &words->dizi[i], table_size, 0);
        if(mod == 1){
            printf("\tEklenen Kelime:\t%s\n", words->dizi[i].name);
            printf("\n\tHash Tablosuna %d Denemede Yerlestirildi.\n", (words->dizi[i].i+1));
            printf("\n\tKelimenin Gectigi Siteler: \n");
            for (j = 0; j < words->dizi[i].siteCounter; ++j) {
                printf("\t%d\t%s\n",j+1, words->dizi[i].sites[j]);
            }
            printf("----------------------------------------------------------\n");
        }
    }
    if(mod == 1){
        printHash(hashTable, table_size);
    }
    while (run){
        char **cumle;
        cumle = inputOku();
        if (strcmp(cumle[0], "0") == 0)
            run = 0;
        else araVeYazdir(hashTable, cumle, table_size);
    }

    return 0;
}

