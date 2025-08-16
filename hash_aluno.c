#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_aluno.h"
#include "entidades.h"

// Função Hash: Calcula o índice para um determinado ID de aluno
static int hash_function(int id) {
    return id % HASH_TABLE_SIZE;
}

// Cria e inicializa uma nova tabela hash
HashTable* criar_hash_table() {
    HashTable* ht = (HashTable*) malloc(sizeof(HashTable));
    if (ht == NULL) {
        perror("Erro ao alocar memoria para a Tabela Hash");
        exit(1);
    }
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        ht->tabela[i] = NULL;
    }
    ht->ultimo_tempo_busca.QuadPart = 0; // Inicializa corretamente como LARGE_INTEGER
    return ht;
}

// Libera toda a memória alocada para a tabela hash
void destruir_hash_table(HashTable* ht) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        HashNode* no_atual = ht->tabela[i];
        while (no_atual != NULL) {
            HashNode* temp = no_atual;
            no_atual = no_atual->proximo;
            free(temp);
        }
    }
    free(ht);
}

// Insere um aluno na tabela hash
void inserir_aluno_hash(HashTable* ht, Aluno aluno) {
    int index = hash_function(aluno.id);
    
    // Cria um novo nó
    HashNode* novo_no = (HashNode*) malloc(sizeof(HashNode));
    if (novo_no == NULL) {
        perror("Erro ao alocar memoria para o no da Tabela Hash");
        return;
    }
    novo_no->aluno = aluno;
    novo_no->proximo = NULL;

    // Insere o nó na lista encadeada do índice correspondente
    if (ht->tabela[index] == NULL) {
        ht->tabela[index] = novo_no;
    } else {
        // Adiciona no início da lista para simplicidade
        novo_no->proximo = ht->tabela[index];
        ht->tabela[index] = novo_no;
    }
}

// Busca um aluno na tabela hash pelo ID
Aluno* buscar_aluno_hash(HashTable* ht, int id) {
    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    
    int index = hash_function(id);
    HashNode* no_atual = ht->tabela[index];

    // Percorre a lista encadeada no índice
    while (no_atual != NULL) {
        if (no_atual->aluno.id == id && no_atual->aluno.ativo) {
            QueryPerformanceCounter(&end);
            ht->ultimo_tempo_busca.QuadPart = end.QuadPart - start.QuadPart;
            // Mostra valores debug
            printf("DEBUG: start=%lld, end=%lld, diff=%lld, freq=%lld\n", 
                   start.QuadPart, end.QuadPart, 
                   ht->ultimo_tempo_busca.QuadPart, frequency.QuadPart);
            return &no_atual->aluno; // Retorna o ponteiro para o aluno encontrado
        }
        no_atual = no_atual->proximo;
    }
    
    QueryPerformanceCounter(&end);
    ht->ultimo_tempo_busca.QuadPart = end.QuadPart - start.QuadPart;
   
    return NULL; // Aluno não encontrado
}

// Remove um aluno da tabela hash pelo ID
void remover_aluno_hash(HashTable* ht, int id) {
    int index = hash_function(id);
    HashNode* no_atual = ht->tabela[index];
    HashNode* anterior = NULL;

    while (no_atual != NULL && no_atual->aluno.id != id) {
        anterior = no_atual;
        no_atual = no_atual->proximo;
    }

    // Se o aluno foi encontrado
    if (no_atual != NULL) {
        if (anterior == NULL) { // O nó a ser removido é o primeiro da lista
            ht->tabela[index] = no_atual->proximo;
        } else { // O nó está no meio ou fim da lista
            anterior->proximo = no_atual->proximo;
        }
        free(no_atual);
        printf("Aluno com ID %d removido da tabela hash.\n", id);
    } else {
        printf("Aluno com ID %d nao encontrado na tabela hash para remocao.\n", id);
    }
}

// Carrega todos os alunos do arquivo para a tabela hash
void carregar_alunos_para_hash_table(HashTable* ht) {
    FILE *file = fopen(ARQUIVO_ALUNOS, "rb");
    if (file == NULL) {
        // Arquivo pode não existir ainda, o que é normal na primeira execução
        return;
    }

    Aluno aluno;
    while (fread(&aluno, sizeof(Aluno), 1, file)) {
        if (aluno.ativo) {
            inserir_aluno_hash(ht, aluno);
        }
    }
    fclose(file);
    printf("Tabela Hash de alunos carregada em memoria.\n");
}