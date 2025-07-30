//Camille Silva Oliveira 23.1.8120
//Mateus Rodrigues 19.2.8177


#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "aluno.h"
#include "coach.h"
#include "treino.h"
#include "performance.h"
#include "ordenacao.h"
#include "crossfit.h"
#include "utils.h"
#include "notificacao.h"
#include "testes.h"
#include <time.h>
#include "crud/alunocrud.h"
#include "services/ordenacao.h"


void inicializar_arquivos() {
    FILE *fx = fopen(ARQUIVO_CROSSFIT, "ab"); if(fx) fclose(fx);
    FILE *fa = fopen(ARQUIVO_ALUNOS, "ab"); if(fa) fclose(fa);
    FILE *fc = fopen(ARQUIVO_COACHES, "ab"); if(fc) fclose(fc);
    FILE *ft = fopen(ARQUIVO_TREINOS, "ab"); if(ft) fclose(ft);
    FILE *fp = fopen(ARQUIVO_PERFORMANCE, "ab"); if(fp) fclose(fp);
    FILE *fn = fopen(ARQUIVO_NOTIFICACOES, "ab"); if(fn) fclose(fn);
}

void exibir_menu() {
    printf("\n--- Menu Principal ---\n");
    printf("1. Cadastrar Aluno\n");
    printf("2. Listar Alunos\n");
    printf("3. Buscar Aluno (Sequencial)\n");
    printf("4. Buscar Aluno (Binaria)\n");
    printf("5. Cadastrar Coach\n");
    printf("6. Listar Coaches\n");
    printf("7. Agendar Treino\n");
    printf("8. Inscrever Aluno em Treino\n");
    printf("9. Listar Treinos\n");
    printf("10. Registrar Performance\n");
    printf("11. Consultar Historico de Aluno\n");
    printf("12. Gerenciar Unidade CrossFit\n");
    printf("13. Exibir Dados da Unidade\n");
    printf("14. Gerar Notificacoes\n");
    printf("15. Exibir Minhas Notificacoes\n");
    printf("16. Busca Avancada\n");
    printf("0. Sair\n");
    printf("\nEscolha uma opcao: ");
}

void exibir_menu_busca_avancada() {
    printf("\n--- Busca Avancada ---\n");
    printf("1. Consultar treinos por tipo\n");
    printf("2. Consultar treinos por data\n");
    printf("3. Consultar alunos por nivel\n");
    printf("4. Consultar alunos por frequencia\n");
    printf("5. Consultar coaches por carga horaria\n");
    printf("0. Voltar ao menu principal\n");
    printf("\nEscolha uma opcao: ");
}

void menu_busca_avancada() {
    int opcao;
    do {
        exibir_menu_busca_avancada();
        if (scanf("%d", &opcao) != 1) {
            printf("\nEntrada invalida. Por favor, digite um numero.\n");
            limpar_buffer_entrada();
            continue;
        }

        switch (opcao) {
            case 1: consultar_treinos_por_tipo(); break;
            case 2: consultar_treinos_por_data(); break;
            case 3: consultar_alunos_por_nivel(); break;
            case 4: consultar_alunos_por_frequencia(); break;
            case 5: consultar_coaches_por_carga_horaria(); break;
            case 0: break; // Volta ao menu principal
            default: printf("Opcao invalida! Tente novamente.\n"); break;
        }
    } while (opcao != 0);
}

// Função para gerar uma base de dados aleatória para os testes
void criar_base_de_dados_teste(const char* nome_arquivo, int num_registros) {
    FILE* f = fopen(nome_arquivo, "wb");
    if (!f) {
        perror("Erro ao criar base de dados para teste");
        exit(1);
    }
    
    fwrite(&num_registros, sizeof(int), 1, f);
    
    srand(time(NULL));
    for (int i = 0; i < num_registros; i++) {
        Aluno a;
        a.id = rand() % (num_registros * 10);
        sprintf(a.nome, "Aluno Teste %d", i);
        sprintf(a.email, "teste%d@email.com", i);
        sprintf(a.senha, "senha%d", i);
        fwrite(&a, sizeof(Aluno), 1, f);
    }
    fclose(f);
}

