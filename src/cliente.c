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
        le_cliente(clien);
        menuInicialCliente(clien);
        break;

    case 3:
        system("cls");
        menuBuscaCliente(clien);
        break;

    case 4:
        system("cls");
        menuOrdenarCliente(clien);
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

Tclien *cliente(int cod, char *nome, char *cpf, char *data, double mensalidade){
    // Aloca memória para o cliente
    Tclien *clien = (Tclien *)malloc(sizeof(Tclien));
    //inicializa espaço de memória com ZEROS
    if (clien) memset(clien, 0, sizeof(Tclien));

    clien->cod = cod;
    strcpy(clien->nome, nome);
    strcpy(clien->cpf, cpf);
    strcpy(clien->data_nascimento, data);
    clien->mensalidade = mensalidade;

    return clien;
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

void criarBaseClientesDesordenada(FILE *clien){
    int n;
    printf("\nDigite a quantidade de clientes a serem inseridos desordenados: ");
    scanf("%d", &n);
    printf("\nInserindo %d clientes no arquivo...\n", n);
    printf("\nCONCLUIDO\n");

    int *indices = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        indices[i] = i + 1;
    }
    shuffle(indices, n);

    for (int i = 0; i < n; i++)
    {
        char cpf[15];
        gerarCPF(cpf, indices[i]);
        char data[11];
        gerarData(data, indices[i]);
        char nome[MAX_TAM_NOME];
        gerarNome(nome, indices[i]);
        Tclien *c1 = cliente(indices[i], nome, cpf, data, gerarMensalidade(i));
        salva_cliente(c1, clien);
        free(c1);
    }

    free(indices);
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
    double mensalidade = ((rand()*i) % 14000) + 5990;
    mensalidade = mensalidade / 100;
    return mensalidade;
}

void imprime_cliente(Tclien *clien)
{
    printf("**********************************************\n");
    printf("\nCliente de codigo ");
    printf("%d", clien->cod);
    printf("\nNome: ");
    printf("%s", clien->nome);
    printf("\nCPF: ");
    printf("%s", clien->cpf);
    printf("\nData de Nascimento: ");
    printf("%s", clien->data_nascimento);
    printf("\nMansalidade: R$");
    printf("%4.2f", clien->mensalidade);
    printf("\n\n**********************************************");
}

void le_cliente(FILE *in)
{
    printf("\n\nLendo cliente do arquivo...\n\n");
    rewind(in);
    Tclien *c;
    while ((c = ler(in)) != NULL)
    {
        imprime_cliente(c);
        free(c);
    }
}

Tclien *ler(FILE *in)
{
    Tclien *clien = (Tclien *) malloc(sizeof(Tclien));
    if (0 >= fread(&clien->cod, sizeof(int), 1, in))
    {
        free(clien);
        return NULL;
    }
    fread(clien->nome, sizeof(char), sizeof(clien->nome), in);
    fread(clien->cpf, sizeof(char), sizeof(clien->cpf), in);
    fread(clien->data_nascimento, sizeof(char), sizeof(clien->data_nascimento), in);
    fread(&clien->mensalidade, sizeof(double), 1, in);
    return clien;
}

void menuBuscaCliente(FILE *out)
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

        Tclien* clien = busca_sequencial_cliente(num, out, tamanho_total_cliente(out));

        fim = clock();
        tempo_execucao = (double)(fim - inicio) / CLOCKS_PER_SEC;

        if (clien != NULL)
        {
            imprime_cliente(clien);
        }
        else
        {
            printf("\n--------Cliente nao encontrado--------\n\n");
        }

        printf("\n\nTempo de execucao da busca sequencial: %.6f segundos\n\n", tempo_execucao);

        menuInicialCliente(out);
        break;

    case 2:
        system("cls");
        int indice;
        printf("----------BUSCA BINARIA----------\n");
        printf("\nDigite o indice a ser procurado: ");
        scanf("%d", &indice);

        inicio = clock();

        Tclien* clien1 = busca_binaria_cliente(indice, out, tamanho_total_cliente(out));

        fim = clock();
        tempo_execucao = (double)(fim - inicio) / CLOCKS_PER_SEC;

        if (clien1 != NULL)
        {
            imprime_cliente(clien1);
        }
        else
        {
            printf("\n--------Cliente nao encontrado--------\n\n");
        }

        printf("\n\nTempo de execucao da busca binaria: %.6f segundos\n\n", tempo_execucao);

        menuInicialCliente(out);
        break;


    case 9:
        system("cls");
        menuInicialCliente(out);
        break;

    default:
        system("cls");
        printf("\n----------EXIT----------\n");
    }

}

Tclien* busca_sequencial_cliente(int cod, FILE* arq, int tam)
{
    rewind(arq); // Reinicia o ponteiro de arquivo para o início

    for (int i = 0; i < tam; i++)
    {
        Tclien* clien = ler(arq);
        if (clien != NULL && clien->cod == cod)
        {
            printf("\n**********************************************");
            printf("\nForam gastos %d iteracoes para encontrar o cliente!\n", i+1);
            return clien; // Cliente encontrado
        }
        free(clien);
    }

    return NULL; // Cliente não encontrado
}

