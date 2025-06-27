#ifndef CROSSFIT_H
#define CROSSFIT_H

#define MAX_CROSSFIT 10
#define MAX_NOME 50
#define MAX_ENDERECO 100

typedef struct {
    int id_crossfit;
    char nome[MAX_NOME];
    char endereco[MAX_ENDERECO];
    FILE *alunos;
    FILE *coaches;
    FILE *treinos;
} CrossFit;

void inicializar_bd_crossfit(CrossFit bd[], int *tamanho);
void adicionar_crossfit(CrossFit bd[], int *tamanho, int id, const char *nome, const char *endereco);
void exibir_bd_crossfit(CrossFit bd[], int tamanho);
void adicionar_treino_crossfit(CrossFit *crossfit);

#endif