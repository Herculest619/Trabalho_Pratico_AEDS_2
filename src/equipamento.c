#include "equipamento.h"
#include "funcionario.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>
#define MAX_NOMES_EQUIP 23
#define MAX_TAM_NOMES_EQUIP 50

void menuInicialEstoque(FILE *equip)
{
    int opcao;
    printf("**********************************************\n");
    printf("----MENU ESTOQUE----\n");
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
        menuCriarBaseEquipamentos(equip);
        break;

    case 2:
        system("cls");
        listarEquipamentos(equip);
        break;

    case 3:
        system("cls");
        menuBuscaEquipamento(equip);
        break;

    case 4:
        system("cls");
        menuOrdenarEquipamento(equip);
        break;

    case 9:
        system("cls");
        main();
        break;
    }
}

void menu_ordenacao_disco_equipamento(FILE *equip)
{
    printf("-----ORDENAR BASE-----\n");
    printf("1: INSERTION SORT\n");
    printf("2: SELECTION SORT\n");
    // printf("3: MERGE SORT\n");
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
        printf("ORDENANDO BASE COM INSERTION SORT....\n");
        inicio = clock();

        insertion_sort_disco_equipamento(equip);

        fim = clock();
        tempo_execucao = (double)(fim - inicio) / CLOCKS_PER_SEC;
        printf("\nTempo para ordenar com Insertion Sort: %.6f segundos\n\n", tempo_execucao);
        menuInicialEstoque(equip);
        break;

    case 2:
        system("cls");
        printf("ORDENANDO BASE COM SELECTION SORT....\n");
        inicio = clock();

        selection_sort_discos_equipamento(equip);

        fim = clock();
        tempo_execucao = (double)(fim - inicio) / CLOCKS_PER_SEC;
        printf("\nTempo para ordenar com Selection Sort: %.6f segundos\n\n", tempo_execucao);
        menuInicialEstoque(equip);
        break;

    case 9:
        system("cls");
        main();
        break;

    default:
        system("cls");
        printf("\n-----EXIT-----\n");
    }
}

void insertion_sort_disco_equipamento(FILE *arq){

    int tam = tamanho_total_equipamento(arq);
    int i, cont=0;
    //faz o insertion sort
    for (int j = 2; j <= tam; j++)
    {
        //posiciona o arquivo no registro j
        fseek(arq, (j-1) * tamanho_equipamento_bytes(), SEEK_SET);
        TEquip *fj = leEquip(arq);
        //printf("\n********* equipamento atual: %d\n", fj->cod);
        i = j - 1;
        //posiciona o cursor no registro i
        fseek(arq, (i-1) * tamanho_equipamento_bytes(), SEEK_SET);
        TEquip *fi = leEquip(arq);
        //printf("fi = %d\n", fi->cod);
        while ((i > 0) && (fi->num_serie > fj->num_serie))
        {
            //posiciona o cursor no registro i+1
            fseek(arq, i * tamanho_equipamento_bytes(), SEEK_SET);
            //printf("Salvando equipamento %d na posicao %d\n", fi->cod, i+1);
            salvaEquipamento(fi, arq);
            i = i - 1;
            //lÃª registro i
            fseek(arq, (i-1) * tamanho_equipamento_bytes(), SEEK_SET);
            fi = leEquip(arq);
            //printf("fi = %d; i = %d\n", fi->cod, i);
            cont++;
        }
        //posiciona cursor no registro i + 1
        fseek(arq, (i) * tamanho_equipamento_bytes(), SEEK_SET);
        //printf("*** Salvando equipamento %d na posicao %d\n", fj->cod, i+1);
        //salva registro j na posiÃ§Ã£o i
        salvaEquipamento(fj, arq);
    }
    printf("\nForam gastas %d iteracoes para ordenar!\n", cont);
    //descarrega o buffer para ter certeza que dados foram gravados
    fflush(arq);
}

