#include <stdio.h>
#include <string.h>
#include "entidades.h"
#include "notificacao.h"
#include "treino.h"
#include "aluno.h"
#include "coach.h"


void criar_notificacao(int id_destinatario, int tipo_destinatario, const char* mensagem) {
    FILE *file = fopen(ARQUIVO_NOTIFICACOES, "ab");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo de notificacoes");
        return;
    }

    Notificacao n;
    n.id_destinatario = id_destinatario;
    n.tipo_destinatario = tipo_destinatario;
    strncpy(n.mensagem, mensagem, sizeof(n.mensagem) - 1);
    n.mensagem[sizeof(n.mensagem) - 1] = '\0';
    n.lida = 0;

    fwrite(&n, sizeof(Notificacao), 1, file);
    fclose(file);
}


void verificar_e_gerar_notificacoes() {
    // Limpa notificações antigas para evitar duplicatas a cada verificação
    fclose(fopen(ARQUIVO_NOTIFICACOES, "wb"));

    FILE *f_treinos = fopen(ARQUIVO_TREINOS, "rb");
    if (f_treinos == NULL) {
        printf("\nNenhum treino encontrado para gerar notificacoes.\n");
        return;
    }

    Treino t;
    printf("\nVerificando treinos para gerar notificacoes...\n");

    while (fread(&t, sizeof(Treino), 1, f_treinos)) {
        // 1. Alerta de Lotação para Coaches
        if (t.num_inscritos >= MAX_ALUNOS_TREINO) {
            char msg[200];
            sprintf(msg, "Alerta: O treino de ID %d (%s em %s) esta com lotacao maxima.", t.id, t.tipo_treino, t.data);
            criar_notificacao(t.id_coach_responsavel, 2, msg); // Tipo 2 para Coach
        }

        // 2. Lembretes para Alunos Inscritos
        for (int i = 0; i < t.num_inscritos; i++) {
            char msg[200];
            sprintf(msg, "Lembrete: Voce esta inscrito no treino de ID %d (%s em %s).", t.id, t.tipo_treino, t.data);
            criar_notificacao(t.lista_alunos_inscritos[i], 1, msg); // Tipo 1 para Aluno
        }
    }

    printf("Verificacao concluida. Novas notificacoes foram geradas.\n");
    fclose(f_treinos);
}

/**
 * @brief Exibe todas as notificações não lidas para um usuário específico.
 */
void exibir_notificacoes_pendentes(int id_usuario, int tipo_usuario) {
    FILE* file = fopen(ARQUIVO_NOTIFICACOES, "rb");
    if (file == NULL) {
        printf("\nNenhuma notificacao encontrada.\n");
        return;
    }

    Notificacao n;
    int encontradas = 0;
    char* tipo_str = (tipo_usuario == 1) ? "Aluno" : "Coach";

    printf("\n--- Notificacoes Pendentes para %s ID %d ---\n", tipo_str, id_usuario);
    while (fread(&n, sizeof(Notificacao), 1, file)) {
        if (n.id_destinatario == id_usuario && n.tipo_destinatario == tipo_usuario && !n.lida) {
            printf("- %s\n", n.mensagem);
            encontradas++;
        }
    }

    if (encontradas == 0) {
        printf("Nenhuma nova notificacao para voce.\n");
    }
     printf("--------------------------------------------------\n");

    fclose(file);
    
}