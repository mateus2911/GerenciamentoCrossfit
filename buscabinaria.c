#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "buscabinaria.h"
#include "ordenacao.h"

CrossFit* busca_sequencial_crossfit(CrossFit bd[], int tamanho, int id) {
    for (int i = 0; i < tamanho; i++) {
        if (bd[i].id_crossfit == id) {
            return &bd[i];
        }
    }
    return NULL;
}

CrossFit* busca_binaria_crossfit(CrossFit bd[], int tamanho, int id) {
    ordenar_bd_crossfit(bd, tamanho);
    int esquerda = 0, direita = tamanho - 1;
    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;
        if (bd[meio].id_crossfit == id) {
            return &bd[meio];
        }
        if (bd[meio].id_crossfit < id) {
            esquerda = meio + 1;
        } else {
            direita = meio - 1;
        }
    }
    return NULL;
}

Coach* busca_sequencial_coach(Coach bd[], int tamanho, int id) {
    for (int i = 0; i < tamanho; i++) {
        if (bd[i].id_coach == id) {
            return &bd[i];
        }
    }
    return NULL;
}

Coach* busca_binaria_coach(Coach bd[], int tamanho, int id) {
    ordenar_bd_coach(bd, tamanho);
    int esquerda = 0, direita = tamanho - 1;
    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;
        if (bd[meio].id_coach == id) {
            return &bd[meio];
        }
        if (bd[meio].id_coach < id) {
            esquerda = meio + 1;
        } else {
            direita = meio - 1;
        }
    }
    return NULL;
}

Aluno* busca_sequencial_aluno(Aluno bd[], int tamanho, int id) {
    for (int i = 0; i < tamanho; i++) {
        if (bd[i].id_aluno == id) {
            return &bd[i];
        }
    }
    return NULL;
}

Aluno* busca_binaria_aluno(Aluno bd[], int tamanho, int id) {
    ordenar_bd_aluno(bd, tamanho);
    int esquerda = 0, direita = tamanho - 1;
    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;
        if (bd[meio].id_aluno == id) {
            return &bd[meio];
        }
        if (bd[meio].id_aluno < id) {
            esquerda = meio + 1;
        } else {
            direita = meio - 1;
        }
    }
    return NULL;
}

TreinoDiario* busca_sequencial_treino(TreinoDiario bd[], int tamanho, int id) {
    for (int i = 0; i < tamanho; i++) {
        if (bd[i].id == id) {
            return &bd[i];
        }
    }
    return NULL;
}

TreinoDiario* busca_binaria_treino(TreinoDiario bd[], int tamanho, int id) {
    ordenar_bd_treino(bd, tamanho);
    int esquerda = 0, direita = tamanho - 1;
    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;
        if (bd[meio].id == id) {
            return &bd[meio];
        }
        if (bd[meio].id < id) {
            esquerda = meio + 1;
        } else {
            direita = meio - 1;
        }
    }
    return NULL;
}