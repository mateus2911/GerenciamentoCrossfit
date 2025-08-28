#ifndef HASH_ALUNO_H
#define HASH_ALUNO_H

#include <windows.h> // Para usar LARGE_INTEGER para medições de tempo precisas
#include <stdio.h>   // Necessário para o tipo FILE
#include "entidades.h" // Para a definição de Aluno

// Define o tamanho da tabela hash
#define HASH_TABLE_SIZE 101

// Estrutura do nó da lista encadeada para colisões (armazenado em arquivo)
typedef struct HashNode {
    Aluno aluno;
    long proximo; // Offset para o próximo nó no arquivo, -1 se for o último
} HashNode;

// Estrutura da tabela hash que opera em arquivo
typedef struct HashTable {
    FILE* arquivo_hash; // Ponteiro para o arquivo que armazena a tabela hash
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