void selection_sort_discos_equipamento(FILE *arq){
    int tam = tamanho_total_equipamento(arq);
    int i, j, min, cont = 0;
    //faz o selection sort
    for (i = 1; i < tam; i++)
    {
        //posiciona o arquivo no registro i
        fseek(arq, (i-1) * tamanho_equipamento_bytes(), SEEK_SET);
        TEquip *fi = leEquip(arq);
        //printf("\n********* equipamento atual: %d\n", fi->cod);
        min = i;
        for (j = i+1; j <= tam; j++)
        {
            //posiciona o arquivo no registro j
            fseek(arq, (j-1) * tamanho_equipamento_bytes(), SEEK_SET);
            TEquip *fj = leEquip(arq);
            //printf("fj = %d\n", fj->cod);
            if (fj->num_serie < fi->num_serie)
            {
                min = j;
                //printf("min = %d\n", min);
                //posiciona o arquivo no registro min
                fseek(arq, (min-1) * tamanho_equipamento_bytes(), SEEK_SET);
                TEquip *fmin = leEquip(arq);
                //printf("fmin = %d\n", fmin->cod);
                //troca os registros
                fseek(arq, (i-1) * tamanho_equipamento_bytes(), SEEK_SET);
                salvaEquipamento(fmin, arq);
                fseek(arq, (min-1) * tamanho_equipamento_bytes(), SEEK_SET);
                salvaEquipamento(fi, arq);
                //posiciona o arquivo no registro i
                fseek(arq, (i-1) * tamanho_equipamento_bytes(), SEEK_SET);
                fi = leEquip(arq);
                //printf("fi = %d\n", fi->cod);
                cont++;
            }
            free(fj);
        }
        free(fi);
    }
    printf("\nTotal de iteracoes: %d\n", cont);
}

void menuOrdenarEquipamento(FILE *equip){
    printf("-----ORDENAR BASE-----\n");
    printf("1: ORDENACAO EM DISCO\n");
    // printf("2: ORDENACAO EM RAM\n");
    printf("9: VOLTAR\n");
    printf("0: EXIT\n");
    printf("---------------------\n\n");
    printf("DIGITE A OPCAO DESEJADA: ");

    int opcao;
    scanf("%d", &opcao);

    switch (opcao)
    {
    case 1:
        system("cls");

        menu_ordenacao_disco_equipamento(equip);

        break;

        /*case 2:

            system("cls");
            printf("ORDENANDO BASE EM RAM USANDO CLASSIFICACAO INTERNA....\n");

            classificacao_interna(out);

            menuInicial(out);
            break;*/

    case 9:
        system("cls");
        menuInicialEstoque(equip);
        break;

    default:
        system("cls");
        printf("\n-----EXIT-----\n");
    }
}

void menuBuscaEquipamento(FILE *equip)
{

    system("cls");
    printf("**********************************************\n");
    printf("----MENU----\n");
    printf("1: BUSCA SEQUENCIAL\n");
    printf("2: BUSCA BINARIA\n");
    printf("9: VOLTAR\n");
    printf("0: EXIT\n");
    printf("**********************************************\n\n");
    printf("DIGITE A OPCAO DESEJADA: ");

    clock_t inicio, fim;
    double tempo_execucao;
    int opcao, num;
    scanf("%d", &opcao);

    switch (opcao)
    {
    case 1:
        system("cls");

        printf("----------BUSCA SEQUENCIAL----------\n");
        printf("\nDigite o indice a ser procurado: ");
        scanf("%d", &num);

        inicio = clock();
        TEquip *equipamento = busca_sequencial_equipamentos(num, equip, tamanho_total_equipamento(equip));
        fim = clock();
        tempo_execucao = (double)(fim - inicio) / CLOCKS_PER_SEC;

        if (equip != NULL)
        {
            imprime_equip(equipamento);
        }
        else
        {
            printf("\n--------Equipamento nao encontrado--------\n\n");
        }

        printf("\n\nTempo de execucao da busca sequencial: %.6f segundos\n\n", tempo_execucao);

        menuInicialEstoque(equip);
        break;

    case 2:
        system("cls");
        printf("----------BUSCA BINARIA----------\n");
        printf("\nDigite o indice a ser procurado: ");
        scanf("%d", &num);

        inicio = clock();
        TEquip *equipamento1 = busca_binaria_equipamento(num, equip, tamanho_total_equipamento(equip));
        fim = clock();
        tempo_execucao = (double)(fim - inicio) / CLOCKS_PER_SEC;

        if (equip != NULL)
        {
            imprime_equip(equipamento1);
        }
        else
        {
            printf("\n--------equipamento nao encontrado--------\n\n");
        }

        printf("\n\nTempo de execucao da busca sequencial: %.6f segundos\n\n", tempo_execucao);

        menuInicialEstoque(equip);
        break;

    case 9:
        system("cls");
        menuInicialEstoque(equip);
        break;

    default:
        system("cls");
        printf("\n----------EXIT----------\n");
    }
}

