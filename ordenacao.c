#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "ordenacao.h"
#include "aluno.h"
#include "utils.h" // Incluído para a função limpar_buffer_entrada()
#include "entidades.h" // Adicionado para ARQUIVO_ALUNOS

#define TAM_MEMORIA 10 // Define o tamanho da memória principal para a ordenação. Pode ser ajustado.
#define ARVORE_FILE "arvore_vencedores.bin" // Arquivo temporário para a árvore

// Protótipos de funções auxiliares estáticas (visíveis apenas neste arquivo)
static char** selecao_com_substituicao(int* numParticoes);
static void intercalar_com_arvore_vencedores(const char* arquivos[], int numParticoes);
static FILE* construir_arvore_vencedores(No* folhas, int n);
static void atualizar_arvore(FILE* arquivoArvore, int n, int indiceFolha, Aluno novo);
static void excluir_particoes(const char** arquivos, int numParticoes);

// Função principal que agora apresenta um menu de escolha
void ordenar_base_alunos() {
    int opcao;
    printf("\n--- Menu de Ordenacao ---\n");
    printf("Escolha o metodo para ordenar a base de alunos:\n");
    printf("1. Ordenacao Externa (Selecao por Substituicao + Arvore de Vencedores) - Recomendado\n");
    printf("2. Bubble Sort (Ineficiente para arquivos grandes)\n");
    printf("0. Cancelar\n");
    printf("Opcao: ");

    if (scanf("%d", &opcao) != 1) {
        printf("Entrada invalida. Operacao cancelada.\n");
        limpar_buffer_entrada();
        return;
    }
    limpar_buffer_entrada();

    switch (opcao) {
        case 1: {
            int numParticoes = 0;
            printf("\nIniciando Ordenacao Externa...\n");

            printf("1. Gerando particoes ordenadas...\n");
            char** nomesParticoes = selecao_com_substituicao(&numParticoes);

            if (nomesParticoes == NULL) {
                printf("Nenhuma particao foi gerada. O arquivo pode ja estar ordenado, vazio ou ocorreu um erro.\n");
                return;
            }
            if (numParticoes == 0) {
                 printf("Arquivo ja esta ordenado ou vazio.\n");
                 free(nomesParticoes);
                 return;
            }


            printf(" -> %d particoes geradas com sucesso.\n", numParticoes);
            printf("2. Intercalando particoes com Arvore de Vencedores...\n");
            intercalar_com_arvore_vencedores((const char**)nomesParticoes, numParticoes);

            printf("3. Limpando arquivos temporarios...\n");
            excluir_particoes((const char**)nomesParticoes, numParticoes);

            printf("\nOrdenacao Externa concluida com sucesso!\n");
            break;
        }
        case 2:
            printf("\nIniciando Bubble Sort...\n");
            bubble_sort_alunos();
            break;
        case 0:
            printf("Operacao de ordenacao cancelada.\n");
            break;
        default:
            printf("Opcao invalida! Tente novamente.\n");
            break;
    }
}

