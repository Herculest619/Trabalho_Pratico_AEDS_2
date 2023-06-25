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
} Tclien;

void menuInicialCliente(FILE *clien);

void menuCriarBaseCliente(FILE *clien);

void criarBaseClientesOrdenada(FILE *clien);

Tclien *cliente(int cod, char *nome, char *cpf, char *data, double mensalidade);

void salva_cliente(Tclien *clien, FILE *clien_file);

double gerarMensalidade(int i);

#endif // CLIENTE_H