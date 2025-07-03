
//Camille Silva Oliveira 23.1.8120

#ifndef COACH_H
#define COACH_H

#include "entidades.h"

// Protótipos das funções de gerenciamento de coaches
void cadastrar_coach();
Coach buscar_coach_sequencial(int id);
Coach buscar_coach_binaria(int id);
void listar_coaches();
int obter_total_coaches();

#endif // COACH_H