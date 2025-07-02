
//Camille Silva Oliveira 23.1.8120

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treino.h"
#include "aluno.h"
#include "coach.h" // Incluir o cabeçalho do coach
#include "utils.h"

int obter_total_treinos() {
    FILE *file = fopen(ARQUIVO_TREINOS, "rb");
    if (file == NULL) return 0;
    fseek(file, 0, SEEK_END);
    int total = ftell(file) / sizeof(Treino);
    fclose(file);
    return total;
}

// Operação que exemplifica a interação entre Coach e Treino.
// Corresponde à operação de "Planejamento de treinos".
void agendar_treino() {
    int id_coach;
    printf("Digite o ID do coach responsável: ");
    scanf("%d", &id_coach);

    Coach c = buscar_coach_sequencial(id_coach);
    if (c.id == -1) {
        printf("\nErro: Coach com ID %d não encontrado.\n", id_coach);
        return;
    }

    Treino novo_treino;
    novo_treino.id_coach_responsavel = id_coach;
    novo_treino.id = obter_total_treinos();
    novo_treino.num_inscritos = 0;

    printf("Digite a data do treino (DD/MM/AAAA): ");
    limpar_buffer_entrada();
    scanf("%[^\n]", novo_treino.data);

    printf("Digite o tipo do treino (ex: WOD): ");
    limpar_buffer_entrada();
    scanf("%[^\n]", novo_treino.tipo_treino);

    FILE *file = fopen(ARQUIVO_TREINOS, "ab");
    if(file == NULL) { perror("Erro ao abrir arquivo"); return; }
    fwrite(&novo_treino, sizeof(Treino), 1, file);
    fclose(file);

    printf("\nTreino agendado com sucesso para o coach %s!\n", c.nome);
}

// Operação que exemplifica a interação entre Aluno e Treino.
void inscrever_aluno_em_treino() {
    int id_aluno, id_treino;

    printf("Digite o ID do treino: ");
    scanf("%d", &id_treino);
    printf("Digite o ID do aluno a ser inscrito: ");
    scanf("%d", &id_aluno);

    Aluno a = buscar_aluno_sequencial(id_aluno);
    if (a.id == -1) {
        printf("\nErro: Aluno com ID %d não encontrado.\n", id_aluno);
        return;
    }

    FILE *file = fopen(ARQUIVO_TREINOS, "r+b");
    if (file == NULL) {
        printf("\nNenhum treino cadastrado.\n");
        return;
    }

    Treino treino;
    int encontrado = 0;
    long pos_arquivo = 0;

    while (fread(&treino, sizeof(Treino), 1, file)) {
        if (treino.id == id_treino) {
            encontrado = 1;
            pos_arquivo = ftell(file) - sizeof(Treino);
            break;
        }
    }

    if (!encontrado) {
        printf("\nErro: Treino com ID %d não encontrado.\n", id_treino);
        fclose(file);
        return;
    }

    if (treino.num_inscritos >= MAX_ALUNOS_TREINO) {
        printf("\nErro: O treino já atingiu o limite de vagas.\n");
        fclose(file);
        return;
    }

    for (int i = 0; i < treino.num_inscritos; i++) {
        if (treino.lista_alunos_inscritos[i] == id_aluno) {
            printf("\nErro: Aluno já está inscrito neste treino.\n");
            fclose(file);
            return;
        }
    }

    treino.lista_alunos_inscritos[treino.num_inscritos] = id_aluno;
    treino.num_inscritos++;

    fseek(file, pos_arquivo, SEEK_SET);
    fwrite(&treino, sizeof(Treino), 1, file);
    fclose(file);

    printf("\nAluno %s inscrito com sucesso no treino %d!\n", a.nome, id_treino);
}

void listar_treinos() {
    FILE *file = fopen(ARQUIVO_TREINOS, "rb");
    if (file == NULL) {
        printf("\nNenhum treino agendado.\n");
        return;
    }

    Treino treino;
    printf("\n--- Lista de Treinos Agendados ---\n");
    while (fread(&treino, sizeof(Treino), 1, file)) {
        Coach c = buscar_coach_sequencial(treino.id_coach_responsavel);
        printf("ID: %d | Data: %s | Tipo: %s | Coach: %s | Vagas: %d/%d\n",
               treino.id, treino.data, treino.tipo_treino,
               (c.id != -1 ? c.nome : "N/A"),
               treino.num_inscritos, MAX_ALUNOS_TREINO);
    }
    printf("----------------------------------\n");
    fclose(file);
}

Treino buscar_treino_sequencial(int id) {
    FILE *file = fopen(ARQUIVO_TREINOS, "rb");
    Treino treino;
    treino.id = -1;

    if (file == NULL) return treino;

    while (fread(&treino, sizeof(Treino), 1, file)) {
        if (treino.id == id) {
            fclose(file);
            return treino;
        }
    }
    fclose(file);
    treino.id = -1;
    return treino;
}