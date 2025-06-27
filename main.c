#include <stdio.h>
#include <string.h>
#include "crossfit.h"
#include "coach.h"
#include "aluno.h"
#include "treino.h"
#include "buscabinaria.h"
#include "ordenacao.h"

int main() {
   
    CrossFit bd_crossfit[MAX_CROSSFIT];
    Coach bd_coaches[MAX_COACHES];
    Aluno bd_alunos[MAX_ALUNOS];
    TreinoDiario bd_treinos[MAX_TREINOS];
    int tamanho_crossfit, tamanho_coach, tamanho_aluno, tamanho_treino;

    inicializar_bd_crossfit(bd_crossfit, &tamanho_crossfit);
    inicializar_bd_coach(bd_coaches, &tamanho_coach);
    inicializar_bd_aluno(bd_alunos, &tamanho_aluno);
    inicializar_bd_treino(bd_treinos, &tamanho_treino);

    
    adicionar_crossfit(bd_crossfit, &tamanho_crossfit, 2, "CrossFit PowerHouse", "Av. Força, 456");
    adicionar_crossfit(bd_crossfit, &tamanho_crossfit, 1, "CrossFit IronWill", "Rua Fitness, 123");

    
    char disp_coach1[][MAX_HORARIO] = {"2025-06-26 08:00", "2025-06-26 18:00"};
    char disp_coach2[][MAX_HORARIO] = {"2025-06-26 10:00", "2025-06-27 14:00"};
    char disp_coach3[][MAX_HORARIO] = {"2025-06-26 09:00", "2025-06-27 08:00"};
    adicionar_coach(bd_coaches, &tamanho_coach, 101, "Ana Costa", "Strength", disp_coach1, 2);
    adicionar_coach(bd_coaches, &tamanho_coach, 103, "Clara Mendes", "Gymnastics", disp_coach3, 2);
    adicionar_coach(bd_coaches, &tamanho_coach, 102, "Bruno Silva", "LPO", disp_coach2, 2);

    
    char hist_aluno1[][MAX_ENTRADA_HISTORICO] = {"WOD_2025-06-20: 100kg"};
    char hist_aluno2[][MAX_ENTRADA_HISTORICO] = {"WOD_2025-06-20: 60kg"};
    adicionar_aluno(bd_alunos, &tamanho_aluno, 203, "Pedro Santos", "fitness", NULL, 0);
    adicionar_aluno(bd_alunos, &tamanho_aluno, 201, "João Pereira", "rx", hist_aluno1, 1);
    adicionar_aluno(bd_alunos, &tamanho_aluno, 202, "Maria Oliveira", "scale", hist_aluno2, 1);

    
    bubble_sort_crossfit_disco(bd_crossfit, &tamanho_crossfit, "bd_crossfit.txt");
    bubble_sort_coach_disco(bd_coaches, &tamanho_coach, "bd_coaches.txt");
    bubble_sort_aluno_disco(bd_alunos, &tamanho_aluno, "bd_alunos.txt");
    bubble_sort_treino_disco(bd_treinos, &tamanho_treino, "bd_treinos.txt");

    
    printf("=== Bancos de Dados Iniciais ===\n");
    exibir_bd_crossfit(bd_crossfit, tamanho_crossfit);
    exibir_bd_coach(bd_coaches, tamanho_coach);
    exibir_bd_aluno(bd_alunos, tamanho_aluno);
    exibir_bd_treino(bd_treinos, tamanho_treino);

    
    printf("\n=== Demonstração de Buscas ===\n");
    CrossFit *cf = busca_binaria_crossfit(bd_crossfit, tamanho_crossfit, 1);
    printf("Busca Binária CrossFit ID 1: %s\n", cf ? cf->nome : "Não encontrado");
    Coach *coach = busca_sequencial_coach(bd_coaches, tamanho_coach, 101);
    printf("Busca Sequencial Coach ID 101: %s\n", coach ? coach->nome : "Não encontrado");

    
    printf("\n=== Operação 1: Planejar Treino ===\n");
    int novos_alunos[] = {201, 202};
    planejar_treino(bd_treinos, &tamanho_treino, 301, "2025-06-26 08:00", "Strength", bd_coaches, tamanho_coach, novos_alunos, 2, 5);

    
    printf("\n=== Operação 2: Registrar Progresso de Aluno ===\n");
    Aluno *aluno = busca_sequencial_aluno(bd_alunos, tamanho_aluno, 201);
    if (aluno) {
        registrar_progresso_aluno(aluno, 301, "105kg");
        printf("Progresso registrado para %s\n", aluno->nome);
    }

    
    printf("\n=== Operação 3: Busca de Treinos por Data e Tipo ===\n");
    buscar_treinos_por_data_e_tipo(bd_treinos, tamanho_treino, bd_coaches, tamanho_coach, bd_alunos, tamanho_aluno, "2025-06-26", "Strength");

    
    printf("\n=== Operação 4: Realocar Coach ===\n");
    TreinoDiario *treino = busca_sequencial_treino(bd_treinos, tamanho_treino, 301);
    if (treino) {
        realocar_coach(treino, bd_coaches, tamanho_coach, "2025-06-26 08:00", "Strength");
    }

    
    printf("\n=== Bancos de Dados Finais ===\n");
    exibir_bd_crossfit(bd_crossfit, tamanho_crossfit);
    exibir_bd_coach(bd_coaches, tamanho_coach);
    exibir_bd_aluno(bd_alunos, tamanho_aluno);
    exibir_bd_treino(bd_treinos, tamanho_treino);

    return 0;
}