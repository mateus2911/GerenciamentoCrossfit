#ifndef COACH_H
#define COACH_H

#define MAX_COACHES 50
#define MAX_NOME 50
#define MAX_ESPECIALIDADE 20
#define MAX_DISPONIBILIDADE 5
#define MAX_HORARIO 20

typedef struct {
    int id_coach;
    char nome[MAX_NOME];
    char especialidade[MAX_ESPECIALIDADE];
    char disponibilidade[MAX_DISPONIBILIDADE][MAX_HORARIO];
    int contador_disponibilidade;
} Coach;

void inicializar_bd_coach(Coach bd[], int *tamanho);
void adicionar_coach(Coach bd[], int *tamanho, int id, const char *nome, const char *especialidade, const char disponibilidade[][MAX_HORARIO], int contador_disp);
void exibir_bd_coach(Coach bd[], int tamanho);
int coach_esta_disponivel(Coach *coach, const char *data_hora);
void atualizar_disponibilidade_coach(Coach *coach, const char *data_hora, int remover);

#endif