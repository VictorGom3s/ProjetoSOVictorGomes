#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

/* Struct que armazena informações de cada arquivo */
typedef struct arquivo{ 
    char nome[50];
    int pos;
    int qntValores;
    int *vetor;
}T_arq;


/* Struct que armazena informações a serem passadas pelas
threads a funcao quicksort */
struct T_quickSort{
    int id_thread;
    int left;
    int right;
};

/* ----------- Funcoes ------------- */
void* carrega(T_arq arquivos);
void* ordenaVetor(struct T_quickSort s_quick);
void* imprimeVetorPrincipal();
void* alocaVetor(T_arq *arquivos);
void* carregaVetorPrincipal(T_arq arquivos);
void* alocaVetorPrincipal();
void * preparaQsort(void * s_quick);
int comparaCrescente(const void* a, const void* b); 

/* -------- Variaveis globais  --------*/
    int *vetorPrincipal = NULL;
    int qntThreads = 0;
    int qntArquivos = 0;
    int indiceGlobal = 0;
    int qntValoresTotal = 0;
    char arqSaida[50];    
/* ---------------------------------- */

int main(int argc, char const *argv[])
{   
    struct timeval inicio, fim; //Variáveis para calcular o tempo de execução;
    int threads = atoi(argv[1]); // pegando o numero de threads
    double tempoLeitura, tempoThreads, tempoEscrita, tempoTotal;
    struct T_quickSort s_quick;

    /* verificando quantidade de parametros */
    if(argc < 3){
        printf("Quantidade de parametros invalida\n");
        return 0;
    }    

    qntThreads = threads; // Passando o numero de threads para uma var global        
    qntArquivos = argc - 3; // Pegando num. total de arquivos lidos
    T_arq arquivos[qntArquivos];

    printf("Quantidade de Threads: %d\n", qntThreads);
    printf("Quantidade de arquivos de entrada: %d\n", qntArquivos);

    strcpy(arqSaida, argv[argc-1]); //Pegando nome do arquivo de saida

    /* Copiando o nome do arquivo para a struct */    
    for(int i = 0; i < qntArquivos; i++){
        strcpy(arquivos[i].nome, argv[i+2]);
    }
    /* ---------- Alocação de vetores------------ */
    for(int i = 0; i < qntArquivos; i++){
        alocaVetor(&arquivos[i]);
    }
    for(int i = 0; i < qntArquivos; i++){
        qntValoresTotal += arquivos[i].qntValores;
    }
    alocaVetorPrincipal();
    /* ------------ Fim das Alocações ------------- */

    /* ---------- Leitura de Arquivos ---------------*/
    gettimeofday(&inicio, NULL); 
    for(int i = 0; i < qntArquivos; i++){
        carrega(arquivos[i]);
    }
            
    for(int i = 0; i < qntArquivos; i++){
        carregaVetorPrincipal(arquivos[i]);
    }
    gettimeofday(&fim, NULL);
    tempoLeitura = (double)(fim.tv_usec-inicio.tv_usec)/1000000+(double)(fim.tv_sec-inicio.tv_sec);
    /* ------------- Fim da Leitura -------------------*/    

    /* ------------------ Threads ---------------------*/
    gettimeofday(&inicio, NULL); 
    ordenaVetor(s_quick);
    gettimeofday(&fim, NULL);
    tempoThreads = (double)(fim.tv_usec-inicio.tv_usec)/1000000+(double)(fim.tv_sec-inicio.tv_sec);
    /* -----------------Fim das Threads----------------- */

    /* ------------------ Impressão ---------------------*/
    gettimeofday(&inicio, NULL);
    imprimeVetorPrincipal();
    gettimeofday(&fim, NULL);
    tempoEscrita = (double)(fim.tv_usec-inicio.tv_usec)/1000000+(double)(fim.tv_sec-inicio.tv_sec);
    /* ------------------ --------------------------------*/
    tempoTotal = tempoEscrita + tempoLeitura + tempoThreads;
    printf("\nTEMPO TOTAL DE PROCESSAMENTO: %fs\n", tempoThreads);
    printf("Tempo total de execução (Leitura, Processamento e Escrita): %fs\n", tempoTotal);
    pthread_exit(NULL);
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
    pthread_t th[qntThreads-1]; //vetor de threads
    printf("Foram criadas %d threads\n", qntThreads);   
    
    /* O vetor principal é dividido de acordo com o numero de threads, parq que cada thread
    fique responsavel por uma parte da ordenação. A cada ciclo, o numero de threads cai pela metade,
    e a ordenação é feita novamente. Segue desta forma até restar somente uma thread, que percorre
    ordenando o que ainda estiver desordenado */
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
            s_quick.left += tam;
            s_quick.right += tam;
            l++;
        }
        /* Fim do Ciclo */
        for(i = 0; i < th_id; i++){
            pthread_join(th[i], NULL);
        }
        qntThreads /= 2;
        l = 0;
   }
    /* Todas as threads já ordenaram parte do vetor
    agora, a thread 0 deve percorrer ordenando o que estiver desordenado */ 
    if(qntThreads < 2){
        s_quick.left = 0;
        s_quick.right = (indiceGlobal-1);
        pthread_create(&(th[0]), NULL, preparaQsort, (void *)&s_quick);
        pthread_join(th[0], NULL);
    }         
}
/* Função intermediaria entre as threads e o quicksort
que atribui as variaveis para o bom funcionamento da ordenação */
void * preparaQsort(void * s_quick){
    struct T_quickSort *quick = (struct T_quickSort *)s_quick;

    int left, right, tam;
    
    tam = (quick->right)-(quick->left);
    left = quick->left;
    right = quick->right;

    /* Função quicksort da biblioteca stdlib.h */
    qsort(vetorPrincipal, tam, sizeof(int), comparaCrescente);

    pthread_exit(NULL);
}

/* ---------------------------------- */
/* Função que define como será a comparação da função qsort() */
int comparaCrescente(const void* a, const void* b){
    if(*(int *)a == *(int *)b) return 0;
    else
        if(*(int*)a < *(int *)b) return -1;
        else
            return 1;
}
/* ---------------------------------- */