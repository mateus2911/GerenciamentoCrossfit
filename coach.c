#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "coach.h"

void inicializar_bd_coach(Coach bd[], int *tamanho) {
    *tamanho = 0;
}

void adicionar_coach(Coach bd[], int *tamanho, int id, const char *nome, const char *especialidade, const char disponibilidade[][MAX_HORARIO], int contador_disp) {
    if (*tamanho < MAX_COACHES && contador_disp <= MAX_DISPONIBILIDADE) {
        bd[*tamanho].id_coach = id;
        strncpy(bd[*tamanho].nome, nome, MAX_NOME - 1);
        bd[*tamanho].nome[MAX_NOME - 1] = '\0';
        strncpy(bd[*tamanho].especialidade, especialidade, MAX_ESPECIALIDADE - 1);
        bd[*tamanho].especialidade[MAX_ESPECIALIDADE - 1] = '\0';
        bd[*tamanho].contador_disponibilidade = contador_disp;
        for (int i = 0; i < contador_disp; i++) {
            strncpy(bd[*tamanho].disponibilidade[i], disponibilidade[i], MAX_HORARIO - 1);
            bd[*tamanho].disponibilidade[i][MAX_HORARIO - 1] = '\0';
        }
        (*tamanho)++;
    }
}

void exibir_bd_coach(Coach bd[], int tamanho) {
    printf("\nBanco de Dados de Coaches:\n");
    for (int i = 0; i < tamanho; i++) {
        printf("ID: %d, Nome: %s, Especialidade: %s, Disponibilidade: ", bd[i].id_coach, bd[i].nome, bd[i].especialidade);
        for (int j = 0; j < bd[i].contador_disponibilidade; j++) {
            printf("%s ", bd[i].disponibilidade[j]);
        }
        printf("\n");
    }
}

int coach_esta_disponivel(Coach *coach, const char *data_hora) {
    for (int i = 0; i < coach->contador_disponibilidade; i++) {
        if (strcmp(coach->disponibilidade[i], data_hora) == 0) {
            return 1;
        }
    }
    return 0;
}

void atualizar_disponibilidade_coach(Coach *coach, const char *data_hora, int remover) {
    if (remover) {
        for (int i = 0; i < coach->contador_disponibilidade; i++) {
            if (strcmp(coach->disponibilidade[i], data_hora) == 0) {
                for (int j = i; j < coach->contador_disponibilidade - 1; j++) {
                    strcpy(coach->disponibilidade[j], coach->disponibilidade[j + 1]);
                }
                coach->contador_disponibilidade--;
                break;
            }
        }
    } else {
        if (coach->contador_disponibilidade < MAX_DISPONIBILIDADE) {
            strcpy(coach->disponibilidade[coach->contador_disponibilidade], data_hora);
            coach->contador_disponibilidade++;
        }
    }
}