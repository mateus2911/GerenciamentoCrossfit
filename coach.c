
//Camille Silva Oliveira 23.1.8120

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "coach.h"
#include "utils.h"

/**
 * @brief Calcula o número total de registros de coaches no arquivo.
 * @return O número total de coaches cadastrados.
 */
int obter_total_coaches() {
    FILE *file = fopen(ARQUIVO_COACHES, "rb");
    if (file == NULL) {
        return 0; // Se o arquivo não existe, não há coaches.
    }
    fseek(file, 0, SEEK_END);
    int total = ftell(file) / sizeof(Coach);
    fclose(file);
    return total;
}

/**
 * @brief Cadastra um novo coach na base de dados.
 * A função solicita o nome e a especialidade e salva o novo registro no final do arquivo `coaches.bin`.
 * [cite_start]Os atributos são baseados na definição da entidade Coach. [cite: 10]
 */
void cadastrar_coach() {
    FILE *file = fopen(ARQUIVO_COACHES, "ab");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo de coaches");
        return;
    }

    Coach novo_coach;
    novo_coach.id = obter_total_coaches(); // ID único sequencial
    novo_coach.ativo = 1;

    printf("Digite o nome do coach: ");
    limpar_buffer_entrada(); // Limpa o buffer antes de ler a string
    scanf("%[^\n]", novo_coach.nome);

    printf("Digite a especialidade do coach (ex: forca, LPO, ginastica): ");
    limpar_buffer_entrada();
    scanf("%[^\n]", novo_coach.especialidade);

    fwrite(&novo_coach, sizeof(Coach), 1, file);
    fclose(file);
    printf("\nCoach com ID %d cadastrado com sucesso!\n", novo_coach.id);
}

/**
 * @brief Busca um coach pelo seu ID usando uma varredura sequencial no arquivo.
 * @param id O ID do coach a ser procurado.
 * @return Retorna a struct Coach se encontrado, ou uma struct com id = -1 se não encontrado.
 */
Coach buscar_coach_sequencial(int id) {
    FILE *file = fopen(ARQUIVO_COACHES, "rb");
    Coach coach;
    coach.id = -1; // Valor padrão para "não encontrado"

    if (file == NULL) {
        return coach; // Retorna "não encontrado" se o arquivo não puder ser aberto
    }

    // Lê o arquivo registro por registro
    while (fread(&coach, sizeof(Coach), 1, file)) {
        if (coach.id == id && coach.ativo) {
            fclose(file);
            return coach; // Retorna o coach encontrado
        }
    }

    fclose(file);
    coach.id = -1; // Garante que o ID é -1 se o loop terminar sem encontrar
    return coach;
}

/**
 * @brief Lista todos os coaches ativos cadastrados na base de dados.
 */
void listar_coaches() {
    FILE *file = fopen(ARQUIVO_COACHES, "rb");
    if (file == NULL) {
        printf("\nNenhum coach cadastrado.\n");
        return;
    }

    Coach coach;
    printf("\n--- Lista de Coaches ---\n");
    printf("%-5s | %-30s | %-20s\n", "ID", "Nome", "Especialidade");
    printf("-------------------------------------------------------------\n");
    
    while (fread(&coach, sizeof(Coach), 1, file)) {
        if (coach.ativo) {
            printf("%-5d | %-30s | %-20s\n", coach.id, coach.nome, coach.especialidade);
        }
    }
    printf("-------------------------------------------------------------\n");
    fclose(file);
}