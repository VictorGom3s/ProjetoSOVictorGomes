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
int quick_sort(int *vetorPrincipal, int left, int right);
void * preparaQsort(void * s_quick);

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

    for(int i = 0; i < (indiceGlobal-1); i++){
        fprintf(fp, "%d\n", vetorPrincipal[i]);
    }

    fclose(fp);
 }
/* ---------------------------------- */
/* Função que chama as threads para ordenar os vetores */
void* ordenaVetor(struct T_quickSort s_quick){
    int i, k = 0, j = 0, th_id = 0, l = 0;
    pthread_t th[qntThreads-1];
    printf("Foram criadas %d threads\n", qntThreads);   

   while(qntThreads >= 2){
        int tam = (indiceGlobal / qntThreads);
        s_quick.left = 0;
        s_quick.right = (tam-1);
        th_id = 0;

        while(l < qntThreads){
            s_quick.id_thread = th_id;
            if(pthread_create(&(th[th_id]), NULL, preparaQsort, (void *)&s_quick) == 0){
                printf("A thread %d está ordenando o vetor neste momento.\n", th_id);
            }
            th_id++;            

            printf("tam = %d\n", tam);
            printf("left = %d\n", s_quick.left);
            printf("right = %d\n", s_quick.right);

            printf("\n\n");
            s_quick.left += tam;
            s_quick.right += tam;
            l++;
        }
        printf("Fim do ciclo\n");
        for(i = 0; i < th_id; i++){
            pthread_join(th[i], NULL);
            printf("Thread %d terminou\n", i);
        }
        qntThreads /= 2;
        l = 0;
   }
    /* Todas as threads já ordenaram parte do vetor
    agora, a thread 0 deve percorrer ordenando o que tiver desordenado */ 
    if(qntThreads < 2){
        s_quick.left = 0;
        s_quick.right = (indiceGlobal-1);
        printf("Estou aqui 2\n");
        pthread_create(&(th[0]), NULL, preparaQsort, (void *)&s_quick);
        printf("Thread %d terminou\n", 0);
        pthread_join(th[0], NULL);
    }         
}

void * preparaQsort(void * s_quick){
    struct T_quickSort *quick = (struct T_quickSort *)s_quick;

    int left, right;

    left = quick->left;
    right = quick->right;

    quick_sort(vetorPrincipal, left, right);
    pthread_exit(NULL);
}

/* ---------------------------------- */
// Quick sort function
int quick_sort(int *vetorPrincipal, int left, int right) {
    int i, j, x, y;
    
    i = left;
    j = right;
    x = vetorPrincipal[(left + right) / 2];
     
    while(i <= j) {
        while(vetorPrincipal[i] < x && i < right) {
            i++;
        }
        while(vetorPrincipal[j] > x && j > left) {
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
    if(j > left) {
        quick_sort(vetorPrincipal, left, j);
    }
    if(i < right) {
        quick_sort(vetorPrincipal, i, right);
    }
}

/*void* naoFazNada(void * s_quick){
    struct T_quickSort *quick = (struct T_quickSort *)s_quick;
    printf("\nEsta funcao nao faz nada e foi executada pela thread %d\n\n", quick->id_thread);
    pthread_exit(NULL);
}*/