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

void ordenar_base_alunos();

int bubble_sort_alunos();

#endif // ORDENACAO_H