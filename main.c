#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct arquivo{
    char nome[50];
    int pos;
    int qntValores;
    int *vetor;
}T_arq;

struct T_quick{
    int *a;
    int left;
    int right;
    pthread_t *th;
};

void* carrega(T_arq arquivos);
void* quick(void* T_quick);
void* imprimeVetorPrincipal();
void* alocaVetor(T_arq *arquivos);
void* carregaVetorPrincipal(T_arq arquivos);
void* alocaVetorPrincipal();

/* Variaveis globais */
    //int *vetorPrincipal = NULL;
    int qntThreads = 0;
    int qntArquivos = 0;
    int indiceGlobal = 0;
    int qntValoresTotal = 0;
    int th_id = 0;
    char arqSaida[50];
    struct T_quick quicksort;
    

//     0       1      2       3       4         5
//  ./multicat 16 arq1.in arq2.in arq3.in arqSaida.out
int main(int argc, char const *argv[])
{   
    struct timeval inicial, final;
    /* verificando quantidade de parametros */
    if(argc < 3){
        printf("Quantidade de parametros invalida\n");
        return 0;
    }
    
    int threads = atoi(argv[1]);
    qntThreads = threads;
    
    /* Alocando as threads */
    quicksort.th = (pthread_t *)malloc(threads * sizeof(pthread_t));
    quicksort.a = NULL;    
    /* ------------ */
    
    qntArquivos = argc - 3;
    T_arq arquivos[qntArquivos];
    strcpy(arqSaida, argv[argc-1]);

    
   /* printf("saida: %s\n", arqSaida);
    printf("qntArquivos: %d\n", qntArquivos);*/

    printf("1\n");
    /* Copiando o nome do arquivo para a struct */    
    for(int i = 0; i < qntArquivos; i++){
        strcpy(arquivos[i].nome, argv[i+2]);
    }
    printf("2\n");
    for(int i = 0; i < qntArquivos; i++){
        alocaVetor(&arquivos[i]);
    }
    printf("3\n");
    for(int i = 0; i < qntArquivos; i++){
        qntValoresTotal += arquivos[i].qntValores;
    }
    printf("4\n");
    alocaVetorPrincipal();
    printf("5\n");
    for(int i = 0; i < qntArquivos; i++){
        carrega(arquivos[i]);
    }
    printf("6\n");        
    for(int i = 0; i < qntArquivos; i++){
        carregaVetorPrincipal(arquivos[i]);
    }
    printf("7\n");
    quicksort.left = 0;
    quicksort.right = (indiceGlobal-1);
    /* ------------------ Threads ---------------------*/
    gettimeofday(&inicial, NULL);
    printf("8\n");
    pthread_create(&(quicksort.th[th_id]), NULL, quick, (void *)&quicksort);
    pthread_join(quicksort.th[th_id], NULL);
    printf("9\n");
    imprimeVetorPrincipal();
    printf("10\n");
    gettimeofday(&final, NULL);

    printf("TEMPO DE PROCESSAMENTO: %ld milisegundos\n", (final.tv_usec-inicial.tv_usec)/1000);
    /* ----------------------------------------------- */
    return 0;
}

void* alocaVetor(T_arq *arquivos){
    int aux;
    arquivos->qntValores = 0;

    FILE* fp = fopen(arquivos->nome, "r");
    if(fp == NULL){
        printf("Arquivo nao encontrado!\n");
        return NULL;
    }

    while(fscanf(fp, "%d", &aux) != EOF){
        arquivos->qntValores++;
    }

    arquivos->vetor = (int *)calloc(arquivos->qntValores, sizeof(int));
    if(arquivos->vetor == NULL){
        printf("Falha na alocacao!\n");
    }

    fclose(fp);
   //pthread_exit(NULL);
}


void* carrega(T_arq arquivos){
    int i = 0, aux = 0;

    FILE* fp = fopen(arquivos.nome, "r");
    if(fp == NULL){
        printf("Arquivo nao encontrado!\n");
        pthread_exit(NULL);
    }

    if(arquivos.vetor == NULL){
        printf("Falha na alocacao!\n");
        pthread_exit(NULL);
    }

    while(fscanf(fp, "%d", &aux) != EOF){ 
        arquivos.vetor[i] = aux;
        //printf(" %d (%d)\n", arquivos.vetor[i], aux);
        i++;
    }
    fclose(fp);
    //pthread_exit(NULL);
}

void* carregaVetorPrincipal(T_arq arquivos){

    FILE* fp = fopen(arquivos.nome, "r");
    if(fp == NULL){
        printf("Arquivo nao encontrado!\n");
        pthread_exit(NULL);
    }

    printf("-------------------------------------------\n");

    for(int i = 0; i < arquivos.qntValores; i++){
        quicksort.a[indiceGlobal] = arquivos.vetor[i];
        //printf("%d (%d) ||| %d (%d)\n", vetorPrincipal[indiceGlobal], indiceGlobal, arquivos.vetor[i], i);
        indiceGlobal++;
    }    

    fclose(fp);
    //pthread_exit(NULL);
}

/* ---------------------------------- */

void* alocaVetorPrincipal(){
    if(quicksort.a == NULL){
        printf("Vetor nao foi alocado ainda\nAlocando...\n");
        quicksort.a = (int *)calloc(qntValoresTotal, sizeof(int));
    }else{
        printf("ERRO\n");
    }
}

/* ---------------------------------- */

void* quick(void* T_quick){
    int i, j, x, y;
    th_id++;    
    i = quicksort.left;
    j = quicksort.right;    
    x = quicksort.a[(quicksort.left + quicksort.right) / 2];    
    while(i <= j) {
        while(quicksort.a[i] < x && i < quicksort.right) {
            i++;
        }
        while(quicksort.a[j] > x && j > quicksort.left) {
            j--;
        }
        if(i <= j) {
            y = quicksort.a[i];
            quicksort.a[i] = quicksort.a[j];
            quicksort.a[j] = y;
            i++;
            j--;
        }
    }
    if(j > quicksort.left) {
        quicksort.right = j;
        quick(&quicksort);
    }
    if(i < quicksort.right) {
        quicksort.left = i;
        quick(&quicksort);
    }
    pthread_exit(NULL);
    /*printf("j = %d\n", j);
     printf("23\n");
    if(j > quicksort.left) {
        printf("24\n");
        quicksort.right = j;
        printf("J = %d\n", quicksort.right);
        /*if(th_id < qntThreads){
            printf("25\n");
            pthread_create(&(quicksort.th[th_id]), NULL, quick, (void *)&quicksort);
        }else{*/
            //quick(&quicksort);
        //}
   /* }
    printf("26\n");
    if(i < quicksort.right) {
        quicksort.left = i;
         /*if(th_id < qntThreads){
            printf("25\n");
            pthread_create(&(quicksort.th[th_id]), NULL, quick, (void *)&quicksort);
        }else{*/
            //quick(&quicksort);
        //}
    //}
 }
    
/* ---------------------------------- */

 void* imprimeVetorPrincipal(){

     FILE *fp = fopen(arqSaida, "w");
     if(fp == NULL){
         printf("ERRO\n");
     }

     for(int i = 0; i < indiceGlobal; i++){
         fprintf(fp, "%d\n", quicksort.a[i]);
     }

     fclose(fp);
     //pthread_exit(NULL);
 }
