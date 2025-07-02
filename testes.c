//Camille Silva Oliveira 23.1.8120

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "testes.h"
#include "entidades.h"
#include "aluno.h"
#include "coach.h"
#include "treino.h"
#include "ordenacao.h" // Necessário para chamar a função de ordenar

void gerar_alunos_aleatorios(int quantidade) {
    FILE *file = fopen(ARQUIVO_ALUNOS, "wb"); // "wb" para criar um arquivo novo para o teste
    if (file == NULL) {
        perror("Falha ao criar arquivo de alunos para teste");
        return;
    }

    const char* nomes[] = {"Ana", "Bruno", "Carlos", "Daniela", "Eduardo", "Fernanda", "Gabriel", "Helena", "Igor", "Julia"};
    const char* sobrenomes[] = {"Silva", "Souza", "Oliveira", "Pereira", "Costa", "Rodrigues", "Martins", "Almeida", "Barros", "Ribeiro"};
    const char* niveis[] = {"fitness", "scale", "rx"};

    int num_nomes = sizeof(nomes) / sizeof(char*);
    int num_sobrenomes = sizeof(sobrenomes) / sizeof(char*);
    int num_niveis = sizeof(niveis) / sizeof(char*);

    printf("Gerando %d registros de alunos aleatórios... ", quantidade);

    for (int i = 0; i < quantidade; i++) {
        Aluno aluno;
        aluno.id = i;
        aluno.ativo = 1;
        sprintf(aluno.nome, "%s %s", nomes[rand() % num_nomes], sobrenomes[rand() % num_sobrenomes]);
        strcpy(aluno.nivel, niveis[rand() % num_niveis]);
        fwrite(&aluno, sizeof(Aluno), 1, file);
    }

    fclose(file);
    printf("Concluído.\n");
}

void gerar_coaches_aleatorios(int quantidade) {
    FILE* file = fopen(ARQUIVO_COACHES, "wb");
    if (file == NULL) {
        perror("Falha ao criar arquivo de coaches para teste");
        return;
    }

    const char* nomes[] = {"Andre", "Beatriz", "Caio", "Debora", "Elton", "Fabiana"};
    const char* sobrenomes[] = {"Mendes", "Nunes", "Lima", "Gomes", "Ferreira", "Araujo"};
    const char* especialidades[] = {"Força", "LPO", "Ginástica", "Cardio", "Mobilidade"};

    int num_nomes = sizeof(nomes) / sizeof(char*);
    int num_sobrenomes = sizeof(sobrenomes) / sizeof(char*);
    int num_especialidades = sizeof(especialidades) / sizeof(char*);

    for (int i = 0; i < quantidade; i++) {
        Coach coach;
        coach.id = i;
        coach.ativo = 1;
        sprintf(coach.nome, "%s %s", nomes[rand() % num_nomes], sobrenomes[rand() % num_sobrenomes]);
        strcpy(coach.especialidade, especialidades[rand() % num_especialidades]);
        fwrite(&coach, sizeof(Coach), 1, file);
    }
    fclose(file);
}

void gerar_treinos_aleatorios(int quantidade, int total_alunos, int total_coaches) {
    if (total_alunos == 0 || total_coaches == 0) return;
    FILE* file = fopen(ARQUIVO_TREINOS, "wb");
    if (file == NULL) {
        perror("Falha ao criar arquivo de treinos para teste");
        return;
    }
    
    const char* tipos_treino[] = {"WOD", "AMRAP 15min", "For Time", "EMOM 20min", "Strength Day"};
    int num_tipos = sizeof(tipos_treino) / sizeof(char*);

    for (int i = 0; i < quantidade; i++) {
        Treino treino;
        treino.id = i;
        sprintf(treino.data, "%02d/%02d/2025", (rand() % 28) + 1, (rand() % 12) + 1);
        strcpy(treino.tipo_treino, tipos_treino[rand() % num_tipos]);
        treino.id_coach_responsavel = rand() % total_coaches;
        treino.num_inscritos = rand() % (MAX_ALUNOS_TREINO + 1);
        for(int j=0; j < treino.num_inscritos; j++){
            treino.lista_alunos_inscritos[j] = rand() % total_alunos;
        }
        fwrite(&treino, sizeof(Treino), 1, file);
    }
    fclose(file);
}

void executar_suite_de_testes() {
    int qtd_registros;
    printf("\n--- Suíte de Testes de Desempenho ---\n");
    printf("Digite a quantidade de registros de alunos a serem gerados para o teste: ");
    scanf("%d", &qtd_registros);

    if (qtd_registros <= 0) {
        printf("Quantidade inválida.\n");
        return;
    }

    // Seed para números aleatórios
    srand(time(NULL));

    // 1. GERAÇÃO DE DADOS
    gerar_alunos_aleatorios(qtd_registros);
    // Gerar outros dados se necessário para testes mais complexos
    // gerar_coaches_aleatorios(qtd_registros / 100 + 1);
    // gerar_treinos_aleatorios(qtd_registros / 10, qtd_registros, qtd_registros / 100 + 1);

    // 2. TESTE DE BUSCA SEQUENCIAL (em arquivo desordenado)
    printf("\n--- Testando Busca Sequencial ---\n");
    int id_busca = rand() % qtd_registros; // Escolhe um ID aleatório para buscar
    printf("Buscando pelo ID aleatório: %d\n", id_busca);

    clock_t inicio = clock();
    Aluno aluno_encontrado_seq = buscar_aluno_sequencial(id_busca);
    clock_t fim = clock();
    double tempo_sequencial = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    
    if (aluno_encontrado_seq.id != -1) {
        printf("Aluno '%s' encontrado.\n", aluno_encontrado_seq.nome);
    } else {
        printf("Aluno com ID %d não encontrado.\n", id_busca);
    }
    printf("Tempo da busca sequencial: %f segundos.\n", tempo_sequencial);

    // 3. TESTE DE ORDENAÇÃO
    printf("\n--- Testando Ordenação em Disco ---\n");
    inicio = clock();
    ordenar_base_alunos();
    fim = clock();
    double tempo_ordenacao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("Tempo para ordenar a base de dados: %f segundos.\n", tempo_ordenacao);

    // 4. TESTE DE BUSCA BINÁRIA (em arquivo ordenado)
    printf("\n--- Testando Busca Binária ---\n");
    printf("Buscando pelo mesmo ID: %d\n", id_busca);
    
    inicio = clock();
    Aluno aluno_encontrado_bin = buscar_aluno_binaria(id_busca);
    fim = clock();
    double tempo_binaria = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    if (aluno_encontrado_bin.id != -1) {
        printf("Aluno '%s' encontrado.\n", aluno_encontrado_bin.nome);
    } else {
        printf("Aluno com ID %d não encontrado.\n", id_busca);
    }
    printf("Tempo da busca binária: %f segundos.\n", tempo_binaria);

    // 5. COMPARAÇÃO
    printf("\n--- Comparativo de Desempenho ---\n");
    printf("Busca Sequencial: %f s\n", tempo_sequencial);
    printf("Busca Binária:    %f s\n", tempo_binaria);
    if(tempo_binaria > 0) {
        printf("A busca binária foi aproximadamente %.2fx mais rápida que a sequencial (desconsiderando o tempo de ordenação).\n", tempo_sequencial / tempo_binaria);
    }
    printf("Custo da ordenação: %f s\n", tempo_ordenacao);
    printf("----------------------------------\n");
}