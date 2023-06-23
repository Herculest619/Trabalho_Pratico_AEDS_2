#include "funcionario.h"
#include "equipamento.h"
#include "cliente.h"
#include "funcionario.c"
#include "equipamento.c"
#include "cliente.c"
#include "main.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>
//
void menu(FILE *func, FILE *clien, FILE *equip){
    system("cls");
    printf("**********************************************\n");
    printf("----MENU----\n");
    printf("1: CLIENTE\n");
    printf("2: RH\n");
    printf("3: ESTOQUE\n");
    printf("0: EXIT\n");
    printf("**********************************************\n\n");
    printf("DIGITE A OPCAO DESEJADA: ");

    int opcao;
    scanf("%d", &opcao);

    switch (opcao)
    {
    case 1:
        system("cls");
        menuInicialCliente(clien);
        break;
    
    case 2:
        system("cls");
        menuInicialFuncionario(func);
        break;
    
    case 3:
        system("cls");
        menuInicialEstoque(equip);
        break;
    }
}

int main()
{
        //declara ponteiro para arquivo
    FILE *func, *clien, *equip;
    // abre arquivos
    //"r+b" abre arquivo binario para leitura e escrita
    if ((func = fopen("funcionario.dat", "r+b")) == NULL)
    {
        printf("Erro ao abrir arquivo de funcionario\n");
        printf("Criando arquivo de funcionario\n");
        if ((func = fopen("funcionario.dat", "w+b")) == NULL)
        {
            printf("Erro ao criar arquivo de funcionario\n");
        }
    }
    if ((clien = fopen("cliente.dat", "r+b")) == NULL)
    {
        printf("Erro ao abrir arquivo de cliente\n");
        printf("Criando arquivo de cliente\n");
        if ((clien = fopen("cliente.dat", "w+b")) == NULL)
        {
            printf("Erro ao criar arquivo de cliente\n");
        }
    }
    if ((equip = fopen("equipamento.dat", "r+b")) == NULL)
    {
        printf("Erro ao abrir arquivo de equipamento\n");
        printf("Criando arquivo de equipamento\n");
        if ((equip = fopen("equipamento.dat", "w+b")) == NULL)
        {
            printf("Erro ao criar arquivo de equipamento\n");
        }
    }
        menu(func, clien, equip);

        //fecha os arquivos
        fclose(func);
        fclose(clien);
        fclose(equip);

    return 0;
}
