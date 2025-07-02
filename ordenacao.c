
//Camille Silva Oliveira 23.1.8120

#include <stdio.h>
#include <stdlib.h>
#include "ordenacao.h"
#include "aluno.h"

// Função de comparação para o qsort, baseada no ID do aluno
int comparar_alunos(const void *a, const void *b) {
    Aluno *alunoA = (Aluno *)a;
    Aluno *alunoB = (Aluno *)b;
    return (alunoA->id - alunoB->id);
}

// Implementa método de ordenação em disco (exceto Insertion Sort).
// Esta abordagem lê todos os registros para a RAM, ordena e depois reescreve no disco.
// Isso minimiza a complexidade de gerenciar múltiplos arquivos temporários,
// mas assume que a base de dados cabe na memória principal.
void ordenar_base_alunos() {
    int total_alunos = obter_total_alunos();
    if (total_alunos == 0) {
        printf("\nBase de alunos está vazia. Nada a ordenar.\n");
        return;
    }

    FILE *file = fopen(ARQUIVO_ALUNOS, "rb");
    if (file == NULL) {
        perror("Não foi possível abrir o arquivo para leitura");
        return;
    }

    Aluno *alunos_em_memoria = (Aluno *)malloc(total_alunos * sizeof(Aluno));
    if (alunos_em_memoria == NULL) {
        perror("Erro de alocação de memória para ordenação");
        fclose(file);
        return;
    }

    fread(alunos_em_memoria, sizeof(Aluno), total_alunos, file);
    fclose(file);

    qsort(alunos_em_memoria, total_alunos, sizeof(Aluno), comparar_alunos);

    file = fopen(ARQUIVO_ALUNOS, "wb");
    if (file == NULL) {
        perror("Não foi possível abrir o arquivo para escrita");
        free(alunos_em_memoria);
        return;
    }

    fwrite(alunos_em_memoria, sizeof(Aluno), total_alunos, file);
    fclose(file);
    free(alunos_em_memoria);

    printf("\nBase de dados de alunos ordenada com sucesso por ID.\n");
}