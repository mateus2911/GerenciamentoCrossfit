
//Camille Silva Oliveira 23.1.8120

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aluno.h"
#include "utils.h"
#include <time.h>

int obter_total_alunos() {
    FILE *file = fopen(ARQUIVO_ALUNOS, "rb");
    if (file == NULL) return 0;
    fseek(file, 0, SEEK_END);
    int total = ftell(file) / sizeof(Aluno);
    fclose(file);
    return total;
}

// Implementa estrutura para Aluno e cria base de dados desordenada.
void cadastrar_aluno() {
    FILE *file = fopen(ARQUIVO_ALUNOS, "ab");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo de alunos");
        return;
    }

    Aluno novo_aluno;
    novo_aluno.id = obter_total_alunos();
    novo_aluno.ativo = 1;

    printf("Digite o nome do aluno: ");
    limpar_buffer_entrada();
    scanf("%[^\n]", novo_aluno.nome);

    printf("Digite o nível do aluno (fitness, scale, rx): ");
    limpar_buffer_entrada();
    scanf("%[^\n]", novo_aluno.nivel);

    fwrite(&novo_aluno, sizeof(Aluno), 1, file);
    fclose(file);
    printf("\nAluno com ID %d cadastrado com sucesso!\n", novo_aluno.id);
}

// Implementa busca sequencial.
Aluno buscar_aluno_sequencial(int id) {
    srand(time(NULL));
    FILE *file = fopen(ARQUIVO_ALUNOS, "rb");
    Aluno aluno;
    aluno.id = -1;
    clock_t fim_clock;

    if (file == NULL) return aluno;
    clock_t inicio_clock = clock();
    while (fread(&aluno, sizeof(Aluno), 1, file)) {
        if (aluno.id == id && aluno.ativo) {
            fim_clock = clock();
            double tempo_sequencial = ((double)(fim_clock - inicio_clock)) / CLOCKS_PER_SEC;
            printf("Tempo da busca sequencial: %f segundos.\n", tempo_sequencial);
            fclose(file);
            return aluno;
        }
    }
    fclose(file);
    aluno.id = -1;
    return aluno;
}

// Implementa busca binária.
Aluno buscar_aluno_binaria(int id) {
    srand(time(NULL));
    FILE *file = fopen(ARQUIVO_ALUNOS, "rb");
    Aluno aluno;
    aluno.id = -1;
    clock_t fim_clock;
    if (file == NULL) return aluno;

    int inicio = 0;
    int fim = obter_total_alunos() - 1;
    clock_t inicio_clock = clock();
    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        fseek(file, meio * sizeof(Aluno), SEEK_SET);
        fread(&aluno, sizeof(Aluno), 1, file);

        if (aluno.id == id && aluno.ativo) {
            fim_clock = clock();
            double tempo_binario = ((double)(fim_clock - inicio_clock)) / CLOCKS_PER_SEC;
            printf("Tempo da busca binária: %f segundos.\n", tempo_binario);
            fclose(file);
            return aluno;
        }
        if (aluno.id < id) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    fclose(file);
    aluno.id = -1;
    return aluno;
}

void listar_alunos() {
    FILE *file = fopen(ARQUIVO_ALUNOS, "rb");
    if (file == NULL) {
        printf("\nNenhum aluno cadastrado.\n");
        return;
    }

    Aluno aluno;
    printf("\n--- Lista de Alunos ---\n");
    printf("%-5s | %-30s | %-15s\n", "ID", "Nome", "Nível");
    printf("-----------------------------------------------------\n");
    while (fread(&aluno, sizeof(Aluno), 1, file)) {
        if (aluno.ativo) {
            printf("%-5d | %-30s | %-15s\n", aluno.id, aluno.nome, aluno.nivel);
        }
    }
    printf("-----------------------------------------------------\n");
    fclose(file);
}