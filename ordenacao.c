#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ordenacao.h"

static void trocar_crossfit(CrossFit *a, CrossFit *b) {
    CrossFit temp = *a;
    *a = *b;
    *b = temp;
}

static int particionar_crossfit(CrossFit bd[], int baixo, int alto) {
    int pivo = bd[alto].id_crossfit;
    int i = baixo - 1;
    for (int j = baixo; j < alto; j++) {
        if (bd[j].id_crossfit <= pivo) {
            i++;
            trocar_crossfit(&bd[i], &bd[j]);
        }
    }
    trocar_crossfit(&bd[i + 1], &bd[alto]);
    return i + 1;
}

void ordenar_bd_crossfit(CrossFit bd[], int tamanho) {
    if (tamanho <= 1) return;
    int pilha[tamanho];
    int topo = -1;
    pilha[++topo] = 0;
    pilha[++topo] = tamanho - 1;
    while (topo >= 0) {
        int alto = pilha[topo--];
        int baixo = pilha[topo--];
        int pi = particionar_crossfit(bd, baixo, alto);
        if (pi - 1 > baixo) {
            pilha[++topo] = baixo;
            pilha[++topo] = pi - 1;
        }
        if (pi + 1 < alto) {
            pilha[++topo] = pi + 1;
            pilha[++topo] = alto;
        }
    }
}

void bubble_sort_crossfit_disco(CrossFit bd[], int *tamanho, const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) {
        printf("Erro ao abrir arquivo %s para escrita\n", nome_arquivo);
        return;
    }
    for (int i = 0; i < *tamanho; i++) {
        fprintf(arquivo, "%d|%s|%s\n", bd[i].id_crossfit, bd[i].nome, bd[i].endereco);
    }
    fclose(arquivo);

    CrossFit bd_temp[MAX_CROSSFIT];
    int tamanho_temp = 0;
    arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir arquivo %s para leitura\n", nome_arquivo);
        return;
    }
    char linha[256];
    while (fgets(linha, sizeof(linha), arquivo) && tamanho_temp < MAX_CROSSFIT) {
        sscanf(linha, "%d|%[^|]|%[^\n]", &bd_temp[tamanho_temp].id_crossfit,
               bd_temp[tamanho_temp].nome, bd_temp[tamanho_temp].endereco);
        tamanho_temp++;
    }
    fclose(arquivo);

    for (int i = 0; i < tamanho_temp - 1; i++) {
        for (int j = 0; j < tamanho_temp - i - 1; j++) {
            if (bd_temp[j].id_crossfit > bd_temp[j + 1].id_crossfit) {
                trocar_crossfit(&bd_temp[j], &bd_temp[j + 1]);
            }
        }
    }

    arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) {
        printf("Erro ao abrir arquivo %s para escrita\n", nome_arquivo);
        return;
    }
    for (int i = 0; i < tamanho_temp; i++) {
        fprintf(arquivo, "%d|%s|%s\n", bd_temp[i].id_crossfit, bd_temp[i].nome, bd_temp[i].endereco);
    }
    fclose(arquivo);

    *tamanho = tamanho_temp;
    for (int i = 0; i < *tamanho; i++) {
        bd[i] = bd_temp[i];
    }
}

static void trocar_coach(Coach *a, Coach *b) {
    Coach temp = *a;
    *a = *b;
    *b = temp;
}

static int particionar_coach(Coach bd[], int baixo, int alto) {
    int pivo = bd[alto].id_coach;
    int i = baixo - 1;
    for (int j = baixo; j < alto; j++) {
        if (bd[j].id_coach <= pivo) {
            i++;
            trocar_coach(&bd[i], &bd[j]);
        }
    }
    trocar_coach(&bd[i + 1], &bd[alto]);
    return i + 1;
}

void ordenar_bd_coach(Coach bd[], int tamanho) {
    if (tamanho <= 1) return;
    int pilha[tamanho];
    int topo = -1;
    pilha[++topo] = 0;
    pilha[++topo] = tamanho - 1;
    while (topo >= 0) {
        int alto = pilha[topo--];
        int baixo = pilha[topo--];
        int pi = particionar_coach(bd, baixo, alto);
        if (pi - 1 > baixo) {
            pilha[++topo] = baixo;
            pilha[++topo] = pi - 1;
        }
        if (pi + 1 < alto) {
            pilha[++topo] = pi + 1;
            pilha[++topo] = alto;
        }
    }
}

