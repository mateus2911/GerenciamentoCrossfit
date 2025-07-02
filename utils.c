
//Camille Silva Oliveira 23.1.8120

#include <stdio.h>
#include "utils.h"

void limpar_buffer_entrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}