#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
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
void* insertionSort2();
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
    char arqSaida[50];
    struct insercao insertion;    
/* ---------------------------------- */

int main(int argc, char const *argv[])
{   
    /* verificando quantidade de parametros */
    if(argc < 3){
        printf("Quantidade de parametros invalida\n");
        return 0;
    }
    time_t inicio,fim;
    int threads = atoi(argv[1]);
    qntThreads = threads;        
    qntArquivos = argc - 3;
    T_arq arquivos[qntArquivos];

    printf("Quantidade de Threads: %d\n", qntThreads);
    printf("Quantidade de arquivos de entrada: %d\n", qntArquivos);

    strcpy(arqSaida, argv[argc-1]);

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
    inicio = time(NULL);
    ordenaVetor();
    fim = time(NULL);

    imprimeVetorPrincipal();
    printf("TEMPO DE PROCESSAMENTO: %lds\n",fim-inicio);
    /* ----------------------------------------------- */
    return 0;
}

/* ---------------------------------- */
/* Aloca o vetor individual de cada arquivo lido,
de acordo com a quantidade de valores em cada arquivo */
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

    printf("Quantidade de valores do arquivo lido: %d\n", arquivos->qntValores++);

    arquivos->vetor = (int *)calloc(arquivos->qntValores, sizeof(int));
    if(arquivos->vetor == NULL){
        printf("Falha na alocacao!\n");
    }

    fclose(fp);
}
/* ---------------------------------- */
/* Carrega os vetores de cada arquivo lido, dentro da
struct arquivos */
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
    printf("Lendo arquivos de entrada.......\n");
    while(fscanf(fp, "%d", &aux) != EOF){ 
        arquivos.vetor[i] = aux;
        i++;
    }
    fclose(fp);
}
/* ---------------------------------- */
/* Carrega o vetor principal com todos os valores lidos */
void* carregaVetorPrincipal(T_arq arquivos){
    FILE* fp = fopen(arquivos.nome, "r");
    if(fp == NULL){
        printf("Arquivo nao encontrado!\n");
        pthread_exit(NULL);
    }

    printf("Carregando vetor principal....\n");

    for(int i = 0; i < arquivos.qntValores; i++){
        vetorPrincipal[indiceGlobal] = arquivos.vetor[i];
        indiceGlobal++;
    }    

    fclose(fp);
}

/* ---------------------------------- */
/* Realiza a alocação de vetor principal de acordo com a 
quantidade total de valores lidos */
void* alocaVetorPrincipal(){
    if(vetorPrincipal == NULL){
        printf("Vetor nao foi alocado ainda\nAlocando...\n");
        vetorPrincipal = (int *)calloc(qntValoresTotal, sizeof(int));
    }else{
        printf("ERRO\n");
    }
}

/* ---------------------------------- */
/* Imprime o vetor ordenado em um arquivo de saída */
 void* imprimeVetorPrincipal(){

    FILE *fp = fopen(arqSaida, "w");
    if(fp == NULL){
        printf("ERRO\n");
    }

    printf("Escrevendo no arquivo de saida .......... \n");

    for(int i = 0; i < indiceGlobal; i++){
        fprintf(fp, "%d\n", vetorPrincipal[i]);
    }

    fclose(fp);
 }
/* ---------------------------------- */
/* Função que chama as threads para ordenar os vetores */
void* ordenaVetor(){
    int tamMiniVetor = 0;
    int i, k = 0, j = 0, th_id = 0, log, l = 0;
    pthread_t th[qntThreads];

    tamMiniVetor = indiceGlobal / qntThreads;
    insertion.tam = tamMiniVetor;

    insertion.vet = (int *) calloc (tamMiniVetor, sizeof(int));

    printf("Ordenando vetor..........\n");
    while(l < qntThreads){
        
        for(i = 0; i < tamMiniVetor; i++, k++){
            insertion.vet[i] = vetorPrincipal[k];
        }

        pthread_create(&(th[th_id]), NULL, insertionSort, NULL);
        pthread_join(th[th_id], NULL);
        th_id++;

        for(i = 0; i < tamMiniVetor; i++, j++){
            vetorPrincipal[j] = insertion.vet[i];
        }
        printf("\n\n");
        l++;
    }

    pthread_create(&(th[0]), NULL, insertionSort2, NULL);
    //pthread_join(th[0], NULL);    
}
/* ---------------------------------- */
/* Ordena o vetor secundario */
void* insertionSort(){

    int i, j, valorAtual;
 
   for( j=1; j < insertion.tam-1; j++ ) 
   {
      valorAtual = insertion.vet[j];
      i = j-1;
      
      while(i >= 0 && insertion.vet[i] > valorAtual)
      {
        insertion.vet[i+1] = insertion.vet[i];
        i--;
      } 
              
      insertion.vet[i+1] = valorAtual;
   }
    
    pthread_exit(NULL);
}

/* ---------------------------------- */
/* Ordena o vetor principal */
void* insertionSort2(){
    int i, j, valorAtual;
 
   for( j=1; j < indiceGlobal; j++ ) 
   {
      valorAtual = vetorPrincipal[j];
      i = j-1;
      
      while(i >= 0 && vetorPrincipal[i] > valorAtual)
      {
        vetorPrincipal[i+1] = vetorPrincipal[i];
        i--;
      } 
              
      vetorPrincipal[i+1] = valorAtual;
   }
   pthread_exit(NULL);
}