#include "cliente.h"
#include "main.h"
#include "funcionario.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>

void menuInicialCliente(FILE *clien){
    int opcao;
    printf("**********************************************\n");
    printf("----MENU CLIENTE----\n");
    printf("1: CRIAR BASE\n");
    printf("2: IMPRIMIR BASE\n");
    printf("3: BUSCA\n");
    printf("4: ORDENAR BASE\n");
    printf("9: VOLTAR\n");
    printf("0: EXIT\n");
    printf("**********************************************\n\n");
    printf("DIGITE A OPCAO DESEJADA: ");

    scanf("%d", &opcao);
    switch (opcao)
    {
    case 1:
        system("cls");
        menuCriarBaseCliente(clien);
        break;

    case 2:
        system("cls");
        //listarClientes(clien);
        break;

    case 3:
        system("cls");
        //menuBuscaCliente(clien);
        break;

    case 4:
        system("cls");
        //menuOrdenarCliente(clien);
        break;

    case 9:
        system("cls");
        main();
        break;
    }
}

void menuCriarBaseCliente(FILE *clien){
    printf("-----CRIAR BASE DE CLIENTES-----\n");
    printf("1: ORDENADA\n");
    printf("2: DESORDENADA\n");
    printf("9: VOLTAR\n");
    printf("0: EXIT\n");
    printf("---------------------\n\n");
    printf("DIGITE A OPCAO DESEJADA: ");

    clock_t inicio, fim;
    double tempo_execucao;

    int opcao;
    scanf("%d", &opcao);

    switch (opcao)
    {
    case 1:
        system("cls");
        printf("-----CRIAR BASE ORDENADA-----\n");
        inicio = clock();
        criarBaseClientesOrdenada(clien);
        fim = clock();
        tempo_execucao = (double)(fim - inicio) / CLOCKS_PER_SEC;
        printf("\nTempo para criar base ordenada: %.6f segundos\n\n", tempo_execucao);
        menuInicialCliente(clien);
        break;

    case 2:
        system("cls");
        printf("-----CRIAR BASE DESORDENADA-----\n");
        inicio = clock();
        criarBaseClientesDesordenada(clien);
        fim = clock();
        tempo_execucao = (double)(fim - inicio) / CLOCKS_PER_SEC;
        printf("\nTempo para criar base desordenada: %.6f segundos\n\n", tempo_execucao);
        menuInicialCliente(clien);
        break;

    case 9:
        system("cls");
        menuInicialCliente(clien);
        break;

    default:
        system("cls");
        printf("\n-----EXIT-----\n");
    }
}

void criarBaseClientesOrdenada(FILE *clien){
    int n;
    printf("\nDigite a quantidade de clientes a serem inseridos ordenados: ");
    scanf("%d", &n);
    printf("\nInserindo %d clientes no arquivo...\n", n);

    for (int i = 1; i <= n; i++)
    {
        srand(time(NULL));
        char cpf[15];
        gerarCPF(cpf, i);
        char data[11];
        gerarData(data, i);
        char nome[MAX_TAM_NOME];
        gerarNome(nome, i);
        Tclien *f1 = cliente(i, nome, cpf, data, gerarMensalidade(i));
        salva_cliente(f1, clien);
        free(f1);
        srand(0);
    }
    printf("\nCONCLUIDO\n");
}

Tclien *cliente(int cod, char *nome, char *cpf, char *data, double mensalidade){
    // Aloca memória para o funcionário
    Tclien *clien = (Tclien *)malloc(sizeof(Tclien));
    //inicializa espaço de memória com ZEROS
    if (clien) memset(clien, 0, sizeof(TFunc));

    clien->cod = cod;
    strcpy(clien->nome, nome);
    strcpy(clien->cpf, cpf);
    strcpy(clien->data_nascimento, data);
    clien->mensalidade = mensalidade;

    return clien;
}

void salva_cliente(Tclien *clien, FILE *clien_file){

    fwrite(&clien->cod, sizeof(int), 1, clien_file);
    fwrite(clien->nome, sizeof(char), sizeof(clien->nome), clien_file);
    fwrite(clien->cpf, sizeof(char), sizeof(clien->cpf), clien_file);
    fwrite(clien->data_nascimento, sizeof(char), sizeof(clien->data_nascimento), clien_file);
    fwrite(&clien->mensalidade, sizeof(double), 1, clien_file);
}

double gerarMensalidade(int i){
    // gerar mensalidade aleatória entre R$59,90 e R$199,90
    double mensalidade = (rand() % 14000) + 5990;
    mensalidade = mensalidade / 100;
    return mensalidade;
}

void criarBaseClientesDesordenada(FILE *clien){
    
}