#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int configuraMatriz(char nome_arq, int t){

    int m, n, v;

    printf("\nDigite o numero de linhas da matriz: ");
    scanf("%d", &m);

    printf("\nDigite o numero de colunas da matriz: ");
    scanf("%d", &n);

    printf("\nDigite o valor a ser buscado na matriz: ");
    scanf("%d", &v);

    int matriz[m][n];

    return matriz;
}

int main(){
    int m, n, t, v, matriz;

    char nome_arq[50];

    printf("\nDigite o nome do arquivo input: ");
    scanf("%s", nome_arq);

    printf("\nDigite a quantidade de Threads a serem utilizadas: ");
    scanf("%d", &t);

    matriz = configuraMatriz(nome_arq, t);

    FILE *fp = fopen(nome_arq, "r");
    
    if (fp == NULL) {
        printf("Arquivo não encontrado!");
    }

    

    //for(i = 0; i < EOF; i++);



    return 0;
}