TEquip *busca_binaria_equipamento(int cod, FILE *arq, int tam)
{
    int count = 0;
    int left = 0, right = tam - 1;
    while (left <= right)
    {
        count++;
        int middle = (left + right) / 2;
        fseek(arq, middle * tamanho_equipamento_bytes(), SEEK_SET);
        TEquip *equip = leEquip(arq);
        if (cod == equip->num_serie)
        {
            printf("\n**********************************************");
            printf("\nForam gastos %d iteracoes para encontrar o equipamento!\n", count);
            return equip;
        }
        else if (equip->num_serie < cod)
        {
            left = middle + 1;
        }
        else
        {
            right = middle - 1;
        }
    }
    return NULL;
}

int tamanho_total_equipamento(FILE *in)
{
    fseek(in, 0, SEEK_END);
    int tam = ftell(in);
    rewind(in);
    return tam / tamanho_equipamento_bytes();
}

int tamanho_equipamento_bytes()
{
    return sizeof(int) + sizeof(char) * MAX_TAM_NOMES_EQUIP + sizeof(char) * 18;
}

TEquip *busca_sequencial_equipamentos(int cod, FILE *arq, int tam)
{
    rewind(arq); // Reinicia o ponteiro de arquivo para o início

    for (int i = 0; i < tam; i++)
    {
        TEquip *equip = leEquip(arq);
        if (equip != NULL && equip->num_serie == cod)
        {
            printf("\n**********************************************");
            printf("\nForam gastos %d iteracoes para encontrar o equipamento!\n", i + 1);
            return equip; // Funcionário encontrado
        }
        free(equip);
    }

    return NULL; // Funcionário não encontrado
}

void listarEquipamentos(FILE *in){
    printf("\n\nLendo equipamentos do arquivo...\n\n");
    rewind(in);
    TEquip *e;
    while ((e = leEquip(in)) != NULL)
    {
        imprime_equip(e);
        free(e);
    }
    menuInicialEstoque(in);
}

void imprime_equip(TEquip *e)
{
    printf("**********************************************\n");
    printf("\nNumero de serie: %d\n", e->num_serie);
    printf("Nome: %s\n", e->nome);
    printf("MAC: %s\n", e->mac);
    printf("\n\n**********************************************");
}

TEquip *leEquip(FILE *in)
{
    TEquip *equip = (TEquip *)malloc(sizeof(TEquip));
    if (0 >= fread(&equip->num_serie, sizeof(int), 1, in))
    {
        free(equip);
        return NULL;
    }
    fread(equip->nome, sizeof(char), sizeof(equip->nome), in);
    fread(equip->mac, sizeof(char), sizeof(equip->mac), in);

    return equip;
}

void menuCriarBaseEquipamentos(FILE *equip)
{
    printf("-----CRIAR BASE DE EQUIPAMENTOS-----\n");
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
        criarBaseEquipamentosOrdenada(equip);
        fim = clock();
        tempo_execucao = (double)(fim - inicio) / CLOCKS_PER_SEC;
        printf("\nTempo para criar base ordenada: %.6f segundos\n\n", tempo_execucao);
        menuInicialEstoque(equip);
        break;

    case 2:
        system("cls");
        printf("-----CRIAR BASE DESORDENADA-----\n");
        inicio = clock();
        criarBaseEquipamentosDesordenada(equip);
        fim = clock();
        tempo_execucao = (double)(fim - inicio) / CLOCKS_PER_SEC;
        printf("\nTempo para criar base desordenada: %.6f segundos\n\n", tempo_execucao);
        menuInicialEstoque(equip);
        break;

    case 9:
        system("cls");
        menuInicialEstoque(equip);
        break;

    default:
        system("cls");
        printf("\n-----EXIT-----\n");
    }
}

