#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "aluno.h"

void inicializar_bd_aluno(Aluno bd[], int *tamanho) {
    *tamanho = 0;
}

void adicionar_aluno(Aluno bd[], int *tamanho, int id, const char *nome, const char *nivel, const char historico[][MAX_ENTRADA_HISTORICO], int contador_hist) {
    if (*tamanho < MAX_ALUNOS && contador_hist <= MAX_HISTORICO) {
        bd[*tamanho].id_aluno = id;
        strncpy(bd[*tamanho].nome, nome, MAX_NOME - 1);
        bd[*tamanho].nome[MAX_NOME - 1] = '\0';
        strncpy(bd[*tamanho].nivel_experiencia, nivel, MAX_NIVEL - 1);
        bd[*tamanho].nivel_experiencia[MAX_NIVEL - 1] = '\0';
        bd[*tamanho].contador_historico = contador_hist;
        for (int i = 0; i < contador_hist; i++) {
            strncpy(bd[*tamanho].historico_treinos[i], historico[i], MAX_ENTRADA_HISTORICO - 1);
            bd[*tamanho].historico_treinos[i][MAX_ENTRADA_HISTORICO - 1] = '\0';
        }
        (*tamanho)++;
    }
}

void exibir_bd_aluno(Aluno bd[], int tamanho) {
    printf("\nBanco de Dados de Alunos:\n");
    for (int i = 0; i < tamanho; i++) {
        printf("ID: %d, Nome: %s, Nível: %s, Histórico: ", bd[i].id_aluno, bd[i].nome, bd[i].nivel_experiencia);
        for (int j = 0; j < bd[i].contador_historico; j++) {
            printf("%s ", bd[i].historico_treinos[j]);
        }
        printf("\n");
    }
}

void registrar_progresso_aluno(Aluno *aluno, int id_treino, const char *desempenho) {
    if (aluno->contador_historico < MAX_HISTORICO) {
        char entrada[MAX_ENTRADA_HISTORICO];
        snprintf(entrada, MAX_ENTRADA_HISTORICO, "Treino_%d: %s", id_treino, desempenho);
        strncpy(aluno->historico_treinos[aluno->contador_historico], entrada, MAX_ENTRADA_HISTORICO - 1);
        aluno->historico_treinos[aluno->contador_historico][MAX_ENTRADA_HISTORICO - 1] = '\0';
        aluno->contador_historico++;
    }
}