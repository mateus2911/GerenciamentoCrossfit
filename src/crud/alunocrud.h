#ifndef ALUNOCrud_H
#define ALUNOCrud_H

#include "models/aluno.h"

// --- Funções do CRUD ---
void createAluno(const char* filename, Aluno aluno);
Aluno* getAlunos(const char* filename, int* n); // Necessária para os testes
void readAllAlunos(const char* filename);
void updateAluno(const char* filename, Aluno aluno);
void deleteAluno(const char* filename, int id);

// --- Função de Ordenação (Parte I) ---
void quicksort(Aluno* alunos, int n);

#endif