#ifndef HASH_ALUNO_H
#define HASH_ALUNO_H

#include <windows.h> // Para usar LARGE_INTEGER para medições de tempo precisas
#include "entidades.h" // Para a definição de Aluno

// Define o tamanho da tabela hash
#define HASH_TABLE_SIZE 101

// Estrutura do nó da lista encadeada para colisões
typedef struct HashNode {
    Aluno aluno;
    struct HashNode* proximo;
} HashNode;

// Estrutura da tabela hash usando LARGE_INTEGER para tempo de alta precisão
typedef struct HashTable {
    HashNode* tabela[HASH_TABLE_SIZE];
    LARGE_INTEGER ultimo_tempo_busca; // Usar o tipo do Windows para tempo
} HashTable;

// Protótipos de funções
HashTable* criar_hash_table();
void destruir_hash_table(HashTable* ht);
void inserir_aluno_hash(HashTable* ht, Aluno aluno);
Aluno* buscar_aluno_hash(HashTable* ht, int id);
void remover_aluno_hash(HashTable* ht, int id);
void carregar_alunos_para_hash_table(HashTable* ht);

#endif // HASH_ALUNO_H