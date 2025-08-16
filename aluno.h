//Camille Silva Oliveira 23.1.8120
//Mateus Rodrigues 19.2.8177

#ifndef ALUNO_H
#define ALUNO_H

#include "entidades.h" // Para a definição de Aluno

// Forward declaration para evitar dependência circular
struct HashTable;

// Protótipos de funções
void cadastrar_aluno(struct HashTable* ht);
Aluno buscar_aluno_sequencial(int id);
Aluno buscar_aluno_binaria(int id);
void listar_alunos();
int obter_total_alunos();
void consultar_alunos_por_nivel();
void embaralhar_base_alunos();
//void consultar_alunos_por_frequencia();

#endif // ALUNO_H