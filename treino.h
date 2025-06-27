#ifndef TREINO_H
#define TREINO_H

#define MAX_TREINOS 100
#define MAX_TIPO_TREINO 50
#define MAX_ALUNOS_TREINO 20

typedef struct {
    int id;
    char data[20];
    char tipo_treino[MAX_TIPO_TREINO];
    int id_coach;
    int alunos_inscritos[MAX_ALUNOS_TREINO];
    int contador_alunos;
} TreinoDiario;

void inicializar_bd_treino(TreinoDiario bd[], int *tamanho);
void exibir_bd_treino(TreinoDiario bd[], int tamanho);
void planejar_treino(TreinoDiario bd_treino[], int *tamanho_treino, int id, const char *data, const char *tipo_treino, Coach bd_coach[], int tamanho_coach, int novos_alunos[], int num_novos_alunos, int max_alunos);
void buscar_treinos_por_data_e_tipo(TreinoDiario bd_treino[], int tamanho_treino, Coach bd_coach[], int tamanho_coach, Aluno bd_aluno[], int tamanho_aluno, const char *data, const char *tipo);
void realocar_coach(TreinoDiario *treino, Coach bd_coach[], int tamanho_coach, const char *data_hora, const char *especialidade);

#endif