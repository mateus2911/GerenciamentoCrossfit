#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "models/aluno.h"
#include "ordenacao.h"

// --- FASE 1: SELEÇÃO COM SUBSTITUIÇÃO ---

// Função auxiliar para encontrar o menor registro não congelado na memória
int encontrar_pos_menor(Aluno* memoria, int* congelado, int tam_memoria) {
    int pos_menor = -1;
    for (int i = 0; i < tam_memoria; i++) {
        if (memoria[i].id != -1 && !congelado[i]) {
            if (pos_menor == -1 || memoria[i].id < memoria[pos_menor].id) {
                pos_menor = i;
            }
        }
    }
    return pos_menor;
}


// Implementa a geração de partições ordenadas
int selecao_com_substituicao(const char* arq_entrada, int tam_memoria) {
    FILE* f_in = fopen(arq_entrada, "rb");
    if (!f_in) {
        perror("Erro ao abrir arquivo de entrada para ordenacao");
        return 0;
    }

    Aluno* memoria = (Aluno*) malloc(sizeof(Aluno) * tam_memoria);
    int* congelado = (int*) calloc(tam_memoria, sizeof(int));
    
    // Inicializa a memória com um ID inválido
    for (int i = 0; i < tam_memoria; i++) memoria[i].id = -1;

    fseek(f_in, sizeof(int), SEEK_SET); // Pula o cabeçalho do arquivo
    
    int num_particoes = 0;
    int registros_lidos_total = 0;
    
    while(fgetc(f_in) != EOF) {
        fseek(f_in, -1, SEEK_CUR); // Volta um byte
        
        // Carrega a memória
        for (int i = 0; i < tam_memoria; i++) {
            if (memoria[i].id == -1) {
                if(fread(&memoria[i], sizeof(Aluno), 1, f_in) == 1){
                     registros_lidos_total++;
                } else {
                    break; // Fim do arquivo
                }
            }
        }

        num_particoes++;
        char nome_particao[30];
        sprintf(nome_particao, "particao_%d.tmp", num_particoes);
        FILE* f_p = fopen(nome_particao, "wb");

        for(int i = 0; i < tam_memoria; i++) congelado[i] = 0;
        int ultimo_id_escrito = -1;

        while(1) {
            int pos_menor = encontrar_pos_menor(memoria, congelado, tam_memoria);
            
            if (pos_menor == -1) break; // Todos na memória estão congelados

            fwrite(&memoria[pos_menor], sizeof(Aluno), 1, f_p);
            ultimo_id_escrito = memoria[pos_menor].id;

            // Substitui na memória
            if (fread(&memoria[pos_menor], sizeof(Aluno), 1, f_in) != 1) {
                memoria[pos_menor].id = -1; // Marca como vazio
            } else {
                 registros_lidos_total++;
                if (memoria[pos_menor].id < ultimo_id_escrito) {
                    congelado[pos_menor] = 1; // Congela
                }
            }
        }
        fclose(f_p);
    }
    
    fclose(f_in);
    free(memoria);
    free(congelado);
    return num_particoes;
}


// --- FASE 2: INTERCALAÇÃO ÓTIMA ---
void intercalacao_otima(int num_particoes, const char* arq_saida, int num_caminhos) {
    if (num_particoes == 0) return;

    char nome_final[100];
    strcpy(nome_final, arq_saida);

    FILE** leitores = (FILE**) malloc(sizeof(FILE*) * (num_caminhos - 1));
    Aluno* buffer = (Aluno*) malloc(sizeof(Aluno) * (num_caminhos - 1));
    
    int total_registros = 0;
    int particao_atual_idx = 1;
    
    // Abre a primeira partição de saída
    FILE* f_out = fopen(nome_final, "wb");
    
    // Intercala as primeiras F-1 partições
    int k_inicial = 0;
    for (int i = 0; i < num_caminhos - 1 && particao_atual_idx <= num_particoes; i++) {
        char nome_particao[30];
        sprintf(nome_particao, "particao_%d.tmp", particao_atual_idx++);
        leitores[i] = fopen(nome_particao, "rb");
        if (leitores[i] && fread(&buffer[i], sizeof(Aluno), 1, leitores[i]) == 1) {
            k_inicial++;
        } else {
            leitores[i] = NULL;
        }
    }
    
    while(k_inicial > 0) {
        int pos_menor = -1;
        for (int i = 0; i < num_caminhos - 1; i++) {
            if (leitores[i] != NULL) {
                if (pos_menor == -1 || buffer[i].id < buffer[pos_menor].id) {
                    pos_menor = i;
                }
            }
        }
        
        fwrite(&buffer[pos_menor], sizeof(Aluno), 1, f_out);
        total_registros++;
        
        if (fread(&buffer[pos_menor], sizeof(Aluno), 1, leitores[pos_menor]) != 1) {
            fclose(leitores[pos_menor]);
            leitores[pos_menor] = NULL;
            k_inicial--;
        }
    }
    
    fclose(f_out);

    // Escreve o cabeçalho no arquivo final
    f_out = fopen(nome_final, "r+b");
    fseek(f_out, 0, SEEK_SET);
    fwrite(&total_registros, sizeof(int), 1, f_out);
    fclose(f_out);

    // Limpeza
    for (int i = 1; i <= num_particoes; i++) {
        char nome_particao[30];
        sprintf(nome_particao, "particao_%d.tmp", i);
        remove(nome_particao);
    }
    
    free(leitores);
    free(buffer);
}


// Função principal que orquestra a Ordenação Externa
void ordenacao_externa(const char* nome_arquivo_entrada, const char* nome_arquivo_saida, int tam_memoria, int num_caminhos) {
    // FASE 1
    int num_particoes = selecao_com_substituicao(nome_arquivo_entrada, tam_memoria);

    // FASE 2
    intercalacao_otima(num_particoes, nome_arquivo_saida, num_caminhos);
}