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

struct T_quickSort{
    int id_thread;
    int left;
    int right;
};

void* carrega(T_arq arquivos);
void* ordenaVetor(struct T_quickSort s_quick);
void* imprimeVetorPrincipal();
void* alocaVetor(T_arq *arquivos);
void* carregaVetorPrincipal(T_arq arquivos);
void* alocaVetorPrincipal();
void* quick_sort(void * s_quick);

/* Variaveis globais */
    int *vetorPrincipal = NULL;
    int qntThreads = 0;
    int qntArquivos = 0;
    int indiceGlobal = 0;
    int qntValoresTotal = 0;
    char arqSaida[50];    
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
    struct T_quickSort s_quick;

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

    s_quick.left = 0;
    s_quick.right = 0;
    /* ------------------ Threads ---------------------*/
    inicio = time(NULL);
    ordenaVetor(s_quick);
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

    for(int i = 0; i < indiceGlobal-1; i++){
        fprintf(fp, "%d\n", vetorPrincipal[i]);
    }

    fclose(fp);
 }
/* ---------------------------------- */
/* Função que chama as threads para ordenar os vetores */
void* ordenaVetor(struct T_quickSort s_quick){
    int i, k = 0, j = 0, th_id = 0, l = 0;
    pthread_t th[qntThreads-1];
   

   while(qntThreads >= 2){
        int tam = (indiceGlobal / qntThreads);

        s_quick.left = 0;
        s_quick.right = (tam-1);
        th_id = 0;

        while(l < qntThreads){
            printf("qntThreads = %d\n", qntThreads);
            printf("th_id = %d\n", th_id);
            s_quick.id_thread = th_id;
            pthread_create(&(th[th_id]), NULL, quick_sort, (void *)&s_quick);
            printf("A thread %d está ordenando o vetor neste momento.\n", th_id);
            //pthread_join(th[th_id], NULL); // <- ERRO
            th_id++;            

            printf("tam = %d\n", tam);
            printf("left = %d\n", s_quick.left);
            printf("right = %d\n", s_quick.right);

            printf("\n\n");
            s_quick.left += tam;
            s_quick.right += tam;
            l++;    
        }
        printf("1\n");
        for(i = 0; i < th_id; i++){
            pthread_join(th[i], NULL);
        }

        qntThreads /= 2;
        l = 0;
   }

   if(qntThreads < 2){
        s_quick.left = 0;
        s_quick.right = (indiceGlobal-1);
        pthread_create(&(th[0]), NULL, quick_sort, (void *)&s_quick);
        pthread_join(th[0], NULL);
   }
         
}
/* ---------------------------------- */
// Quick sort function
void* quick_sort(void * s_quick) {
    int i, j, x, y;

    struct T_quickSort *quick = (struct T_quickSort *)s_quick;
    
    i = quick->left;
    j = quick->right;
    x = vetorPrincipal[(quick->left + quick->right) / 2];
     
    while(i <= j) {
        while(vetorPrincipal[i] < x && i < quick->right) {
            i++;
        }
        while(vetorPrincipal[j] > x && j > quick->left) {
            j--;
        }
        if(i <= j) {
            y = vetorPrincipal[i];
            vetorPrincipal[i] = vetorPrincipal[j];
            vetorPrincipal[j] = y;
            i++;
            j--;
        }
    }     
    if(j > quick->left) {
        quick_sort(quick);
    }
    if(i < quick->right) {
        quick_sort(quick);
    }
}