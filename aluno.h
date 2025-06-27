#ifndef ALUNO_H
#define ALUNO_H

#define MAX_ALUNOS 100
#define MAX_NOME 50
#define MAX_NIVEL 10
#define MAX_HISTORICO 50
#define MAX_ENTRADA_HISTORICO 50

typedef struct {
    int id_aluno;
    char nome[MAX_NOME];
    char nivel_experiencia[MAX_NIVEL];
    char historico_treinos[MAX_HISTORICO][MAX_ENTRADA_HISTORICO];
    int contador_historico;
    int treino;
} Aluno;

void inicializar_bd_aluno(Aluno bd[], int *tamanho);
void adicionar_aluno(Aluno bd[], int *tamanho, int id, const char *nome, const char *nivel, const char historico[][MAX_ENTRADA_HISTORICO], int contador_hist);
void exibir_bd_aluno(Aluno bd[], int tamanho);
void registrar_progresso_aluno(Aluno *aluno, int id_treino, const char *desempenho);

#endif