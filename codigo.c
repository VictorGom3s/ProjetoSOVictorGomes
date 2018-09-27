#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/* Funções do programa */
float **alocarMatriz(int linhas, int colunas);
int carrega(float **matriz, char nome_arq[]);
int localiza(float **matriz, int valor);
int imprimeSaida(float **matriz, int valor);

/* Principal */
int main(){
    int linhas, colunas, threads, valor, erro;
    char nome_arq[50];
    float **matriz;

    printf("\nDigite o nome do arquivo de dados: ");
    fgets(nome_arq, 50, stdin);

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

