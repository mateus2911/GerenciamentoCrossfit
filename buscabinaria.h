#ifndef BUSCABINARIA_H
#define BUSCABINARIA_H

#include "crossfit.h"
#include "coach.h"
#include "aluno.h"
#include "treino.h"

CrossFit* busca_sequencial_crossfit(CrossFit bd[], int tamanho, int id);
CrossFit* busca_binaria_crossfit(CrossFit bd[], int tamanho, int id);

Coach* busca_sequencial_coach(Coach bd[], int tamanho, int id);
Coach* busca_binaria_coach(Coach bd[], int tamanho, int id);

Aluno* busca_sequencial_aluno(Aluno bd[], int tamanho, int id);
Aluno* busca_binaria_aluno(Aluno bd[], int tamanho, int id);

TreinoDiario* busca_sequencial_treino(TreinoDiario bd[], int tamanho, int id);
TreinoDiario* busca_binaria_treino(TreinoDiario bd[], int tamanho, int id);

#endif