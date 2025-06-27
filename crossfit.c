#include <stdio.h>
#include <string.h>
#include "crossfit.h"

void inicializar_bd_crossfit(CrossFit bd[], int *tamanho) {
    *tamanho = 0;
}

void adicionar_crossfit(CrossFit bd[], int *tamanho, int id, const char *nome, const char *endereco) {
    if (*tamanho < MAX_CROSSFIT) {
        bd[*tamanho].id_crossfit = id;
        strncpy(bd[*tamanho].nome, nome, MAX_NOME - 1);
        bd[*tamanho].nome[MAX_NOME - 1] = '\0';
        strncpy(bd[*tamanho].endereco, endereco, MAX_ENDERECO - 1);
        bd[*tamanho].endereco[MAX_ENDERECO - 1] = '\0';
        (*tamanho)++;
    }
}

void exibir_bd_crossfit(CrossFit bd[], int tamanho) {
    printf("Banco de Dados CrossFit:\n");
    for (int i = 0; i < tamanho; i++) {
        printf("ID: %d, Nome: %s, Endereço: %s\n", bd[i].id_crossfit, bd[i].nome, bd[i].endereco);
    }
}