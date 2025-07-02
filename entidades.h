
//Camille Silva Oliveira 23.1.8120

#ifndef ENTIDADES_H
#define ENTIDADES_H

#include <stdio.h>

// --- Constantes de tamanho ---
#define MAX_NOME 50
#define MAX_ENDERECO 100
#define MAX_ESPECIALIDADE 30
#define MAX_NIVEL 15
#define MAX_TIPO_TREINO 20
#define MAX_ALUNOS_TREINO 15
#define MAX_RESULTADO 100

// --- Nomes dos arquivos de banco de dados ---
#define ARQUIVO_CROSSFIT "crossfit.bin" // Novo arquivo
#define ARQUIVO_ALUNOS "alunos.bin"
#define ARQUIVO_COACHES "coaches.bin"
#define ARQUIVO_TREINOS "treinos.bin"
#define ARQUIVO_PERFORMANCE "performance.bin"

// --- Estruturas de Entidades ---

/**
 * @brief Representa a academia como unidade central.
 * @param id Identificador único da unidade.
 * @param nome Nome da academia de CrossFit.
 * @param endereco Endereço físico da academia.
 */
typedef struct {
    int id;
    char nome[MAX_NOME];
    char endereco[MAX_ENDERECO];
} CrossFit;

/**
 * @brief Representa os frequentadores da academia que participam dos treinos.
 */
typedef struct {
    int id;
    char nome[MAX_NOME];
    char nivel[MAX_NIVEL];
    int ativo;
} Aluno;

/**
 * @brief Representa os profissionais responsáveis por conduzir os treinos.
 */
typedef struct {
    int id;
    char nome[MAX_NOME];
    char especialidade[MAX_ESPECIALIDADE];
    int ativo;
} Coach;

/**
 * @brief Representa os planos de treino diários.
 */
typedef struct {
    int id;
    char data[11]; // Formato DD/MM/AAAA
    char tipo_treino[MAX_TIPO_TREINO];
    int id_coach_responsavel;
    int lista_alunos_inscritos[MAX_ALUNOS_TREINO];
    int num_inscritos;
} Treino;

/**
 * @brief Representa um registro de desempenho para acompanhamento de progresso.
 */
typedef struct {
    int id_aluno;
    int id_treino;
    char resultado[MAX_RESULTADO];
} Performance;

#endif // ENTIDADES_H