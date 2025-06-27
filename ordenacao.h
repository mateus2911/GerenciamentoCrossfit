#ifndef ORDENACAO_H
#define ORDENACAO_H

#include "crossfit.h"
#include "coach.h"
#include "aluno.h"
#include "treino.h"

void ordenar_bd_crossfit(CrossFit bd[], int tamanho);
void bubble_sort_crossfit_disco(CrossFit bd[], int *tamanho, const char *nome_arquivo);

void ordenar_bd_coach(Coach bd[], int tamanho);
void bubble_sort_coach_disco(Coach bd[], int *tamanho, const char *nome_arquivo);

void ordenar_bd_aluno(Aluno bd[], int tamanho);
void bubble_sort_aluno_disco(Aluno bd[], int *tamanho, const char *nome_arquivo);

void ordenar_bd_treino(TreinoDiario bd[], int tamanho);
void bubble_sort_treino_disco(TreinoDiario bd[], int *tamanho, const char *nome_arquivo);

#endif