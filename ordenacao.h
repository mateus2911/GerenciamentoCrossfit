#ifndef ORDENACAO_H
#define ORDENACAO_H

#include <stdio.h>
#include <stdbool.h>
#include "aluno.h"
#include "entidades.h" // Adicionado para struct Aluno

// Estrutura do nó para a árvore de vencedores, usando Aluno.
typedef struct No {
    Aluno aluno;
    int indiceParticao;
} No;

// Estrutura para manter os registros em memória durante a seleção por substituição.
typedef struct {
    Aluno aluno;
    bool congelado;
} RegistroMemoria;

/**
 * @brief Função principal para ordenar a base de dados de alunos.
 *
 * Esta função implementa um método de ordenação externa eficiente (Seleção
 * por Substituição com Intercalação usando Árvore de Vencedores) para ordenar
 * o arquivo de alunos com base no ID.
 */
void ordenar_base_alunos();

/**
 * @brief Ordena o arquivo de alunos usando o método Bubble Sort.
 *
 * ATENÇÃO: Este método é ineficiente para arquivos grandes e serve principalmente
 * para fins de comparação ou para bases de dados muito pequenas.
 * A função ordenar_base_alunos() é a recomendada.
 * @return Retorna 0 em caso de sucesso, 1 em caso de erro.
 */
int bubble_sort_alunos();

#endif // ORDENACAO_H