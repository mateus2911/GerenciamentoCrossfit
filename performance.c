#include <stdio.h>
#include <string.h>
#include "aluno.h"
#include "treino.h"
#include "performance.h"
#include "entidades.h"
#include "utils.h"

// Operação de "Acompanhamento de progresso", interagindo com Aluno e Treino.
void registrar_performance_aluno() {
    int id_aluno, id_treino;
    Performance nova_performance;

    printf("Digite o ID do aluno: ");
    scanf("%d", &id_aluno);
    printf("Digite o ID do treino: ");
    scanf("%d", &id_treino);

    Aluno aluno = buscar_aluno_sequencial(id_aluno);
    if (aluno.id == -1) {
        printf("\nErro: Aluno com ID %d nao foi encontrado.\n", id_aluno);
        return;
    }

    Treino treino = buscar_treino_sequencial(id_treino);
    if (treino.id == -1) {
        printf("\nErro: Treino com ID %d nao foi encontrado.\n", id_treino);
        return;
    }

    int aluno_inscrito = 0;
    for (int i = 0; i < treino.num_inscritos; i++) {
        if (treino.lista_alunos_inscritos[i] == id_aluno) {
            aluno_inscrito = 1;
            break;
        }
    }
    if (!aluno_inscrito) {
        printf("\nErro: O aluno %s nao esta inscrito no treino %d.\n", aluno.nome, id_treino);
        return;
    }

    nova_performance.id_aluno = id_aluno;
    nova_performance.id_treino = id_treino;
    printf("Digite o resultado do aluno (ex: 15:30 RX): ");
    limpar_buffer_entrada();
    scanf("%[^\n]", nova_performance.resultado);

    FILE* file = fopen(ARQUIVO_PERFORMANCE, "ab");
    if(file == NULL) { perror("Erro ao abrir arquivo"); return; }
    fwrite(&nova_performance, sizeof(Performance), 1, file);
    fclose(file);

    printf("\nPerformance do aluno %s registrada com sucesso!\n", aluno.nome);
}

void consultar_historico_aluno(int id_aluno) {
    Aluno aluno = buscar_aluno_sequencial(id_aluno);
    if (aluno.id == -1) {
        printf("\nErro: Aluno com ID %d nao foi encontrado.\n", id_aluno);
        return;
    }

    printf("\n--- Historico de Treinos de %s ---\n", aluno.nome);

    FILE* file = fopen(ARQUIVO_PERFORMANCE, "rb");
    if (file == NULL) {
        printf("Nenhum historico de performance encontrado.\n");
        return;
    }

    Performance p;
    int count = 0;
    while(fread(&p, sizeof(Performance), 1, file)) {
        if (p.id_aluno == id_aluno) {
            Treino t = buscar_treino_sequencial(p.id_treino);
            if (t.id != -1) {
                printf("Treino ID %d (%s em %s) - Resultado: %s\n",
                       t.id, t.tipo_treino, t.data, p.resultado);
            }
            count++;
        }
    }

    if (count == 0) {
        printf("Nenhum registro de performance para este aluno.\n");
    }
    printf("---------------------------------------\n");
    fclose(file);
}