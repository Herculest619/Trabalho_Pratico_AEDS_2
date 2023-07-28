#ifndef FUNCIONARIO_H
#define FUNCIONARIO_H

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
    double salario;
} TFunc;

// Imprime funcionario
void imprime(TFunc *func);

// Cria funcionario. Lembrar de usar free(funcionario)
TFunc *funcionario(int cod, char *nome, char *cpf, char *data_nascimento, double salario);

// salva_funcionario funcionario no arquivo out, na posicao atual do cursor
void salva_funcionario(TFunc *func, FILE *out);

// Le um funcionario do arquivo in na posicao atual do cursor
// Retorna um ponteiro para funcionario lido do arquivo
TFunc *le(FILE *in);

// Retorna tamanho do funcionario em bytes
int tamanho();

TFunc* busca_binaria(int cod, FILE *arq, int tam);

void insere_n_funcionarios_ordenados(FILE *out);

void insere_n_funcionarios_desordenados(FILE *out);

double gerarSalario(int i);

void gerarNome(char *nome, int i);

void gerarData(char *data, int i);

void gerarCPF(char *cpf, int n);

void menuRH(FILE *out);

void menuCriarBaseFuncioarios(FILE *out);

void shuffle(int *array, int size);

void insere_5_funcionarios(FILE *out);

void le_funcionarios(FILE *in);

void le_segundo_funcionario(FILE *in);

void adiciona_funcionario(FILE *in);

void sobrescreve_quarto_funcionario(FILE *in);

TFunc* busca_sequencial_funcionario(int cod, FILE* arq, int tam);

int tamanho_total_funcionario(FILE *in);

void insertion_sort_disco_funcionario(FILE *arq);

void selection_sort_disco_funcionario(FILE *arq);

void menu_ordenacao_funcionario(FILE *out);

void menu_ordenacao_funcionario_disco(FILE *out);

void menu_ordenacao_ram_funcionario(FILE *out);

void insertion_sort_memoria_funcionario(FILE *arq);

void menuInicialFuncionario(FILE *out);

void selececao_por_substituicao_func(TFunc *arr, int size);

void criar_particoes_func(FILE *out);

void intercalacao_otima_func(FILE *arq, int qnt_part, const char *nome_base, float time_used);

void copiar_arquivo_func(FILE *arq_destino, const char *nome_arquivo_origem);

#endif //FUNCIONARIO_H
