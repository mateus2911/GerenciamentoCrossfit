#include <stdio.h>
#include <string.h>
#include "treino.h"
#include "coach.h"
#include "aluno.h"
#include "buscabinaria.h"

void inicializar_bd_treino(TreinoDiario bd[], int *tamanho) {
    *tamanho = 0;
}

void exibir_bd_treino(TreinoDiario bd[], int tamanho) {
    printf("\nBanco de Dados de Treinos:\n");
    for (int i = 0; i < tamanho; i++) {
        printf("ID: %d, Data: %s, Tipo: %s, ID Coach: %d, Alunos: ", bd[i].id, bd[i].data, bd[i].tipo_treino, bd[i].id_coach);
        for (int j = 0; j < bd[i].contador_alunos; j++) {
            printf("%d ", bd[i].alunos_inscritos[j]);
        }
        printf("\n");
    }
}

void planejar_treino(TreinoDiario bd_treino[], int *tamanho_treino, int id, const char *data, const char *tipo_treino, Coach bd_coach[], int tamanho_coach, int novos_alunos[], int num_novos_alunos, int max_alunos) {
    if (*tamanho_treino >= MAX_TREINOS) {
        printf("Não é possível adicionar mais treinos.\n");
        return;
    }

    int id_coach_atribuido = -1;
    for (int i = 0; i < tamanho_coach; i++) {
        if (strcmp(bd_coach[i].especialidade, tipo_treino) == 0 && coach_esta_disponivel(&bd_coach[i], data)) {
            id_coach_atribuido = bd_coach[i].id_coach;
            atualizar_disponibilidade_coach(&bd_coach[i], data, 1);
            break;
        }
    }

    if (id_coach_atribuido == -1) {
        printf("Nenhum coach disponível para %s na data %s.\n", tipo_treino, data);
        return;
    }

    TreinoDiario *novo_treino = &bd_treino[*tamanho_treino];
    novo_treino->id = id;
    strncpy(novo_treino->data, data, sizeof(novo_treino->data) - 1);
    novo_treino->data[sizeof(novo_treino->data) - 1] = '\0';
    strncpy(novo_treino->tipo_treino, tipo_treino, MAX_TIPO_TREINO - 1);
    novo_treino->tipo_treino[MAX_TIPO_TREINO - 1] = '\0';
    novo_treino->id_coach = id_coach_atribuido;
    novo_treino->contador_alunos = 0;

    for (int i = 0; i < num_novos_alunos && novo_treino->contador_alunos < max_alunos; i++) {
        novo_treino->alunos_inscritos[novo_treino->contador_alunos++] = novos_alunos[i];
    }

    (*tamanho_treino)++;
    printf("Treino de %s planejado para %s com o Coach ID %d.\n", tipo_treino, data, id_coach_atribuido);
}

void buscar_treinos_por_data_e_tipo(TreinoDiario bd_treino[], int tamanho_treino, Coach bd_coach[], int tamanho_coach, Aluno bd_aluno[], int tamanho_aluno, const char *data, const char *tipo) {
     printf("Buscando treinos para data (prefixo): %s e tipo: %s\n", data, tipo);
    for (int i = 0; i < tamanho_treino; i++) {
        if (strncmp(bd_treino[i].data, data, strlen(data)) == 0 && strcmp(bd_treino[i].tipo_treino, tipo) == 0) {
            Coach *c = busca_sequencial_coach(bd_coach, tamanho_coach, bd_treino[i].id_coach);
            printf("\nTreino ID: %d, Data: %s, Tipo: %s, Coach: %s\n", bd_treino[i].id, bd_treino[i].data, bd_treino[i].tipo_treino, c ? c->nome : "N/A");
            printf("Alunos Inscritos:\n");
            for (int j = 0; j < bd_treino[i].contador_alunos; j++) {
                Aluno *a = busca_sequencial_aluno(bd_aluno, tamanho_aluno, bd_treino[i].alunos_inscritos[j]);
                printf(" - %s (ID: %d)\n", a ? a->nome : "N/A", bd_treino[i].alunos_inscritos[j]);
            }
        }
    }
}


void realocar_coach(TreinoDiario *treino, Coach bd_coach[], int tamanho_coach, const char *data_hora, const char *especialidade) {
    if (!treino) return;

    Coach *coach_antigo = busca_sequencial_coach(bd_coach, tamanho_coach, treino->id_coach);
    if (coach_antigo) {
        atualizar_disponibilidade_coach(coach_antigo, data_hora, 0); 
    }

    int novo_id_coach = -1;
    for (int i = 0; i < tamanho_coach; i++) {
        if (bd_coach[i].id_coach != treino->id_coach && strcmp(bd_coach[i].especialidade, especialidade) == 0 && coach_esta_disponivel(&bd_coach[i], data_hora)) {
            novo_id_coach = bd_coach[i].id_coach;
            atualizar_disponibilidade_coach(&bd_coach[i], data_hora, 1);
            break;
        }
    }

    if (novo_id_coach != -1) {
        printf("Coach %s (ID: %d) realocado para Coach (ID: %d) para o treino ID %d.\n", coach_antigo ? coach_antigo->nome : "N/A", treino->id_coach, novo_id_coach, treino->id);
        treino->id_coach = novo_id_coach;
    } else {
        printf("Não foi possível realocar um novo coach para o treino ID %d.\n", treino->id);
         if (coach_antigo) {
            atualizar_disponibilidade_coach(coach_antigo, data_hora, 1);
        }
    }
}