void criarBaseEquipamentosOrdenada(FILE *equip)
{
    int n;
    printf("\nDigite a quantidade de equipamentos a serem inseridos ordenados: ");
    scanf("%d", &n);
    printf("\nInserindo %d equipamentos no arquivo...\n", n);

    for (int i = 1; i <= n; i++)
    {
        char nomeequip[50];
        gerarNomeEquipamento(nomeequip, i);
        char mac[17];
        gerarMAC(mac, i);
        // printf("\n%s - tamanho %d\n", mac, strlen(mac));
        TEquip *f1 = equipamento(i, nomeequip, mac);
        salvaEquipamento(f1, equip);
        free(f1);
    }
    printf("\nCONCLUIDO\n");
}

void gerarNomeEquipamento(char *nomeequip, int i)
{
    const char *nomeequips[MAX_NOMES_EQUIP] = {
        "Linksys WRT3200ACM", "TP-Link Archer C7",
        "Huawei EchoLife HG8245H", "ZTE F660", "Cisco RV260W",
        "Netgear Nighthawk X6", "Nokia G-240W-B", "Fiberhome AN5506-02-FG", "Asus RT-AC68U",
        "D-Link DIR-882", "Calix 716GE-I", "TP-Link TX-VG1530", "MikroTik RB3011UiAS-RM",
        "Ubiquiti UniFi Dream Machine", "Huawei MA5608T", "ZTE C320", "Nokia ISAM 7360",
        "Fiberhome AN5516-04", "Alcatel-Lucent G-010G-A", "Alcatel-Lucent G-240W-C",
        "TP-Link TX-6610", "Huawei B525s-23a", "Alcatel-Lucent 7360 ISAM FX-8"};

    // Gera um índice aleatório para selecionar um nome da lista
    srand(time(NULL));
    int indice = (rand() + i) % MAX_NOMES_EQUIP;

    // Copia o nome selecionado para o parâmetro de saída
    strcpy(nomeequip, nomeequips[indice]);
}

void gerarMAC(char *mac, int n)
{
    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    int j, i = 17;
    for (j = 0; j < i; j++)
    {
        if (j % 3 == 2)
        {
            mac[j] = ':'; // Insere ":" a cada 2 caracteres
        }
        else
        {
            mac[j] = (rand() + n) % 16 + '0'; // Gera um dígito hexadecimal aleatório
            if (mac[j] > '9')
            {
                mac[j] += 7; // Ajusta os caracteres para A-F
            }
        }
    }
    // printf("\nMAC: %s - TAM %d\n", mac, strlen(mac));
    mac[i] = '\0'; // Adiciona o caractere nulo ao final do array
}

// Salva equipamento no arquivo out, na posicao atual do cursor
void salvaEquipamento(TEquip *f1, FILE *equip)
{
    fwrite(&f1->num_serie, sizeof(int), 1, equip);
    fwrite(f1->nome, sizeof(char), sizeof(f1->nome), equip);
    fwrite(f1->mac, sizeof(char), sizeof(f1->mac), equip);
}

TEquip *equipamento(int num_serie, char *nome, char *mac)
{
    TEquip *f1 = (TEquip *)malloc(sizeof(TEquip));
    // inicializa espaço de memória com ZEROS
    if (f1)
        memset(f1, 0, sizeof(TEquip));

    f1->num_serie = num_serie;
    strcpy(f1->nome, nome);
    strcpy(f1->mac, mac);
    return f1;
}

void criarBaseEquipamentosDesordenada(FILE *equip)
{
    int n;
    printf("\nDigite a quantidade de equipamentos a serem inseridos ordenados: ");
    scanf("%d", &n);
    printf("\nInserindo %d equipamentos no arquivo...\n", n);

    int *indices = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        indices[i] = i + 1;
    }
    shuffle(indices, n);

    for (int i = 0; i < n; i++)
    {
        char nomeequip[50];
        gerarNomeEquipamento(nomeequip, i);
        char mac[17];
        gerarMAC(mac, i);
        // printf("\n%s - tamanho %d\n", mac, strlen(mac));
        TEquip *f1 = equipamento(indices[i], nomeequip, mac);
        salvaEquipamento(f1, equip);
        free(f1);
    }
    printf("\nCONCLUIDO\n");
}
