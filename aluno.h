
//Camille Silva Oliveira 23.1.8120

#ifndef ALUNO_H
#define ALUNO_H

#include "entidades.h"

void cadastrar_aluno();
Aluno buscar_aluno_sequencial(int id);
Aluno buscar_aluno_binaria(int id);
void listar_alunos();
int obter_total_alunos();
void consultar_alunos_por_nivel();
void consultar_alunos_por_frequencia();

#endif // ALUNO_H