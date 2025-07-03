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
#include "testes.h"

void inicializar_arquivos() {
    FILE *fx = fopen(ARQUIVO_CROSSFIT, "ab"); if(fx) fclose(fx);
    FILE *fa = fopen(ARQUIVO_ALUNOS, "ab"); if(fa) fclose(fa);
    FILE *fc = fopen(ARQUIVO_COACHES, "ab"); if(fc) fclose(fc);
    FILE *ft = fopen(ARQUIVO_TREINOS, "ab"); if(ft) fclose(ft);
    FILE *fp = fopen(ARQUIVO_PERFORMANCE, "ab"); if(fp) fclose(fp);
}

void exibir_menu() {
    printf("\n--- Menu Principal ---\n");
    printf("1. Cadastrar Aluno\n");
    printf("2. Listar Alunos\n");
    printf("3. Buscar Aluno (Sequencial)\n");
    printf("4. Buscar Aluno (Binária)\n");
    printf("5. Cadastrar Coach\n");
    printf("6. Listar Coaches\n");
    printf("7. Agendar Treino\n");
    printf("8. Inscrever Aluno em Treino\n");
    printf("9. Listar Treinos\n");
    printf("10. Registrar Performance\n");
    printf("11. Consultar Histórico de Aluno\n");
    printf("12. Gerenciar Unidade CrossFit\n");
    printf("13. Exibir Dados da Unidade\n");
    printf("0. Sair\n");
    printf("\nEscolha uma opcao: ");
}


int main() {
    setlocale(LC_ALL, "Portuguese_Brazil.1252"); // Corrige caracteres especiais
    int quantidade_alunos = 0;
    int opcao;
    int id_busca; // Declarada aqui para ser usada em múltiplos cases
    Aluno aluno_resultado; // Variável reutilizável para buscas

    inicializar_arquivos();

    do {
        exibir_menu();
        if (scanf("%d", &opcao) != 1) {
            printf("\nEntrada inválida. Por favor, digite um número.\n");
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
                else printf("Aluno não encontrado.\n");
                break;
            case 4: 
                printf("Digite o ID do aluno: ");
                scanf("%d", &id_busca);
                aluno_resultado = buscar_aluno_binaria(id_busca);
                if (aluno_resultado.id != -1) printf("Aluno encontrado: %s\n", aluno_resultado.nome);
                else printf("Aluno não encontrado.\n");
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
            case 0: printf("Saindo do sistema...\n"); break;
            default: printf("Opção inválida! Tente novamente.\n"); break;
        }
    } while (opcao != 0);

    return 0;
}