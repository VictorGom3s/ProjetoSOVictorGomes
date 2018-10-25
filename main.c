#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct arquivo{
    char nome[50];
    int pos;
    int qntValores;
    int *vetor;
}T_arq;

void* carrega(T_arq arquivos);
void quick(int *a, int left, int right);
void* imprimeVetorPrincipal();
void* alocaVetor(T_arq *arquivos);
void* carregaVetorPrincipal(T_arq arquivos);

/* Variaveis globais */
    int *vetorPrincipal;
    int qntArquivos = 0;
    int MAX = 0;

//     0       1      2       3       4         5
//  ./multicat 16 arq1.in arq2.in arq3.in arqSaida.out
int main(int argc, char const *argv[])
{   
    /* verificando quantidade de parametros */
    if(argc < 3){
        printf("Quantidade de parametros invalida\n");
        return 0;
    }
    
    int threads = atoi(argv[1]);
    qntArquivos = argc - 2;
    T_arq arquivos[qntArquivos];

    printf("qntArquivos: %d\n", qntArquivos);

    /* Copiando o nome do arquivo para a struct */    
    for(int i = 0; i < qntArquivos; i++){
        strcpy(arquivos[i].nome, argv[i+2]);
    }

    for(int i = 0; i < qntArquivos; i++){
        alocaVetor(&arquivos[i]);
    }
    for(int i = 0; i < qntArquivos; i++){
        carrega(arquivos[i]);
    }
        
    for(int i = 0; i < qntArquivos; i++){
        carregaVetorPrincipal(arquivos[i]);
    }


    imprimeVetorPrincipal();
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

    if(vetorPrincipal == NULL){
        printf("Vetor nao foi alocado ainda\n Alocando...\n");
        vetorPrincipal = (int *)calloc(arquivos.qntValores, sizeof(int));
    }else{
        vetorPrincipal = (int *)realloc(arquivos.vetor, sizeof(int));
    }

    printf("-------------------------------------------\n");
    for(int i = 0; i < arquivos.qntValores; i++){
        MAX++;
        vetorPrincipal[i] = arquivos.vetor[i];
        //printf("%d (%d) ||| %d (%d)\n", vetorPrincipal[i], i, arquivos.vetor[i], i);
    }

    quick(vetorPrincipal ,0 , MAX-1);

    fclose(fp);
    //pthread_exit(NULL);
}

void quick(int *a, int left, int right){
    int i, j, x, y;
     
    i = left;
    j = right;
    x = a[(left + right) / 2];
     
    while(i <= j) {
        while(a[i] < x && i < right) {
            i++;
        }
        while(a[j] > x && j > left) {
            j--;
        }
        if(i <= j) {
            y = a[i];
            a[i] = a[j];
            a[j] = y;
            i++;
            j--;
        }
    }
     
    if(j > left) {
        quick(a, left, j);
    }
    if(i < right) {
        quick(a, i, right);
    }
 }

 void* imprimeVetorPrincipal(){

     for(int i = 0; i < MAX; i++){
         printf("%d (%d)\n", vetorPrincipal[i], i);
     }
 }
