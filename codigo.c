#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/* Funções do programa */
float **alocarMatriz(int linhas, int colunas);
int carrega(float **matriz, char nome_arq[]);
int localizaNaMatriz(float **matriz, int valor);
int imprimeMatriz(float **matriz, int linhas, int colunas);

/* Principal */
int main(){
    int linhas, colunas, threads, valor, erro;
    char nome_arq[50];
    float **matriz;    

    /* pegando dados do usuario */
    printf("\nDigite o nome do arquivo de dados: ");
    scanf("%s", nome_arq);

    printf("\nDigite a quantidade de Threads a serem utilizadas: ");
    scanf("%d", &threads);

    printf("\nDigite o numero de linhas da matriz: ");
    scanf("%d", &linhas);

    printf("\nDigite o numero de colunas da matriz: ");
    scanf("%d", &colunas);

    printf("\nDigite o valor a ser buscado na matriz: ");
    scanf("%d", &valor);

    /* Chamando funcao que aloca matriz e verificando se a alocacao
    foi bem sucedida */
    matriz = alocarMatriz(linhas, colunas);
    if(matriz == NULL){
        printf("\nERRO: nao foi possivel alocar!\n");
    }

    /* funcao que carrega dados do arquivo para a matriz */
    carrega(matriz, nome_arq);

    /* imprimindo a matriz */
    imprimeMatriz(matriz, linhas, colunas);

    
    return 0;
}

/* ------------------------------------------------------------------------- */

float **alocarMatriz(int linhas, int colunas){

    float **matriz;
    /* alocando linhas */
    matriz = (float **)calloc(linhas, sizeof(float *));

    /* tratamento de erros */
    if (matriz == NULL) {
        printf("\nERRO: Memoria insuficiente para alocar a matriz!\n");
        return NULL;
    }
    
    /* alocando colunas */
    for(int i = 0; i < linhas; i++){
        matriz[i] = (float *)calloc(colunas, sizeof(float));
        if(matriz[i] == NULL){
            printf("\nERRO: Memoria insuficiente para alocar a matriz!\n");
            return NULL;
        }
    }
    return matriz;
}
/* ------------------------------------------------------------------------- */

int carrega(float **matriz, char nome_arq[]){
    int linhas, colunas;

    /* Abrindo arquivo para leitura */
    FILE* fp = fopen(nome_arq,"r");    
    if(fp == NULL){
        printf("\nERRO: Arquivo não encontrado\n");
        return 1;
    }

    fscanf(fp, "%d %d", &linhas, &colunas);
    printf("\n%d %d\n", linhas, colunas);

    for(int i = 0; i < linhas; i++){
        for(int k = 0; k < colunas; k++){
            fscanf(fp ,"%f", &matriz[i][k]);
        }
    }

    fclose(fp);

    return 0;
}


int imprimeMatriz(float **matriz, int linhas, int colunas){

    for(int i = 0; i < linhas; i++){
        printf("\n");
        for(int k = 0; k < colunas; k++){
            printf("%.2f ", matriz[i][k]);
        }
    }
}