// Implementação da Seleção com Substituição para criar as partições ordenadas.
// (Esta função permanece a mesma da versão anterior)
static char** selecao_com_substituicao(int* numParticoes) {
    FILE* arquivoPrincipal = fopen(ARQUIVO_ALUNOS, "rb");
    if (!arquivoPrincipal) {
        perror("Erro ao abrir o arquivo de alunos");
        return NULL;
    }

    fseek(arquivoPrincipal, 0, SEEK_END);
    long tamanhoArquivo = ftell(arquivoPrincipal);
    if (tamanhoArquivo < sizeof(Aluno)) {
        fclose(arquivoPrincipal);
        *numParticoes = 0;
        return NULL;
    }
    rewind(arquivoPrincipal);

    RegistroMemoria* memoria = (RegistroMemoria*)malloc(TAM_MEMORIA * sizeof(RegistroMemoria));
    char** nomesParticoes = (char**)malloc(10 * sizeof(char*));
    if (!memoria || !nomesParticoes) {
        perror("Falha ao alocar memória inicial");
        if(memoria) free(memoria);
        if(nomesParticoes) free(nomesParticoes);
        fclose(arquivoPrincipal);
        return NULL;
    }
    int capacidadeParticoes = 10;

    int registrosCarregados = 0;
    for (int i = 0; i < TAM_MEMORIA; ++i) {
        if (fread(&memoria[i].aluno, sizeof(Aluno), 1, arquivoPrincipal) == 1) {
            memoria[i].congelado = false;
            registrosCarregados++;
        } else {
            break;
        }
    }

    int particaoAtual = 0;
    while (registrosCarregados > 0) {
        if (particaoAtual >= capacidadeParticoes) {
            capacidadeParticoes *= 2;
            char** temp = (char**)realloc(nomesParticoes, capacidadeParticoes * sizeof(char*));
            if (!temp) {
                perror("Falha ao realocar nomes de partições");
                // Cleanup
                return NULL;
            }
            nomesParticoes = temp;
        }

        char nomeParticao[30];
        sprintf(nomeParticao, "particao_%d.bin", particaoAtual);
        FILE* pFile = fopen(nomeParticao, "wb");
        if (!pFile) {
            perror("Erro ao criar arquivo de partição");
            // Cleanup
            return NULL;
        }

        nomesParticoes[particaoAtual] = strdup(nomeParticao);
        int ultimoId = -1;

        while (true) {
            int menorIndice = -1;
            int menorId = INT_MAX;

            for (int i = 0; i < registrosCarregados; ++i) {
                if (!memoria[i].congelado && memoria[i].aluno.id < menorId) {
                    menorId = memoria[i].aluno.id;
                    menorIndice = i;
                }
            }

            if (menorIndice == -1) break;

            fwrite(&memoria[menorIndice].aluno, sizeof(Aluno), 1, pFile);
            ultimoId = memoria[menorIndice].aluno.id;

            Aluno novoAluno;
            if (fread(&novoAluno, sizeof(Aluno), 1, arquivoPrincipal) == 1) {
                memoria[menorIndice].aluno = novoAluno;
                memoria[menorIndice].congelado = (novoAluno.id < ultimoId);
            } else {
                registrosCarregados--;
                memoria[menorIndice] = memoria[registrosCarregados];
            }
        }
        fclose(pFile);
        particaoAtual++;

        for (int i = 0; i < registrosCarregados; ++i) {
            memoria[i].congelado = false;
        }
    }

    *numParticoes = particaoAtual;
    free(memoria);
    fclose(arquivoPrincipal);
    return nomesParticoes;
}


// --- IMPLEMENTAÇÃO DA ÁRVORE DE VENCEDORES ---

static FILE* construir_arvore_vencedores(No* folhas, int n) {
    FILE* arquivoArvore = fopen(ARVORE_FILE, "wb+");
    if (arquivoArvore == NULL) {
        perror("Erro ao criar arquivo da árvore de vencedores");
        return NULL;
    }

    // Escreve os nós folhas (nível mais baixo) na segunda metade do arquivo
    fseek(arquivoArvore, (n - 1) * sizeof(No), SEEK_SET);
    fwrite(folhas, sizeof(No), n, arquivoArvore);

    // Constrói os nós internos
    for (int i = n - 2; i >= 0; i--) {
        int filho_esq_idx = 2 * i + 1;
        int filho_dir_idx = 2 * i + 2;

        No noEsq, noDir;
        fseek(arquivoArvore, filho_esq_idx * sizeof(No), SEEK_SET);
        fread(&noEsq, sizeof(No), 1, arquivoArvore);

        // Define um valor máximo para o nó direito se ele não existir
        noDir.aluno.id = INT_MAX;
        noDir.indiceParticao = -1;
        if (filho_dir_idx < (2 * n - 1)) {
            fseek(arquivoArvore, filho_dir_idx * sizeof(No), SEEK_SET);
            fread(&noDir, sizeof(No), 1, arquivoArvore);
        }

        No vencedor = (noEsq.aluno.id <= noDir.aluno.id) ? noEsq : noDir;
        fseek(arquivoArvore, i * sizeof(No), SEEK_SET);
        fwrite(&vencedor, sizeof(No), 1, arquivoArvore);
    }
    return arquivoArvore;
}

static void atualizar_arvore(FILE* arquivoArvore, int n, int indiceFolha, Aluno novo) {
    int pos = (n - 1) + indiceFolha; // Posição do nó folha no arquivo
    No noAtualizado = {novo, indiceFolha};
    fseek(arquivoArvore, pos * sizeof(No), SEEK_SET);
    fwrite(&noAtualizado, sizeof(No), 1, arquivoArvore);

    // Sobe na árvore atualizando os pais
    while (pos > 0) {
        int pai_idx = (pos - 1) / 2;
        int filho_esq_idx = 2 * pai_idx + 1;
        int filho_dir_idx = 2 * pai_idx + 2;

        No noEsq, noDir, vencedor;
        fseek(arquivoArvore, filho_esq_idx * sizeof(No), SEEK_SET);
        fread(&noEsq, sizeof(No), 1, arquivoArvore);
        
        noDir.aluno.id = INT_MAX; // Valor sentinela
        if(filho_dir_idx < (2 * n -1)){
             fseek(arquivoArvore, filho_dir_idx * sizeof(No), SEEK_SET);
             fread(&noDir, sizeof(No), 1, arquivoArvore);
        }

        vencedor = (noEsq.aluno.id <= noDir.aluno.id) ? noEsq : noDir;
        fseek(arquivoArvore, pai_idx * sizeof(No), SEEK_SET);
        fwrite(&vencedor, sizeof(No), 1, arquivoArvore);

        pos = pai_idx;
    }
}

