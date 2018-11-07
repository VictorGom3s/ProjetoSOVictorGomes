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
    int tamMiniVetor = 0;
    int i, k = 0, j = 0, th_id = 0, log, l = 0;
    pthread_t th[qntThreads];

    tamMiniVetor = indiceGlobal / qntThreads;
    insertion.tam = tamMiniVetor;

    insertion.vet = (int *) calloc (tamMiniVetor, sizeof(int));
    printf("qntThreads: %d\n", qntThreads);
    while(l < qntThreads){
        
        for(i = 0; i < tamMiniVetor; i++, k++){
            insertion.vet[i] = vetorPrincipal[k];
        }

        pthread_create(&(th[th_id]), NULL, insertionSort, NULL);
        pthread_join(th[th_id], NULL);
        th_id++;

        for(i = 0; i < tamMiniVetor; i++, j++){
            vetorPrincipal[j] = insertion.vet[i];
            //printf("%d ", insertion.vet[i]);
        }
        printf("\n\n");
        l++;
    }


    pthread_create(&(th[0]), NULL, insertionSort2, NULL);

    
}
/* ---------------------------------- */
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

   /*printf("mini vetor\n");
   for(int i = 0; i < indiceGlobal; i++){
       printf("%d ", insertion.vet[i]);
   }*/

    /*//insercao* in = (insercao *)insertion;
    printf("prim %d \n", insertion.vet[0]);
    int i, j, x;
    for (i = 1; i <= insertion.tam; i++){
        x = insertion.vet[i];
        j = i-1;
        insertion.vet[0] = x; 
        while (x < insertion.vet[j]){
            insertion.vet[j+1] = insertion.vet[j];
            j--;
        }
        insertion.vet[j+1] = x;
    }*/

    
    pthread_exit(NULL);
}


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

   /*printf("vetor principal\n");
   for(int i = 0; i < indiceGlobal; i++){
       printf("%d ", vetorPrincipal[i]);
   }*/
}