Tclien* busca_binaria_cliente(int cod, FILE *arq, int tam)
{
    int count = 0;
    int left = 0, right = tam - 1;
    while(left <= right)
    {
        count++;
        int middle = (left + right) / 2;
        fseek(arq, middle * tamanho(), SEEK_SET);
        Tclien* clien = ler(arq);
        if(cod == clien->cod)
        {
            printf("\n**********************************************");
            printf("\nForam gastos %d iteracoes para encontrar o cliente!\n", count);
            return clien;
        }
        else if(clien->cod < cod)
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

int tamanho_total_cliente(FILE *in)
{
    rewind(in);
    Tclien *c;
    int i=0;
    while ((c = ler(in)) != NULL)
    {
        free(c);
        i++;
    }
    return i;
}

/*void menuOrdenarCliente(FILE *out)
{
    printf("-----ORDENAR BASE-----\n");
    printf("1: ORDENACAO EM DISCO\n");
    //printf("2: ORDENACAO EM RAM\n");
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


        menu_ordenacao_cliente_disco(out);

        break;*/

    /*case 2:

        system("cls");
        printf("ORDENANDO BASE EM RAM USANDO CLASSIFICACAO INTERNA....\n");

        classificacao_interna(out);

        menuInicial(out);
        break;*/

/*    case 9:
        system("cls");
        menuInicialCliente(out);
        break;

    default:
        system("cls");
        printf("\n-----EXIT-----\n");
    }
}*/

void menuOrdenarCliente(FILE *out)
{
    printf("-----ORDENAR BASE-----\n");
    printf("1: INSERTION SORT\n");
    printf("2: SELECTION SORT\n");
    //printf("3: MERGE SORT\n");
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

        insertion_sort_disco_cliente(out);

        fim = clock();
        tempo_execucao = (double)(fim - inicio) / CLOCKS_PER_SEC;
        printf("\nTempo para ordenar com Insertion Sort: %.6f segundos\n\n", tempo_execucao);
        menuInicialCliente(out);
        break;

    case 2:
        system("cls");
        printf("ORDENANDO BASE COM SELECTION SORT....\n");
        inicio = clock();

        selection_sort_disco_cliente(out);

        fim = clock();
        tempo_execucao = (double)(fim - inicio) / CLOCKS_PER_SEC;
        printf("\nTempo para ordenar com Selection Sort: %.6f segundos\n\n", tempo_execucao);
        menuInicialCliente(out);
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

void insertion_sort_disco_cliente(FILE *arq)
{
    int tam = tamanho_total_cliente(arq);
    int i, cont=0;
    //faz o insertion sort
    for (int j = 2; j <= tam; j++)
    {
        //posiciona o arquivo no registro j
        fseek(arq, (j-1) * tamanho(), SEEK_SET);
        Tclien *fj = ler(arq);
        //printf("\n********* Cliente atual: %d\n", fj->cod);
        i = j - 1;
        //posiciona o cursor no registro i
        fseek(arq, (i-1) * tamanho(), SEEK_SET);
        Tclien *fi = ler(arq);
        //printf("fi = %d\n", fi->cod);
        while ((i > 0) && (fi->cod > fj->cod))
        {
            //posiciona o cursor no registro i+1
            fseek(arq, i * tamanho(), SEEK_SET);
            //printf("salva_cliente cliente %d na posicao %d\n", fi->cod, i+1);
            salva_cliente(fi, arq);
            i = i - 1;
            //lÃª registro i
            fseek(arq, (i-1) * tamanho(), SEEK_SET);
            fi = ler(arq);
            //printf("fi = %d; i = %d\n", fi->cod, i);
            cont++;
        }
        //posiciona cursor no registro i + 1
        fseek(arq, (i) * tamanho(), SEEK_SET);
        //printf("*** salva_cliente cliente %d na posicao %d\n", fj->cod, i+1);
        //salva_cliente registro j na posiÃ§Ã£o i
        salva_cliente(fj, arq);
    }
    printf("\nForam gastas %d iteracoes para ordenar!\n", cont);
    //descarrega o buffer para ter certeza que dados foram gravados
    fflush(arq);
}

void selection_sort_disco_cliente(FILE *arq)
{
    //ordenação pelo atributo cod de Tclien
    int tam = tamanho_total_cliente(arq);
    int i, j, min, cont = 0;
    //faz o selection sort
    for (i = 1; i < tam; i++)
    {
        //posiciona o arquivo no registro i
        fseek(arq, (i-1) * tamanho(), SEEK_SET);
        Tclien *fi = ler(arq);
        //printf("\n********* cliente atual: %d\n", fi->cod);
        min = i;
        for (j = i+1; j <= tam; j++)
        {
            //posiciona o arquivo no registro j
            fseek(arq, (j-1) * tamanho(), SEEK_SET);
            Tclien *fj = ler(arq);
            //printf("fj = %d\n", fj->cod);
            if (fj->cod < fi->cod)
            {
                min = j;
                //printf("min = %d\n", min);
                //posiciona o arquivo no registro min
                fseek(arq, (min-1) * tamanho(), SEEK_SET);
                Tclien *fmin = ler(arq);
                //printf("fmin = %d\n", fmin->cod);
                //troca os registros
                fseek(arq, (i-1) * tamanho(), SEEK_SET);
                salva_cliente(fmin, arq);
                fseek(arq, (min-1) * tamanho(), SEEK_SET);
                salva_cliente(fi, arq);
                //posiciona o arquivo no registro i
                fseek(arq, (i-1) * tamanho(), SEEK_SET);
                fi = ler(arq);
                //printf("fi = %d\n", fi->cod);
                cont++;
            }
            free(fj);
        }
        free(fi);
    }
    printf("\nTotal de iteracoes: %d\n", cont);
}