void bubble_sort_coach_disco(Coach bd[], int *tamanho, const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) {
        printf("Erro ao abrir arquivo %s para escrita\n", nome_arquivo);
        return;
    }
    for (int i = 0; i < *tamanho; i++) {
        fprintf(arquivo, "%d|%s|%s|%d", bd[i].id_coach, bd[i].nome, bd[i].especialidade, bd[i].contador_disponibilidade);
        for (int j = 0; j < bd[i].contador_disponibilidade; j++) {
            fprintf(arquivo, "|%s", bd[i].disponibilidade[j]);
        }
        fprintf(arquivo, "\n");
    }
    fclose(arquivo);

    Coach bd_temp[MAX_COACHES];
    int tamanho_temp = 0;
    arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir arquivo %s para leitura\n", nome_arquivo);
        return;
    }
    char linha[512];
    while (fgets(linha, sizeof(linha), arquivo) && tamanho_temp < MAX_COACHES) {
        char *token = strtok(linha, "|");
        bd_temp[tamanho_temp].id_coach = atoi(token);
        token = strtok(NULL, "|");
        strncpy(bd_temp[tamanho_temp].nome, token, MAX_NOME - 1);
        bd_temp[tamanho_temp].nome[MAX_NOME - 1] = '\0';
        token = strtok(NULL, "|");
        strncpy(bd_temp[tamanho_temp].especialidade, token, MAX_ESPECIALIDADE - 1);
        bd_temp[tamanho_temp].especialidade[MAX_ESPECIALIDADE - 1] = '\0';
        token = strtok(NULL, "|");
        bd_temp[tamanho_temp].contador_disponibilidade = atoi(token);
        for (int j = 0; j < bd_temp[tamanho_temp].contador_disponibilidade; j++) {
            token = strtok(NULL, "|");
            strncpy(bd_temp[tamanho_temp].disponibilidade[j], token, MAX_HORARIO - 1);
            bd_temp[tamanho_temp].disponibilidade[j][MAX_HORARIO - 1] = '\0';
        }
        tamanho_temp++;
    }
    fclose(arquivo);

    for (int i = 0; i < tamanho_temp - 1; i++) {
        for (int j = 0; j < tamanho_temp - i - 1; j++) {
            if (bd_temp[j].id_coach > bd_temp[j + 1].id_coach) {
                trocar_coach(&bd_temp[j], &bd_temp[j + 1]);
            }
        }
    }

    arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) {
        printf("Erro ao abrir arquivo %s para escrita\n", nome_arquivo);
        return;
    }
    for (int i = 0; i < tamanho_temp; i++) {
        fprintf(arquivo, "%d|%s|%s|%d", bd_temp[i].id_coach, bd_temp[i].nome, bd_temp[i].especialidade, bd_temp[i].contador_disponibilidade);
        for (int j = 0; j < bd_temp[i].contador_disponibilidade; j++) {
            fprintf(arquivo, "|%s", bd_temp[i].disponibilidade[j]);
        }
        fprintf(arquivo, "\n");
    }
    fclose(arquivo);

    *tamanho = tamanho_temp;
    for (int i = 0; i < *tamanho; i++) {
        bd[i] = bd_temp[i];
    }
}

static void trocar_aluno(Aluno *a, Aluno *b) {
    Aluno temp = *a;
    *a = *b;
    *b = temp;
}

static int particionar_aluno(Aluno bd[], int baixo, int alto) {
    int pivo = bd[alto].id_aluno;
    int i = baixo - 1;
    for (int j = baixo; j < alto; j++) {
        if (bd[j].id_aluno <= pivo) {
            i++;
            trocar_aluno(&bd[i], &bd[j]);
        }
    }
    trocar_aluno(&bd[i + 1], &bd[alto]);
    return i + 1;
}