// Função que executa os testes de ordenação
void executar_testes_ordenacao() {
    int tamanhos[] = {500, 2000, 10000};
    int num_tamanhos = sizeof(tamanhos) / sizeof(int);
    
    int tam_memoria = 100; // Parâmetro M para Seleção com Substituição
    int num_caminhos = 10; // Parâmetro F para Intercalação Ótima

    FILE* log_file = fopen("log_testes.txt", "w");
    FILE* relatorio_file = fopen("relatorio.txt", "w");

    printf("Iniciando testes de ordenacao...\n");
    fprintf(log_file, "INICIO DOS TESTES DE ORDENACAO\nConfiguracao: Memoria (M)=%d, Caminhos (F)=%d\n\n", tam_memoria, num_caminhos);
    fprintf(relatorio_file, "RELATORIO DE COMPARACAO DE METODOS DE ORDENACAO\n==============================================\n\n");

    for (int i = 0; i < num_tamanhos; i++) {
        char nome_arquivo[40];
        sprintf(nome_arquivo, "alunos_teste_%d.db", tamanhos[i]);
        
        fprintf(log_file, "--- Testando com base de %d registros ---\n", tamanhos[i]);
        fprintf(relatorio_file, "--- Base de %d registros ---\n", tamanhos[i]);

        criar_base_de_dados_teste(nome_arquivo, tamanhos[i]);
        fprintf(log_file, "Base de dados '%s' criada.\n", nome_arquivo);

        // --- Teste 1: Ordenação Interna (Quicksort da Parte I) ---
        int n_interna;
        Aluno* alunos_interna = getAlunos(nome_arquivo, &n_interna);
        
        clock_t inicio_interno = clock();
        quicksort(alunos_interna, n_interna); // Usa a função do alunocrud
        clock_t fim_interno = clock();
        long tempo_interno = ((double)(fim_interno - inicio_interno) / CLOCKS_PER_SEC) * 1000;
        
        fprintf(log_file, "Tempo Ordenacao Interna (Quicksort): %ld ms\n", tempo_interno);
        fprintf(relatorio_file, "-> Tempo Ordenacao Interna (Quicksort): %ld ms\n", tempo_interno);
        free(alunos_interna);

        // --- Teste 2: Ordenação Externa (Implementação da Parte II) ---
        char arq_saida_externo[50];
        sprintf(arq_saida_externo, "alunos_externo_%d.db", tamanhos[i]);

        clock_t inicio_externo = clock();
        ordenacao_externa(nome_arquivo, arq_saida_externo, tam_memoria, num_caminhos);
        clock_t fim_externo = clock();
        long tempo_externo = ((double)(fim_externo - inicio_externo) / CLOCKS_PER_SEC) * 1000;

        fprintf(log_file, "Tempo Ordenacao Externa: %ld ms\n\n", tempo_externo);
        fprintf(relatorio_file, "-> Tempo Ordenacao Externa: %ld ms\n\n", tempo_externo);
        
        // Limpeza dos arquivos de teste
        remove(nome_arquivo);
        remove(arq_saida_externo);
    }

    fprintf(relatorio_file, "\n--- Conclusao Geral ---\n");
    fprintf(relatorio_file, "A Ordenacao Interna (Quicksort) demonstrou ser extremamente rapida para bases de dados que podem ser carregadas na RAM, devido a ausencia de operacoes de I/O em disco, que sao lentas.\n\n");
    fprintf(relatorio_file, "A Ordenacao Externa, embora mais lenta para bases menores por causa do I/O intensivo, e a unica solucao viavel para ordenar arquivos que excedem a capacidade da memoria. O metodo se mostrou escalavel e robusto, lidando com os dados em blocos (particoes) e gerenciando a intercalacao de forma otimizada.\n");

    fclose(log_file);
    fclose(relatorio_file);
    printf("Testes finalizados. Verifique os arquivos 'log_testes.txt' e 'relatorio.txt'.\n");
}




