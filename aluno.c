
//Camille Silva Oliveira 23.1.8120
//Mateus Rodrigues 19.2.8177

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
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

    printf("Digite o nivel do aluno (fitness, scale, rx): ");
    limpar_buffer_entrada();
    scanf("%[^\n]", novo_aluno.nivel);

    fwrite(&novo_aluno, sizeof(Aluno), 1, file);
    fclose(file);
    printf("\nAluno com ID %d cadastrado com sucesso!\n", novo_aluno.id);
}

// Implementa busca sequencial.
Aluno buscar_aluno_sequencial(int id) {
    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);
    FILE *file = fopen(ARQUIVO_ALUNOS, "rb");
    Aluno aluno;
    aluno.id = -1;

    if (file == NULL) return aluno;
    QueryPerformanceCounter(&start);
    while (fread(&aluno, sizeof(Aluno), 1, file)) {
        if (aluno.id == id && aluno.ativo) {
            QueryPerformanceCounter(&end);
            double tempo_sequencial = ((double)(end.QuadPart - start.QuadPart)) / frequency.QuadPart;
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
    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);
    FILE *file = fopen(ARQUIVO_ALUNOS, "rb");
    Aluno aluno;
    aluno.id = -1;
    if (file == NULL) return aluno;

    int inicio = 0;
    int fim = obter_total_alunos() - 1;
    QueryPerformanceCounter(&start);
    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        fseek(file, meio * sizeof(Aluno), SEEK_SET);
        fread(&aluno, sizeof(Aluno), 1, file);

        if (aluno.id == id && aluno.ativo) {
            QueryPerformanceCounter(&end);
            double tempo_binario = ((double)(end.QuadPart - start.QuadPart)) / frequency.QuadPart;
            printf("Tempo da busca binaria: %f segundos.\n", tempo_binario);
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

void consultar_alunos_por_nivel() {
    char nivel_busca[MAX_NIVEL];
    printf("\nDigite o nivel do aluno a ser buscado (fitness, scale, rx): ");
    limpar_buffer_entrada();
    scanf("%[^\n]", nivel_busca);

    FILE *file = fopen(ARQUIVO_ALUNOS, "rb");
    if (file == NULL) {
        printf("\nNenhum aluno cadastrado.\n");
        return;
    }

    Aluno aluno;
    int encontrados = 0;
    printf("\n--- Alunos encontrados para o nivel '%s' ---\n", nivel_busca);
    printf("%-5s | %-30s | %-15s\n", "ID", "Nome", "Nível");
    printf("-----------------------------------------------------\n");
    while (fread(&aluno, sizeof(Aluno), 1, file)) {
        if (aluno.ativo && strcmp(aluno.nivel, nivel_busca) == 0) {
            printf("%-5d | %-30s | %-15s\n", aluno.id, aluno.nome, aluno.nivel);
            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf("Nenhum aluno encontrado com o nivel especificado.\n");
    }
    printf("-----------------------------------------------------\n");
    fclose(file);
}

/**
 * @brief Calcula e exibe a frequência de cada aluno com base no histórico de performance.
 */
void consultar_alunos_por_frequencia() {
    FILE *f_alunos = fopen(ARQUIVO_ALUNOS, "rb");
    if (f_alunos == NULL) {
        printf("\nNenhum aluno cadastrado.\n");
        return;
    }

    printf("\n--- Frequencia de Alunos (baseado no historico de performance) ---\n");
    printf("%-5s | %-30s | %-10s\n", "ID", "Nome", "Treinos");
    printf("--------------------------------------------------------\n");

    Aluno aluno;
    while (fread(&aluno, sizeof(Aluno), 1, f_alunos)) {
        if (aluno.ativo) {
            FILE *f_perf = fopen(ARQUIVO_PERFORMANCE, "rb");
            if (f_perf == NULL) continue;

            Performance p;
            int contagem_treinos = 0;
            while (fread(&p, sizeof(Performance), 1, f_perf)) {
                if (p.id_aluno == aluno.id) {
                    contagem_treinos++;
                }
            }
            printf("%-5d | %-30s | %-10d\n", aluno.id, aluno.nome, contagem_treinos);
            fclose(f_perf);
        }
    }
    printf("--------------------------------------------------------\n");
    fclose(f_alunos);
}