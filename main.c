//Camille Silva Oliveira 23.1.8120
//Mateus Rodrigues 19.2.8177


#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "utils.h"
#include "entidades.h"
#include "aluno.h"
#include "coach.h"
#include "treino.h"
#include "performance.h"
#include "crossfit.h"
#include "notificacao.h"
#include "ordenacao.h"
#include "hash_aluno.h" // Essencial para usar a tabela hash
#include <time.h>
#include <limits.h>
#include "testes.h"

#define TAMANHO_MEMORIA 100

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
    printf("3. Buscar Aluno (Tabela Hash)\n");
    printf("4. Buscar Aluno (Sequencial - Comparativo)\n");
    printf("5. Cadastrar Coach\n");
    printf("6. Listar Coaches\n");
    printf("7. Agendar Treino\n");
    printf("8. Inscrever Aluno em Treino\n");
    printf("9. Listar Treinos\n");
    printf("10. Registrar Performance de Aluno\n");
    printf("11. Consultar Historico de Aluno\n");
    printf("12. Gerenciar Unidade CrossFit\n");
    printf("13. Exibir Dados da Unidade\n");
    printf("14. Gerar Notificacoes\n");
    printf("15. Exibir Minhas Notificacoes\n");
    printf("16. Busca Avancada\n");
    printf("17. Ordenar Base de Alunos\n");
    printf("18. Embaralhar Base de Alunos (Teste)\n");
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
            //case 4: consultar_alunos_por_frequencia(); break;
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
        fwrite(&a, sizeof(Aluno), 1, f);
    }
    fclose(f);
}

void criarArquivoDeTeste(const char* nomeArquivo, int numRegistros);
void copiarArquivo(const char* origem, const char* destino);
void seuSelectionSort(const char* nomeArquivo);


int main() {
    setlocale(LC_ALL, "Portuguese_Brazil.1252");

    //Inicialização da Tabela Hash
    HashTable* ht_alunos = criar_hash_table();
    carregar_alunos_para_hash_table(ht_alunos);
    
    int opcao;
    int id_busca;
    Aluno* aluno_resultado_hash; // Ponteiro para o resultado da busca hash
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
                // A função de cadastro precisa ser ajustada para inserir na hash table também
                cadastrar_aluno(ht_alunos); 
                break;
            case 2: listar_alunos(); break;
            case 3: 
                printf("Digite o ID do aluno (Busca Hash): ");
                scanf("%d", &id_busca);
                aluno_resultado_hash = buscar_aluno_hash(ht_alunos, id_busca);
                if (aluno_resultado_hash != NULL) {
                    printf("Aluno encontrado: %s\n", aluno_resultado_hash->nome);
                    LARGE_INTEGER frequency;
                    QueryPerformanceFrequency(&frequency);
                    
                    // Calcula o tempo em microssegundos (multiplicando por 1.000.000)
                    double tempo_hash_micros = ((double)ht_alunos->ultimo_tempo_busca.QuadPart * 1000000.0) / frequency.QuadPart;
                    
                    printf("Tempo da busca hash: %.2f microssegundos\n", tempo_hash_micros);
                    // Também mostre em segundos para comparação
                    printf("(equivalente a %f segundos)\n", tempo_hash_micros / 1000000.0);
                } else {
                    printf("Aluno nao encontrado.\n");
                }
                break;
            case 4: 
                printf("Digite o ID do aluno (Busca Sequencial): ");
                scanf("%d", &id_busca);
                aluno_resultado = buscar_aluno_sequencial(id_busca);
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
            case 17: ordenar_base_alunos(); break;
            case 18: embaralhar_base_alunos(); break;
            case 0: 
                printf("Saindo do sistema...\n"); 
                destruir_hash_table(ht_alunos); // Liberar memória da hash table
                break;
            default: printf("Opcao invalida! Tente novamente.\n"); break;
        }
    } while (opcao != 0);

    return 0;
}

