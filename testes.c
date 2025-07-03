//Camille Silva Oliveira 23.1.8120

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "testes.h"
#include "entidades.h"
#include "aluno.h"
#include "coach.h"
#include "treino.h"
#include "ordenacao.h"

// Função auxiliar para embaralhar um array de inteiros (algoritmo Fisher-Yates)
void embaralhar_ids(int *array, int n) {
    if (n > 1) {
        for (int i = n - 1; i > 0; i--) {
            // Escolhe um índice aleatório de 0 a i
            int j = rand() % (i + 1);
            // Troca o elemento de i com o elemento do índice aleatório j
            int temp = array[i];
            array[i] = array[j];
            array[j] = temp;
        }
    }
}

// Função modificada para gerar uma base de dados de alunos genuinamente DESORDENADA por ID.
void gerar_alunos_aleatorios(int quantidade) {
    // "wb" cria um arquivo novo, apagando qualquer um que já exista.
    FILE *file = fopen(ARQUIVO_ALUNOS, "wb");
    if (file == NULL) {
        perror("Falha ao criar arquivo de alunos para teste");
        return;
    }

    // 1. Cria um array em memória com todos os IDs em ordem sequencial.
    int *ids = malloc(quantidade * sizeof(int));
    if (ids == NULL) {
        perror("Erro de alocacao de memoria para IDs");
        fclose(file);
        return;
    }
    for (int i = 0; i < quantidade; i++) {
        ids[i] = i;
    }

    // 2. Embaralha o array de IDs.
    embaralhar_ids(ids, quantidade);

    const char* nomes[] = {"Ana", "Bruno", "Carlos", "Daniela", "Eduardo", "Fernanda", "Gabriel", "Helena", "Igor", "Julia"};
    const char* sobrenomes[] = {"Silva", "Souza", "Oliveira", "Pereira", "Costa", "Rodrigues", "Martins", "Almeida", "Barros", "Ribeiro"};
    const char* niveis[] = {"fitness", "scale", "rx"};

    int num_nomes = sizeof(nomes) / sizeof(char*);
    int num_sobrenomes = sizeof(sobrenomes) / sizeof(char*);
    int num_niveis = sizeof(niveis) / sizeof(char*);

    printf("Gerando %d registros de alunos de forma desordenada... ", quantidade);

    // 3. Escreve os alunos no arquivo usando os IDs embaralhados.
    for (int i = 0; i < quantidade; i++) {
        Aluno aluno;
        aluno.id = ids[i]; // Pega o ID da lista embaralhada
        aluno.ativo = 1;
        sprintf(aluno.nome, "%s %s", nomes[rand() % num_nomes], sobrenomes[rand() % num_sobrenomes]);
        strcpy(aluno.nivel, niveis[rand() % num_niveis]);
        fwrite(&aluno, sizeof(Aluno), 1, file);
    }

    free(ids); // Libera a memoria do array de IDs
    fclose(file);
    printf("Concluido.\n");
}

void gerar_coaches_aleatorios(int quantidade) {
    FILE* file = fopen(ARQUIVO_COACHES, "wb");
    if (file == NULL) {
        perror("Falha ao criar arquivo de coaches para teste");
        return;
    }

    const char* nomes[] = {"Andre", "Beatriz", "Caio", "Debora", "Elton", "Fabiana"};
    const char* sobrenomes[] = {"Mendes", "Nunes", "Lima", "Gomes", "Ferreira", "Araujo"};
    const char* especialidades[] = {"Força", "LPO", "Ginástica", "Cardio", "Mobilidade"};

    int num_nomes = sizeof(nomes) / sizeof(char*);
    int num_sobrenomes = sizeof(sobrenomes) / sizeof(char*);
    int num_especialidades = sizeof(especialidades) / sizeof(char*);

    for (int i = 0; i < quantidade; i++) {
        Coach coach;
        coach.id = i;
        coach.ativo = 1;
        sprintf(coach.nome, "%s %s", nomes[rand() % num_nomes], sobrenomes[rand() % num_sobrenomes]);
        strcpy(coach.especialidade, especialidades[rand() % num_especialidades]);
        fwrite(&coach, sizeof(Coach), 1, file);
    }
    fclose(file);
}

void gerar_treinos_aleatorios(int quantidade, int total_alunos, int total_coaches) {
    if (total_alunos == 0 || total_coaches == 0) return;
    FILE* file = fopen(ARQUIVO_TREINOS, "wb");
    if (file == NULL) {
        perror("Falha ao criar arquivo de treinos para teste");
        return;
    }
    
    const char* tipos_treino[] = {"WOD", "AMRAP 15min", "For Time", "EMOM 20min", "Strength Day"};
    int num_tipos = sizeof(tipos_treino) / sizeof(char*);

    for (int i = 0; i < quantidade; i++) {
        Treino treino;
        treino.id = i;
        sprintf(treino.data, "%02d/%02d/2025", (rand() % 28) + 1, (rand() % 12) + 1);
        strcpy(treino.tipo_treino, tipos_treino[rand() % num_tipos]);
        treino.id_coach_responsavel = rand() % total_coaches;
        treino.num_inscritos = rand() % (MAX_ALUNOS_TREINO + 1);
        for(int j=0; j < treino.num_inscritos; j++){
            treino.lista_alunos_inscritos[j] = rand() % total_alunos;
        }
        fwrite(&treino, sizeof(Treino), 1, file);
    }
    fclose(file);
}