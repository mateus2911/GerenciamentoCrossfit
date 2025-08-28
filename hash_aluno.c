#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_aluno.h"
#include "entidades.h"

#define ARQUIVO_HASH_ALUNOS "hash_alunos.dat"

// Função Hash: Calcula o endereco base para um determinado ID de aluno
static int hash_function(int id) {
    return id % HASH_TABLE_SIZE;
}

// Cria e inicializa uma nova tabela hash em arquivo
HashTable* criar_hash_table() {
    HashTable* ht = (HashTable*) malloc(sizeof(HashTable));
    if (ht == NULL) {
        perror("Erro ao alocar memoria para a Tabela Hash");
        exit(1);
    }

    // Tenta abrir o arquivo existente, se não existir, cria um novo
    ht->arquivo_hash = fopen(ARQUIVO_HASH_ALUNOS, "r+b");
    if (ht->arquivo_hash == NULL) {
        ht->arquivo_hash = fopen(ARQUIVO_HASH_ALUNOS, "w+b");
        if (ht->arquivo_hash == NULL) {
            perror("Erro ao criar o arquivo da Tabela Hash");
            free(ht);
            exit(1);
        }
        // Inicializa o arquivo com -1 em todas as posições do índice
        long offset_vazio = -1;
        for (int i = 0; i < HASH_TABLE_SIZE; i++) {
            fwrite(&offset_vazio, sizeof(long), 1, ht->arquivo_hash);
        }
    }
    
    ht->ultimo_tempo_busca.QuadPart = 0;
    return ht;
}

// Libera recursos da tabela hash (fecha o arquivo)
void destruir_hash_table(HashTable* ht) {
    if (ht != NULL) {
        if (ht->arquivo_hash != NULL) {
            fclose(ht->arquivo_hash);
        }
        free(ht);
    }
}

// Insere um aluno na tabela hash (arquivo)
void inserir_aluno_hash(HashTable* ht, Aluno aluno) {
    int index = hash_function(aluno.id);
    long indice_offset = index * sizeof(long);

    // Lê o offset do início da lista para este índice
    long head_offset;
    fseek(ht->arquivo_hash, indice_offset, SEEK_SET);
    fread(&head_offset, sizeof(long), 1, ht->arquivo_hash);

    // Prepara o novo nó
    HashNode novo_no;
    novo_no.aluno = aluno;
    novo_no.proximo = head_offset; // O novo nó aponta para o antigo início da lista

    // Vai para o final do arquivo para escrever o novo nó
    fseek(ht->arquivo_hash, 0, SEEK_END);
    long novo_offset = ftell(ht->arquivo_hash);
    fwrite(&novo_no, sizeof(HashNode), 1, ht->arquivo_hash);

    // Atualiza o índice da tabela para apontar para o novo nó
    fseek(ht->arquivo_hash, indice_offset, SEEK_SET);
    fwrite(&novo_offset, sizeof(long), 1, ht->arquivo_hash);

    if (head_offset != -1) {
        printf("Info: Ocorreu uma colisao no indice %d. Inserindo aluno com ID %d.\n", index, aluno.id);
    }
}

// Busca um aluno na tabela hash (arquivo) pelo ID
Aluno* buscar_aluno_hash(HashTable* ht, int id) {
    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    int index = hash_function(id);
    long indice_offset = index * sizeof(long);

    // Lê o offset do início da lista
    long no_offset;
    fseek(ht->arquivo_hash, indice_offset, SEEK_SET);
    fread(&no_offset, sizeof(long), 1, ht->arquivo_hash);

    HashNode no_atual;
    while (no_offset != -1) {
        fseek(ht->arquivo_hash, no_offset, SEEK_SET);
        fread(&no_atual, sizeof(HashNode), 1, ht->arquivo_hash);

        if (no_atual.aluno.id == id && no_atual.aluno.ativo) {
            QueryPerformanceCounter(&end);
            ht->ultimo_tempo_busca.QuadPart = end.QuadPart - start.QuadPart;
            
            // Aloca memória para retornar o aluno encontrado
            Aluno* aluno_encontrado = (Aluno*) malloc(sizeof(Aluno));
            if(aluno_encontrado) {
                *aluno_encontrado = no_atual.aluno;
            }
            return aluno_encontrado; // ATENÇÃO: A memória deve ser liberada pelo chamador
        }
        no_offset = no_atual.proximo;
    }

    QueryPerformanceCounter(&end);
    ht->ultimo_tempo_busca.QuadPart = end.QuadPart - start.QuadPart;
    return NULL; // Aluno não encontrado
}

// Remove um aluno da tabela hash (arquivo) pelo ID (marca como inativo)
// A remoção física em arquivo é complexa, então a abordagem mais simples é a remoção lógica.
void remover_aluno_hash(HashTable* ht, int id) {
    int index = hash_function(id);
    long indice_offset = index * sizeof(long);

    long no_offset;
    fseek(ht->arquivo_hash, indice_offset, SEEK_SET);
    fread(&no_offset, sizeof(long), 1, ht->arquivo_hash);

    HashNode no_atual;
    while (no_offset != -1) {
        long current_pos = no_offset;
        fseek(ht->arquivo_hash, current_pos, SEEK_SET);
        fread(&no_atual, sizeof(HashNode), 1, ht->arquivo_hash);

        if (no_atual.aluno.id == id) {
            no_atual.aluno.ativo = 0; // Marca como inativo
            fseek(ht->arquivo_hash, current_pos, SEEK_SET);
            fwrite(&no_atual, sizeof(HashNode), 1, ht->arquivo_hash);
            printf("Aluno com ID %d marcado como inativo na tabela hash.\n", id);
            return;
        }
        no_offset = no_atual.proximo;
    }

    printf("Aluno com ID %d nao encontrado na tabela hash para remocao.\n", id);
}

// Carrega todos os alunos do arquivo principal para a tabela hash em arquivo
void carregar_alunos_para_hash_table(HashTable* ht) {
    // Primeiro, limpa o arquivo hash antigo se existir
    fclose(ht->arquivo_hash);
    ht->arquivo_hash = fopen(ARQUIVO_HASH_ALUNOS, "w+b");
    if (ht->arquivo_hash == NULL) {
        perror("Erro ao recriar o arquivo da Tabela Hash");
        exit(1);
    }
    // Inicializa o arquivo com -1
    long offset_vazio = -1;
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        fwrite(&offset_vazio, sizeof(long), 1, ht->arquivo_hash);
    }

    FILE *file = fopen(ARQUIVO_ALUNOS, "rb");
    if (file == NULL) {
        printf("Arquivo de alunos nao encontrado. Tabela Hash vazia.\n");
        return;
    }

    Aluno aluno;
    while (fread(&aluno, sizeof(Aluno), 1, file)) {
        if (aluno.ativo) {
            inserir_aluno_hash(ht, aluno);
        }
    }
    fclose(file);
    printf("Tabela Hash de alunos recarregada em memoria.\n");
}