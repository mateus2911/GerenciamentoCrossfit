
//Camille Silva Oliveira 23.1.8120

#include <stdio.h>
#include <stdlib.h>
#include "ordenacao.h"
#include "aluno.h"

void ordenar_base_alunos() {
    int total_alunos = obter_total_alunos();
    if (total_alunos <= 1) {
        printf("\nBase de alunos já está ordenada ou vazia. Nada a fazer.\n");
        return;
    }

    // Abre o arquivo para leitura e escrita ("r+b")
    FILE *file = fopen(ARQUIVO_ALUNOS, "r+b");
    if (file == NULL) {
        perror("Não foi possível abrir o arquivo para ordenacao");
        return;
    }

    Aluno aluno_i, aluno_j, min_aluno;
    int i, j, min_idx;

    printf("\nIniciando ordenacao da base de alunos em disco (Selection Sort)\n");

    for (i = 0; i < total_alunos - 1; i++) {
        // Assume que o elemento atual é o mínimo
        min_idx = i;
        fseek(file, i * sizeof(Aluno), SEEK_SET);
        fread(&min_aluno, sizeof(Aluno), 1, file);

        // Procura pelo menor elemento no restante do arquivo
        for (j = i + 1; j < total_alunos; j++) {
            fseek(file, j * sizeof(Aluno), SEEK_SET);
            fread(&aluno_j, sizeof(Aluno), 1, file);

            // Se encontrar um elemento com ID menor, atualiza o mínimo
            if (aluno_j.id < min_aluno.id) {
                min_aluno = aluno_j;
                min_idx = j;
            }
        }

        // Se um novo mínimo foi encontrado, realiza a troca no arquivo
        if (min_idx != i) {
            // Guarda o aluno da posição 'i' na memória para a troca
            fseek(file, i * sizeof(Aluno), SEEK_SET);
            fread(&aluno_i, sizeof(Aluno), 1, file);

            // 1. Escreve o aluno mínimo encontrado (de min_idx) na posição 'i'
            fseek(file, i * sizeof(Aluno), SEEK_SET);
            fwrite(&min_aluno, sizeof(Aluno), 1, file);

            // 2. Escreve o aluno que estava em 'i' na antiga posição do mínimo (min_idx)
            fseek(file, min_idx * sizeof(Aluno), SEEK_SET);
            fwrite(&aluno_i, sizeof(Aluno), 1, file);
        }
    }

    fclose(file);

    printf("\nBase de dados de alunos ordenada com sucesso por ID.\n");
}