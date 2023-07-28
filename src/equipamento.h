#ifndef EQUIPAMENTO_H
#define EQUIPAMENTO_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>

typedef struct {
    int num_serie;
    char nome[50];
    char mac[18];
} TEquip;

void menuInicialEstoque(FILE *equip);

void menuCriarBaseEquipamentos(FILE *equip);

void criarBaseEquipamentosOrdenada(FILE *equip);

void criarBaseEquipamentosDesordenada(FILE *equip);

void salvaEquipamento(TEquip *f1, FILE *equip);

void gerarNomeEquipamento(char *nomeequip, int i);

void gerarMAC(char *mac, int n);

void criarBaseEquipamentosDesordenada(FILE *equip);

TEquip *equipamento(int num_serie, char *nome, char *mac);

TEquip *leEquip(FILE *in);

void listarEquipamentos(FILE *in);

void imprime_equip(TEquip *e);

void menuBuscaEquipamento(FILE *equip);

TEquip* busca_sequencial_equipamentos(int cod, FILE* arq, int tam);

int tamanho_total_equipamento(FILE *in);

int tamanho_equipamento_bytes();

void menuOrdenacaoEquipamentos(FILE *equip);

TEquip *busca_binaria_equipamento(int cod, FILE *arq, int tam);

void menuOrdenarEquipamento(FILE *equip);

void menu_ordenacao_disco_equipamento(FILE *equip);

void insertion_sort_disco_equipamento(FILE *arq);

void selection_sort_discos_equipamento(FILE *arq);

void criar_particoes_equip(FILE *arq);

void selececao_por_substituicao_equip(TEquip *arr, int size);

void intercalacao_otima_equip(FILE *arq, int qnt_part, const char *nome_base, float time_used);

void copiar_arquivo_equip(FILE *arq_destino, const char *nome_arquivo_origem);

#endif //EQUIPAMENTO_H
