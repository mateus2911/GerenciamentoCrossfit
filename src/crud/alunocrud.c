#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "models/aluno.h" // Corrigido para o caminho correto
#include "crud/alunocrud.h" // Inclui o seu próprio cabeçalho

void createAluno(const char* filename, Aluno aluno) {
    FILE* file = fopen(filename, "r+b");
    if (file == NULL) {
        file = fopen(filename, "w+b");
        int count = 0;
        fwrite(&count, sizeof(int), 1, file);
    }

    int count;
    fseek(file, 0, SEEK_SET);
    fread(&count, sizeof(int), 1, file);
    count++;

    fseek(file, 0, SEEK_SET);
    fwrite(&count, sizeof(int), 1, file);

    fseek(file, 0, SEEK_END);
    fwrite(&aluno, sizeof(Aluno), 1, file);

    fclose(file);
}

Aluno* getAlunos(const char* filename, int* n) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        *n = 0;
        return NULL;
    }

    fread(n, sizeof(int), 1, file);
    Aluno* alunos = (Aluno*) malloc(sizeof(Aluno) * (*n));
    fread(alunos, sizeof(Aluno), *n, file);

    fclose(file);
    return alunos;
}

void readAllAlunos(const char* filename) {
    int n;
    Aluno* alunos = getAlunos(filename, &n);

    if (n == 0) {
        printf("Nenhum aluno cadastrado.\n");
        return;
    }

    printf("\n--- Lista de Alunos ---\n");
    for (int i = 0; i < n; i++) {
        printf("ID: %d, Nome: %s, Email: %s\n", alunos[i].id, alunos[i].nome, alunos[i].email);
    }
    printf("-----------------------\n");

    free(alunos);
}

void updateAluno(const char* filename, Aluno aluno) {
    int n;
    Aluno* alunos = getAlunos(filename, &n);
    int found = 0;

    for (int i = 0; i < n; i++) {
        if (alunos[i].id == aluno.id) {
            strcpy(alunos[i].nome, aluno.nome);
            strcpy(alunos[i].email, aluno.email);
            strcpy(alunos[i].senha, aluno.senha);
            found = 1;
            break;
        }
    }

    if (found) {
        FILE* file = fopen(filename, "wb");
        fwrite(&n, sizeof(int), 1, file);
        fwrite(alunos, sizeof(Aluno), n, file);
        fclose(file);
        printf("Aluno atualizado com sucesso!\n");
    } else {
        printf("Aluno com ID %d nao encontrado.\n", aluno.id);
    }

    free(alunos);
}

void deleteAluno(const char* filename, int id) {
    int n;
    Aluno* alunos = getAlunos(filename, &n);
    int found_index = -1;

    for (int i = 0; i < n; i++) {
        if (alunos[i].id == id) {
            found_index = i;
            break;
        }
    }

    if (found_index != -1) {
        FILE* file = fopen(filename, "wb");
        n--;
        fwrite(&n, sizeof(int), 1, file);
        for (int i = 0; i < n + 1; i++) {
            if (i !=