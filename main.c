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

typedef struct insercao{
    int *vet;
    int tam;
}insercao;

void* carrega(T_arq arquivos);
void* ordenaVetor();
void* insertionSort();
void* imprimeVetorPrincipal();
void* alocaVetor(T_arq *arquivos);
void* carregaVetorPrincipal(T_arq arquivos);
void* alocaVetorPrincipal();

/* Variaveis globais */
    int *vetorPrincipal = NULL;
    int qntThreads = 0;
    int qntArquivos = 0;
    int indiceGlobal = 0;
    int qntValoresTotal = 0;
    //int th_id = 0;
    char arqSaida[50];
    struct insercao insertion;
    

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
        
    qntArquivos = argc - 3;
    T_arq arquivos[qntArquivos];
    strcpy(arqSaida, argv[argc-1]);

    
   /* printf("saida: %s\n", arqSaida);
    printf("qntArquivos: %d\n", qntArquivos);*/

    /* Copiando o nome do arquivo para a struct */    
    for(int i = 0; i < qntArquivos; i++){
        strcpy(arquivos[i].nome, argv[i+2]);
    }
    for(int i = 0; i < qntArquivos; i++){
        alocaVetor(&arquivos[i]);
    }
    for(int i = 0; i < qntArquivos; i++){
        qntValoresTotal += arquivos[i].qntValores;
    }
    alocaVetorPrincipal();
    for(int i = 0; i < qntArquivos; i++){
        carrega(arquivos[i]);
    }
            
    for(int i = 0; i < qntArquivos; i++){
        carregaVetorPrincipal(arquivos[i]);
    }
    
   
    /* ------------------ Threads ---------------------*/
    gettimeofday(&inicial, NULL);
    ordenaVetor();
    gettimeofday(&final, NULL);

    imprimeVetorPrincipal();
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
}

void* carregaVetorPrincipal(T_arq arquivos){

    FILE* fp = fopen(arquivos.nome, "r");
    if(fp == NULL){
        printf("Arquivo nao encontrado!\n");
        pthread_exit(NULL);
    }

    printf("-------------------------------------------\n");

    for(int i = 0; i < arquivos.qntValores; i++){
        vetorPrincipal[indiceGlobal] = arquivos.vetor[i];
        //printf("%d (%d) ||| %d (%d)\n", vetorPrincipal[indiceGlobal], indiceGlobal, arquivos.vetor[i], i);
        indiceGlobal++;
    }    

    fclose(fp);

}

/* ---------------------------------- */

void* alocaVetorPrincipal(){
    if(vetorPrincipal == NULL){
        printf("Vetor nao foi alocado ainda\nAlocando...\n");
        vetorPrincipal = (int *)calloc(qntValoresTotal, sizeof(int));
    }else{
        printf("ERRO\n");
    }
}

/* ---------------------------------- */

 void* imprimeVetorPrincipal(){

     FILE *fp = fopen(arqSaida, "w");
     if(fp == NULL){
         printf("ERRO\n");
     }

     for(int i = 0; i < indiceGlobal; i++){
         fprintf(fp, "%d\n", vetorPrincipal[i]);
     }

     fclose(fp);
 }
/* ---------------------------------- */
void* ordenaVetor(){
    int *vet, tamMiniVetor = 0;
    int i, k = 0, j = 0, th_id = 0, log, l = 0;
    pthread_t th[qntThreads];

    tamMiniVetor = indiceGlobal / qntThreads;

    insertion.vet = (int *) calloc (tamMiniVetor, sizeof(int));
    printf("qntThreads: %d\n", qntThreads);
    while(l < qntThreads){
        
        for(i = 0; i < tamMiniVetor; i++, k++){
            insertion.vet[i] = vetorPrincipal[k];
        }
        k++;
        log = pthread_create(&(th[th_id]), NULL, insertionSort, NULL);
        printf("log : %d\n", log);
        pthread_join(th[th_id], NULL);
        th_id++;
        for(i = 0; i < tamMiniVetor; i++, j++){
            vetorPrincipal[j] = insertion.vet[i];
            //printf("%d ", insertion.vet[i]);
            //printf("\n i : %d\n", i);
        }
    }
    
}
/* ---------------------------------- */
void* insertionSort(){
    //insercao* in = (insercao *)insertion;
    printf("%d ", insertion.vet[0]);
    int i, j, x;
    for (i = 2; i <= insertion.tam; i++){
        x = insertion.vet[i];
        j = i-1;
        insertion.vet[0] = x; 
        while (x < insertion.vet[j]){
            insertion.vet[j+1] = insertion.vet[j];
            j--;
        }
        insertion.vet[j+1] = x;
        printf("%d ", insertion.vet[i]);
    }

    for(int i = 0; i < insertion.tam; i++){
        printf("%d ", insertion.vet[i]);
    }
    pthread_exit(NULL);
}
