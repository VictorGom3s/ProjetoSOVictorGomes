#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct arquivo{
    char nome[50];
    int pos;
    int *vetor;
}T_arq;

void* carrega(T_arq arquivos);
void* QuickSort();
void* imprimeSaida();
void* alocaVetor(T_arq arquivos);

/* Variaveis globais */
    int *vetorPrincipal;
    int qntArquivos = 0;
    int qntValores = 0;

//     0       1      2       3       4         5
//  ./multicat 16 arq1.in arq2.in arq3.in arqSaida.out
int main(int argc, char const *argv[])
{   
    /* verificando quantidade de parametros */
    if(argc < 3){
        printf("Quantidade de parametros invalida\n");
        return 0;
    }
    
    int *vetor;
    int threads = atoi(argv[1]);
    qntArquivos = argc - 2;
    T_arq arquivos[qntArquivos];

    printf("qntArquivos: %d\n", qntArquivos);
    /* Copiando o nome do arquivo para a struct */    
    for(int i = 0; i < qntArquivos; i++){
        strcpy(arquivos[i].nome, argv[i+2]);
    }
    printf("%s\n", arquivos[0].nome);
    printf("1\n");
    alocaVetor(arquivos[0]);
    printf("2\n");
    carrega(arquivos[0]);


    return 0;
}

void* alocaVetor(T_arq arquivos){
    printf("3\n");
    FILE* fp = fopen(arquivos.nome, "r");
    if(fp == NULL){
        printf("Arquivo nao encontrado!\n");
        return NULL;
    }
    printf("4\n");
    printf("arquivo lido\n");
    while(fgets(NULL, 10000, fp)){ // <--- ERRO
        qntValores++;
    }
    printf("5\n");
    printf("qntValores: %d", qntValores);
    arquivos.vetor = (int *)calloc(qntValores, sizeof(int));

    pthread_exit(NULL);


    fclose(fp);
    pthread_exit(NULL);
}


void* carrega(T_arq arquivos){
    int i = 0;
    printf("6\n");
    FILE* fp = fopen(arquivos.nome, "r");
    if(fp == NULL){
        printf("Arquivo nao encontrado!\n");
        return NULL;
    }
    printf("7\n");
    while(fscanf(fp, "%d", &arquivos.vetor[i]) != EOF){
        i++;
        printf(" %d ", arquivos.vetor[i]);
    }
    
    pthread_exit(NULL);
}
