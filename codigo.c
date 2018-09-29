#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/* Funções do programa */
float **alocarMatriz(int linhas, int colunas);
int carrega(float **matriz, char nome_arq[], int linhas, int colunas);
int localizaNaMatriz(float **matriz, float valor, int linhas, int colunas);
int imprimeMatriz(float **matriz, int linhas, int colunas);

/* Principal */
int main(){
    int linhas, colunas, threads, erro;
    char nome_arq[50];
    float **matriz, valor = 0;    

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
    scanf("%f", &valor);

    /* Chamando funcao que aloca matriz e verificando se a alocacao
    foi bem sucedida */
    matriz = alocarMatriz(linhas, colunas);
    if(matriz == NULL){
        printf("\nERRO: nao foi possivel alocar!\n");
    }

    /* funcao que carrega dados do arquivo para a matriz */
    carrega(matriz, nome_arq, linhas, colunas);

    /* imprimindo a matriz 
    imprimeMatriz(matriz, linhas, colunas);*/

    localizaNaMatriz(matriz, valor, linhas, colunas);


    free(matriz);
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

int carrega(float **matriz, char nome_arq[], int linhas, int colunas){

    /* Abrindo arquivo para leitura */
    FILE* fp = fopen(nome_arq,"r");    
    if(fp == NULL){
        printf("\nERRO: Arquivo não encontrado\n");
        return 1;
    }

    /* Percorrendo os dados no arquivo e jogando na matriz */
    for(int i = 0; i < linhas; i++){
        for(int k = 0; k < colunas; k++){
            fscanf(fp ,"%f", &matriz[i][k]);
        }
        fscanf(fp ,"\n");
    }

    fclose(fp);

    return 0;
}

/* ------------------------------------------------------------------------- */

int imprimeMatriz(float **matriz, int linhas, int colunas){

    for(int i = 0; i < linhas; i++){
        printf("\n");
        for(int k = 0; k < colunas; k++){
            printf("%.2f ", matriz[i][k]);
        }
    }
    printf("\n");
}

/* ------------------------------------------------------------------------- */

int localizaNaMatriz(float **matriz, float valor, int linhas, int colunas){
    /* variavel auxiliar para contar se o valor foi encontrado alguma vez ou não */
    int aux = 0, k, i;

    /* Percorre a matriz comparando com a variavel valor */
    for(i = 0; i < linhas; i++){
        for(k = 0; k < colunas; k++){
            if(matriz[i][k] == valor){
                printf("\nVALOR ENCONTRADO\nPosicao na matriz ==> linha %d, coluna %d\n", i, k);
                aux++;
            }
        }
    }

    /* verifica se o valor foi encontrado pelo menos uma vez
     * caso não, mostra mensagem informando que o valor
     * não existe na matriz */
    if(aux == 0){
        printf("\nValor não foi encontrado na matriz!\n");
    }

    return 0;
}