void ordenar_bd_aluno(Aluno bd[], int tamanho) {
    if (tamanho <= 1) return;
    int pilha[tamanho];
    int topo = -1;
    pilha[++topo] = 0;
    pilha[++topo] = tamanho - 1;
    while (topo >= 0) {
        int alto = pilha[topo--];
        int baixo = pilha[topo--];
        int pi = particionar_aluno(bd, baixo, alto);
        if (pi - 1 > baixo) {
            pilha[++topo] = baixo;
            pilha[++topo] = pi - 1;
        }
        if (pi + 1 < alto) {
            pilha[++topo] = pi + 1;
            pilha[++topo] = alto;
        }
    }
}

void bubble_sort_aluno_disco(Aluno bd[], int *tamanho, const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) {
        printf("Erro ao abrir arquivo %s para escrita\n", nome_arquivo);
        return;
    }
    for (int i = 0; i < *tamanho; i++) {
        fprintf(arquivo, "%d|%s|%s|%d", bd[i].id_aluno, bd[i].nome, bd[i].nivel_experiencia, bd[i].contador_historico);
        for (int j = 0; j < bd[i].contador_historico; j++) {
            fprintf(arquivo, "|%s", bd[i].historico_treinos[j]);
        }
        fprintf(arquivo, "\n");
    }
    fclose(arquivo);

    Aluno bd_temp[MAX_ALUNOS];
    int tamanho_temp = 0;
    arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir arquivo %s para leitura\n", nome_arquivo);
        return;
    }
    char linha[512];
    while (fgets(linha, sizeof(linha), arquivo) && tamanho_temp < MAX_ALUNOS) {
        char *token = strtok(linha, "|");
        bd_temp[tamanho_temp].id_aluno = atoi(token);
        token = strtok(NULL, "|");
        strncpy(bd_temp[tamanho_temp].nome, token, MAX_NOME - 1);
        bd_temp[tamanho_temp].nome[MAX_NOME - 1] = '\0';
        token = strtok(NULL, "|");
        strncpy(bd_temp[tamanho_temp].nivel_experiencia, token, MAX_NIVEL - 1);
        bd_temp[tamanho_temp].nivel_experiencia[MAX_NIVEL - 1] = '\0';
        token = strtok(NULL, "|");
        bd_temp[tamanho_temp].contador_historico = atoi(token);
        for (int j = 0; j < bd_temp[tamanho_temp].contador_historico; j++) {
            token = strtok(NULL, "|");
            strncpy(bd_temp[tamanho_temp].historico_treinos[j], token, MAX_ENTRADA_HISTORICO - 1);
            bd_temp[tamanho_temp].historico_treinos[j][MAX_ENTRADA_HISTORICO - 1] = '\0';
        }
        tamanho_temp++;
    }
    fclose(arquivo);

    for (int i = 0; i < tamanho_temp - 1; i++) {
        for (int j = 0; j < tamanho_temp - i - 1; j++) {
            if (bd_temp[j].id_aluno > bd_temp[j + 1].id_aluno) {
                trocar_aluno(&bd_temp[j], &bd_temp[j + 1]);
            }
        }
    }

    arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) {
        printf("Erro ao abrir arquivo %s para escrita\n", nome_arquivo);
        return;
    }
    for (int i = 0; i < tamanho_temp; i++) {
        fprintf(arquivo, "%d|%s|%s|%d", bd_temp[i].id_aluno, bd_temp[i].nome, bd_temp[i].nivel_experiencia, bd_temp[i].contador_historico);
        for (int j = 0; j < bd_temp[i].contador_historico; j++) {
            fprintf(arquivo, "|%s", bd_temp[i].historico_treinos[j]);
        }
        fprintf(arquivo, "\n");
    }
    fclose(arquivo);

    *tamanho = tamanho_temp;
    for (int i = 0; i < *tamanho; i++) {
        bd[i] = bd_temp[i];
    }
}

static void trocar_treino(TreinoDiario *a, TreinoDiario *b) {
    TreinoDiario temp = *a;
    *a = *b;
    *b = temp;
}

