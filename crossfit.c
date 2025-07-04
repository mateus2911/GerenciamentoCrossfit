//Camille Silva Oliveira 23.1.8120
//Mateus Rodrigues 19.2.8177

#include <stdio.h>
#include <stdlib.h>
#include "crossfit.h"
#include "utils.h"

/**
 * @brief Permite cadastrar ou atualizar os dados da unidade central CrossFit.
 * A função sempre trabalha com o primeiro (e único) registro do arquivo.
 */
void gerenciar_unidade_crossfit() {
    CrossFit unidade;

    printf("\n--- Gerenciar Dados da Unidade CrossFit ---\n");
    
    unidade.id = 0; // ID fixo para a unidade central

    printf("Digite o nome da academia: ");
    limpar_buffer_entrada();
    scanf("%[^\n]", unidade.nome);
    
    printf("Digite o endereco da academia: ");
    limpar_buffer_entrada();
    scanf("%[^\n]", unidade.endereco);

    // Abre o arquivo em modo "wb" para sempre sobrescrever ou criar o arquivo.
    // Isso garante que haverá apenas um registro de unidade.
    FILE *file = fopen(ARQUIVO_CROSSFIT, "wb");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo da unidade CrossFit");
        return;
    }

    fwrite(&unidade, sizeof(CrossFit), 1, file);
    fclose(file);

    printf("\nDados da unidade CrossFit salvos com sucesso!\n");
}

/**
 * @brief Exibe os dados da unidade CrossFit cadastrada.
 */
void exibir_dados_unidade() {
    CrossFit unidade;

    FILE *file = fopen(ARQUIVO_CROSSFIT, "rb");
    if (file == NULL) {
        printf("\nNenhuma unidade CrossFit cadastrada ainda.\n");
        printf("Use a opcao de gerenciamento para cadastrar a unidade.\n");
        return;
    }

    // Lê o primeiro (e único) registro do arquivo.
    if (fread(&unidade, sizeof(CrossFit), 1, file) != 1) {
        printf("\nNao foi possível ler os dados da unidade. O arquivo pode estar vazio ou corrompido.\n");
    } else {
        printf("\n--- Dados da Unidade CrossFit ---\n");
        printf("Nome: %s\n", unidade.nome);
        printf("Endereco: %s\n", unidade.endereco);
        printf("----------------------------------\n");
    }
    
    fclose(file);
}