int main() {
    setlocale(LC_ALL, "Portuguese_Brazil.1252"); // Corrige caracteres especiais
    int quantidade_alunos = 0;
    int opcao;
    int id_busca; // Declarada aqui para ser usada em múltiplos cases
    Aluno aluno_resultado; // Variável reutilizável para buscas
    int id_usuario, tipo_usuario;
    inicializar_arquivos();

    do {
        exibir_menu();
        if (scanf("%d", &opcao) != 1) {
            printf("\nEntrada invalida. Por favor, digite um numero.\n");
            limpar_buffer_entrada();
            continue;
        }

        switch (opcao) {
            case 1: 
            printf("Quantos alunos deseja criar(para fins de teste)? ");
            scanf("%d", &quantidade_alunos);
            gerar_alunos_aleatorios(quantidade_alunos);
            listar_alunos();
            printf("\nBase de alunos desordenada criada com %d registros.\n", quantidade_alunos);
            ordenar_base_alunos(); 
            printf("Base de alunos ordenada com sucesso.\n");
            break;
            case 2: listar_alunos(); break;
            case 3: 
                printf("Digite o ID do aluno: ");
                scanf("%d", &id_busca);
                aluno_resultado = buscar_aluno_sequencial(id_busca);
                if (aluno_resultado.id != -1) printf("Aluno encontrado: %s\n", aluno_resultado.nome);
                else printf("Aluno nao encontrado.\n");
                break;
            case 4: 
                printf("Digite o ID do aluno: ");
                scanf("%d", &id_busca);
                aluno_resultado = buscar_aluno_binaria(id_busca);
                if (aluno_resultado.id != -1) printf("Aluno encontrado: %s\n", aluno_resultado.nome);
                else printf("Aluno nao encontrado.\n");
                break;
            case 5: cadastrar_coach(); break;
            case 6: listar_coaches(); break;
            case 7: agendar_treino(); break;
            case 8: inscrever_aluno_em_treino(); break;
            case 9: listar_treinos(); break;
            case 10: registrar_performance_aluno(); break;
            case 11: 
                printf("Digite o ID do aluno: ");
                scanf("%d", &id_busca);
                consultar_historico_aluno(id_busca);
                break;
            case 12: gerenciar_unidade_crossfit(); break;
            case 13: exibir_dados_unidade(); break;
            case 14: verificar_e_gerar_notificacoes();
                printf("Notificacoes geradas com sucesso.\n");
                break;
            case 15:
                printf("Voce e (1) Aluno ou (2) Coach? ");
                scanf("%d", &tipo_usuario);
                printf("Digite seu ID: ");
                scanf("%d", &id_usuario);
                if (tipo_usuario == 1 || tipo_usuario == 2) {
                    exibir_notificacoes_pendentes(id_usuario, tipo_usuario);
                } else {
                    printf("Tipo de usuario invalido.\n");
                }
                break;
            case 16: menu_busca_avancada(); break; // Adiciona o menu de busca avançada
            case 0: printf("Saindo do sistema...\n"); break;
            default: printf("Opcao invalida! Tente novamente.\n"); break;
        }
    } while (opcao != 0);

    return 0;

    int op;
    char nomeDoArquivo[] = "alunos.db";

    do {
        printf("\nMenu de Opcoes:\n");
        printf("1 - Adicionar Aluno\n");
        printf("2 - Listar Alunos\n");
        printf("3 - Atualizar Aluno\n");
        printf("4 - Deletar Aluno\n");
        printf("5 - Testes de Ordenacao (PARTE II)\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &op);

        int id;
        Aluno a;

        switch (op) {
            case 1:
                printf("ID: ");
                scanf("%d", &a.id);
                printf("Nome: ");
                scanf(" %[^\n]", a.nome);
                printf("Email: ");
                scanf(" %[^\n]", a.email);
                printf("Senha: ");
                scanf(" %[^\n]", a.senha);
                createAluno(nomeDoArquivo, newAluno(a.id, a.nome, a.email, a.senha));
                break;
            case 2:
                readAllAlunos(nomeDoArquivo);
                break;
            case 3:
                printf("ID do aluno a ser atualizado: ");
                scanf("%d", &id);
                printf("Novo Nome: ");
                scanf(" %[^\n]", a.nome);
                printf("Novo Email: ");
                scanf(" %[^\n]", a.email);
                printf("Nova Senha: ");
                scanf(" %[^\n]", a.senha);
                a.id = id;
                updateAluno(nomeDoArquivo, a);
                break;
            case 4:
                printf("ID do aluno a ser deletado: ");
                scanf("%d", &id);
                deleteAluno(nomeDoArquivo, id);
                break;
            case 5:
                executar_testes_ordenacao();
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (op != 0);

    return 0;

}