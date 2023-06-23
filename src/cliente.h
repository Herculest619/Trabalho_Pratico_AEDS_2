#ifndef CLIENTE_H
#define CLIENTE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>

typedef struct {
    int cod;
    char nome[50];
    char cpf[15];
    char data_nascimento[11];
    double mensalidade;
    char plano[20];
} cliente;

void menuInicialCliente(FILE *clien);

#endif // CLIENTE_H