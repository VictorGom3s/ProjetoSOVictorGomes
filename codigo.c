#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/* Funções do programa */
float **alocarMatriz();
void* carrega();
void* localizaNaMatriz();
int imprimeMatriz();

/*Variaveis globais */
    int linhas, colunas, threads, q, erro;
    char nome_arq[50];
    float **matriz, valor = 0;

/* Principal */
int main(){
    int i, k;
    do{  

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

    pthread_t th[threads];
    
    /* Chamando funcao que aloca matriz e verificando se a alocacao
    foi bem sucedida */
    matriz = alocarMatriz();
    if(matriz == NULL){
       printf("\nERRO: nao foi possivel alocar!\n");
    }
    
    /* funcao que carrega dados do arquivo para a matriz */
   /* for(i = 0; i < 1; i++){
        pthread_create(&(th[i]), NULL, carrega, NULL);
        pthread_join(th[i], NULL);
    }*/
    carrega();

    /* Threads */
    for(k = 0; k < threads; k++){
        pthread_create(&(th[k]), NULL, localizaNaMatriz, NULL);
        pthread_join(th[k], NULL);
    }

    /* ----------------------------------------------- */

    printf("\nDigite 1 para buscar novamente, e 0 para sair!\n");
    scanf("%d", &q);
    }
    while(q == 1);

    //free(matriz);

    return 0;
}

/* ------------------------------------------------------------------------- */

float **alocarMatriz(){

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

void* carrega(){

    /* Abrindo arquivo para leitura */
    FILE* fp = fopen(nome_arq,"r");    
    if(fp == NULL){
        printf("\nERRO: Arquivo não encontrado\n");
        //return 1;
    }

    /* Percorrendo os dados no arquivo e jogando na matriz */
    for(int i = 0; i < linhas; i++){
        for(int k = 0; k < colunas; k++){
            fscanf(fp ,"%f", &matriz[i][k]);
        }
        fscanf(fp ,"\n");
    }

    fclose(fp);

    //return 0;
}

/* ------------------------------------------------------------------------- */

int imprimeMatriz(){

    for(int i = 0; i < linhas; i++){
        printf("\n");
        for(int k = 0; k < colunas; k++){
            printf("%.2f ", matriz[i][k]);
        }
    }
    printf("\n");
}

/* ------------------------------------------------------------------------- */

void* localizaNaMatriz(){
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

    //return 0;
}