static void intercalar_com_arvore_vencedores(const char* arquivos[], int numParticoes) {
    if (numParticoes == 0) return;

    FILE* saida = fopen(ARQUIVO_ALUNOS, "wb");
    FILE** particoes = (FILE**)malloc(numParticoes * sizeof(FILE*));
    No* folhas = (No*)malloc(numParticoes * sizeof(No));
    if (!saida || !particoes || !folhas) {
        perror("Erro de alocação ou abertura de arquivo na intercalação");
        // Cleanup
        return;
    }

    int particoesAtivas = 0;
    for (int i = 0; i < numParticoes; i++) {
        particoes[i] = fopen(arquivos[i], "rb");
        if (particoes[i] && fread(&folhas[i].aluno, sizeof(Aluno), 1, particoes[i]) == 1) {
            folhas[i].indiceParticao = i;
            particoesAtivas++;
        } else { // Partição vazia ou erro na leitura
            folhas[i].aluno.id = INT_MAX;
            folhas[i].indiceParticao = i;
            if(particoes[i]) fclose(particoes[i]);
        }
    }

    if (particoesAtivas == 0) {
        // ... Lidar com este caso
        return;
    }

    FILE* arvore = construir_arvore_vencedores(folhas, numParticoes);
    if (!arvore) {
        // ... Lidar com erro
        return;
    }

    while (particoesAtivas > 0) {
        No raiz;
        rewind(arvore);
        fread(&raiz, sizeof(No), 1, arvore);

        if (raiz.aluno.id == INT_MAX) break; // Fim da intercalação

        fwrite(&raiz.aluno, sizeof(Aluno), 1, saida);
        
        int indiceParticaoVencedora = raiz.indiceParticao;
        Aluno proximo;
        if (fread(&proximo, sizeof(Aluno), 1, particoes[indiceParticaoVencedora]) != 1) {
            proximo.id = INT_MAX; // Fim da partição
            particoesAtivas--;
        }

        atualizar_arvore(arvore, numParticoes, indiceParticaoVencedora, proximo);
    }

    fclose(arvore);
    remove(ARVORE_FILE);
    for (int i = 0; i < numParticoes; i++) {
        if (particoes[i]) fclose(particoes[i]);
    }
    fclose(saida);
    free(particoes);
    free(folhas);
}

// --- FIM DA IMPLEMENTAÇÃO DA ÁRVORE DE VENCEDORES ---

static void excluir_particoes(const char** arquivos, int numParticoes) {
    for (int i = 0; i < numParticoes; i++) {
        remove(arquivos[i]);
        free((void*)arquivos[i]);
    }
    free(arquivos);
}

// Implementação do Bubble Sort (sem alterações)
int bubble_sort_alunos() {
    FILE *f = fopen(ARQUIVO_ALUNOS, "r+b");
    if (!f) {
        perror("Erro ao abrir arquivo para Bubble Sort");
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long num_alunos = ftell(f) / sizeof(Aluno);
    fclose(f);

    if (num_alunos <= 1) {
        printf("Arquivo com 1 ou menos alunos. Nao e necessario ordenar.\n");
        return 0;
    }

    // Reabre para leitura e escrita
    f = fopen(ARQUIVO_ALUNOS, "r+b");

    Aluno aluno_i, aluno_j;
    for (long i = 0; i < num_alunos - 1; i++) {
        for (long j = 0; j < num_alunos - i - 1; j++) {
            fseek(f, j * sizeof(Aluno), SEEK_SET);
            fread(&aluno_i, sizeof(Aluno), 1, f);
            fread(&aluno_j, sizeof(Aluno), 1, f);

            if (aluno_i.id > aluno_j.id) {
                fseek(f, j * sizeof(Aluno), SEEK_SET);
                fwrite(&aluno_j, sizeof(Aluno), 1, f);
                fwrite(&aluno_i, sizeof(Aluno), 1, f);
            }
        }
    }

    fclose(f);
    printf("Arquivo de alunos ordenado com Bubble Sort.\n");
    return 0;
}