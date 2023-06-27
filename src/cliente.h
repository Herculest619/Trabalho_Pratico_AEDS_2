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

Tclien *cliente(int cod, char *nome, char *cpf, char *data, double mensalidade);

void criarBaseClientesOrdenada(FILE *clien);

void criarBaseClientesDesordenada(FILE *clien);

void salva_cliente(Tclien *clien, FILE *clien_file);

double gerarMensalidade(int i);

void imprime_cliente(Tclien *clien);

void le_cliente(FILE *in);

Tclien *ler(FILE *in);

void menuBuscaCliente(FILE *out);

Tclien* busca_sequencial_cliente(int cod, FILE* arq, int tam);

Tclien* busca_binaria_cliente(int cod, FILE *arq, int tam);

int tamanho_total_cliente(FILE *in);

void menuOrdenarCliente(FILE *out);

void menuOrdenarCliente(FILE *out);

void insertion_sort_disco_cliente(FILE *arq);

void selection_sort_disco_cliente(FILE *arq);

#endif // CLIENTE_H