static int particionar_treino(TreinoDiario bd[], int baixo, int alto) {
    int pivo = bd[alto].id;
    int i = baixo - 1;
    for (int j = baixo; j < alto; j++) {
        if (bd[j].id <= pivo) {
            i++;
            trocar_treino(&bd[i], &bd[j]);
        }
    }
    trocar_treino(&bd[i + 1], &bd[alto]);
    return i + 1;
}

void ordenar_bd_treino(TreinoDiario bd[], int tamanho) {
    if (tamanho <= 1) return;
    int pilha[tamanho];
    int topo = -1;
    pilha[++topo] = 0;
    pilha[++topo] = tamanho - 1;
    while (topo >= 0) {
        int alto = pilha[topo--];
        int baixo = pilha[topo--];
        int pi = particionar_treino(bd, baixo, alto);
        if (pi - 1 > baixo) {
            pilha[++topo] = baixo;
            pilha[++topo] = pi - 1;
        }
        if (pi + 1 < alto) {
            pilha[++topo] = pi + 1;
            pilha[++topo] = alto;
        }
    }
}

void bubble_sort_treino_disco(TreinoDiario bd[], int *tamanho, const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) {
        printf("Erro ao abrir arquivo %s para escrita\n", nome_arquivo);
        return;
    }
    for (int i = 0; i < *tamanho; i++) {
        fprintf(arquivo, "%d|%s|%s|%d|%d", bd[i].id, bd[i].data, bd[i].tipo_treino, bd[i].id_coach, bd[i].contador_alunos);
        for (int j = 0; j < bd[i].contador_alunos; j++) {
            fprintf(arquivo, "|%d", bd[i].alunos_inscritos[j]);
        }
        fprintf(arquivo, "\n");
    }
    fclose(arquivo);

    TreinoDiario bd_temp[MAX_TREINOS];
    int tamanho_temp = 0;
    arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir arquivo %s para leitura\n", nome_arquivo);
        return;
    }
    char linha[512];
    while (fgets(linha, sizeof(linha), arquivo) && tamanho_temp < MAX_TREINOS) {
        char *token = strtok(linha, "|");
        bd_temp[tamanho_temp].id = atoi(token);
        token = strtok(NULL, "|");
        strncpy(bd_temp[tamanho_temp].data, token, sizeof(bd_temp[tamanho_temp].data) - 1);
        bd_temp[tamanho_temp].data[sizeof(bd_temp[tamanho_temp].data) - 1] = '\0';
        token = strtok(NULL, "|");
        strncpy(bd_temp[tamanho_temp].tipo_treino, token, MAX_TIPO_TREINO - 1);
        bd_temp[tamanho_temp].tipo_treino[MAX_TIPO_TREINO - 1] = '\0';
        token = strtok(NULL, "|");
        bd_temp[tamanho_temp].id_coach = atoi(token);
        token = strtok(NULL, "|");
        bd_temp[tamanho_temp].contador_alunos = atoi(token);
        for (int j = 0; j < bd_temp[tamanho_temp].contador_alunos; j++) {
            token = strtok(NULL, "|");
            bd_temp[tamanho_temp].alunos_inscritos[j] = atoi(token);
        }
        tamanho_temp++;
    }
    fclose(arquivo);

    for (int i = 0; i < tamanho_temp - 1; i++) {
        for (int j = 0; j < tamanho_temp - i - 1; j++) {
            if (bd_temp[j].id > bd_temp[j + 1].id) {
                trocar_treino(&bd_temp[j], &bd_temp[j + 1]);
            }
        }
    }

    arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) {
        printf("Erro ao abrir arquivo %s para escrita\n", nome_arquivo);
        return;
    }
    for (int i = 0; i < tamanho_temp; i++) {
        fprintf(arquivo, "%d|%s|%s|%d|%d", bd_temp[i].id, bd_temp[i].data, bd_temp[i].tipo_treino, bd_temp[i].id_coach, bd_temp[i].contador_alunos);
        for (int j = 0; j < bd_temp[i].contador_alunos; j++) {
            fprintf(arquivo, "|%d", bd_temp[i].alunos_inscritos[j]);
        }
        fprintf(arquivo, "\n");
    }
    fclose(arquivo);

    *tamanho = tamanho_temp;
    for (int i = 0; i < *tamanho; i++) {
        bd[i] = bd_temp[i];
    }
}