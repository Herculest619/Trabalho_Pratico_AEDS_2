#include "funcionario.h"
#include "main.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>

// imprime_funcionario funcionario
void imprime_funcionario(TFunc *func)
{
    printf("**********************************************\n");
    printf("\nFuncionario de codigo ");
    printf("%d", func->cod);
    printf("\nNome: ");
    printf("%s", func->nome);
    printf("\nCPF: ");
    printf("%s", func->cpf);
    printf("\nData de Nascimento: ");
    printf("%s", func->data_nascimento);
    printf("\nSalario: R$");
    printf("%4.2f", func->salario);
    printf("\n\n**********************************************");
}

// Cria funcionario. Lembrar de usar free(func)
TFunc *funcionario(int cod, char *nome, char *cpf, char *data_nascimento, double salario)
{
    TFunc *func = (TFunc *) malloc(sizeof(TFunc));
    //inicializa espaço de memória com ZEROS
    if (func) memset(func, 0, sizeof(TFunc));
    //copia valores para os campos de func
    func->cod = cod;
    strcpy(func->nome, nome);
    strcpy(func->cpf, cpf);
    strcpy(func->data_nascimento, data_nascimento);
    func->salario = salario;
    return func;
}

// salva_funcionario funcionario no arquivo out, na posicao atual do cursor
void salva_funcionario(TFunc *func, FILE *out)
{
    fwrite(&func->cod, sizeof(int), 1, out);
    //func->nome ao invés de &func->nome, pois string já é ponteiro
    fwrite(func->nome, sizeof(char), sizeof(func->nome), out);
    fwrite(func->cpf, sizeof(char), sizeof(func->cpf), out);
    fwrite(func->data_nascimento, sizeof(char), sizeof(func->data_nascimento), out);
    fwrite(&func->salario, sizeof(double), 1, out);
}

// Le um funcionario do arquivo in na posicao atual do cursor
// Retorna um ponteiro para funcionario lido do arquivo
TFunc *le(FILE *in)
{
    TFunc *func = (TFunc *) malloc(sizeof(TFunc));
    if (0 >= fread(&func->cod, sizeof(int), 1, in))
    {
        free(func);
        return NULL;
    }
    fread(func->nome, sizeof(char), sizeof(func->nome), in);
    fread(func->cpf, sizeof(char), sizeof(func->cpf), in);
    fread(func->data_nascimento, sizeof(char), sizeof(func->data_nascimento), in);
    fread(&func->salario, sizeof(double), 1, in);
    return func;
}

// Retorna tamanho do funcionario em bytes
int tamanho()
{
    return sizeof(int)  //cod
           + sizeof(char) * 50 //nome
           + sizeof(char) * 15 //cpf
           + sizeof(char) * 11 //data_nascimento
           + sizeof(double); //salario
}

TFunc* busca_binaria_funcionario(int cod, FILE *arq, int tam)
{
    int count = 0;
    int left = 0, right = tam - 1;
    while(left <= right)
    {
        count++;
        int middle = (left + right) / 2;
        fseek(arq, middle * tamanho(), SEEK_SET);
        TFunc* func = le(arq);
        if(cod == func->cod)
        {
            printf("\n**********************************************");
            printf("\nForam gastos %d iteracoes para encontrar o funcionario!\n", count);
            return func;
        }
        else if(func->cod < cod)
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

TFunc* busca_sequencial_funcionario(int cod, FILE* arq, int tam)
{
    rewind(arq); // Reinicia o ponteiro de arquivo para o início

    for (int i = 0; i < tam; i++)
    {
        TFunc* func = le(arq);
        if (func != NULL && func->cod == cod)
        {
            printf("\n**********************************************");
            printf("\nForam gastos %d iteracoes para encontrar o funcionario!\n", i+1);
            return func; // Funcionário encontrado
        }
        free(func);
    }

    return NULL; // Funcionário não encontrado
}

void insere_n_funcionarios_ordenados(FILE *out)
{
    int n;
    printf("\nDigite a quantidade de funcionarios a serem inseridos ordenados: ");
    scanf("%d", &n);
    printf("\nInserindo %d funcionarios no arquivo...\n", n);

    for(int i=1; i<=n; i++)
    {
        srand(time(NULL));
        char cpf[15];
        gerarCPF(cpf, i);
        char data[11];
        gerarData(data, i);
        char nome[MAX_TAM_NOME];
        gerarNome(nome, i);
        TFunc *f1 = funcionario(i, nome, cpf, data, gerarSalario(i));
        salva_funcionario(f1, out);
        free(f1);
        srand(0);
    }
    printf("\nCONCLUIDO\n");
}

void shuffle(int *array, int size)
{
    srand(time(NULL));
    if (size > 1)
    {
        for (int i = size - 1; i > 0; i--)
        {
            int j = rand() % (i + 1);
            int temp = array[i];
            array[i] = array[j];
            array[j] = temp;
        }
    }
}

void insere_n_funcionarios_desordenados_funcionario(FILE *out)
{
    int n;
    printf("\nDigite a quantidade de funcionarios a serem inseridos desordenados: ");
    scanf("%d", &n);
    printf("\nInserindo %d funcionarios no arquivo...\n", n);
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
        TFunc *f1 = funcionario(indices[i], nome, cpf, data, gerarSalario(indices[i]));
        salva_funcionario(f1, out);
        free(f1);
    }

    free(indices);
}

double gerarSalario(int i)
{
    double salarioMinimo = 1320.00; // Valor do salário mínimo no Brasil em 2021
    double salarioMaximo = 30000.00;

    double salario = salarioMinimo + ((double)(rand() + i) / RAND_MAX) * (salarioMaximo - salarioMinimo);
    return salario;
}

void gerarNome(char *nome, int i)
{
    const char *nomes[MAX_NOMES] =
    {
        "Maria", "Joao", "Ana", "Jose", "Antonio",
        "Francisco", "Carlos", "Paula", "Marcia", "Luis",
        "Fernanda", "Sandra", "Pedro", "Rafael", "Camila",
        "Bruno", "Diego", "Teresa", "Fabio", "Amanda",
        "Roberto", "Laura", "Larissa", "Gustavo", "Isabela",
        "Marcelo", "Ricardo", "Simone", "Rodrigo", "Vitor",
        "Lucas", "Andre", "Adriana", "Cristina", "Eduardo",
        "Vanessa", "Carolina", "Fernando", "Valeria", "Silvia",
        "Hugo", "Fabiana", "Sonia", "Gisele", "Mariana",
        "Natalia", "Raul", "Luana", "Beatriz", "Camilo",
        "Vinicius", "Leticia", "Alessandra", "Giovanna", "Glaucio",
        "Gabriel", "Romulo", "Cecilia", "Samuel", "Renato",
        "Victor", "Rosa", "Milton", "Leandro", "Bruna",
        "Marina", "Lucia", "Dalila", "Sara", "Adriano",
        "Julia", "Simara", "Thiago", "Elisa", "Carla",
        "Juliana", "Renata", "Raphael", "Daniele", "Larissa",
        "Luciana", "Alexandre", "Cintia", "Helen", "Marcela",
        "Priscila", "Bruno", "Patricia", "Caua", "Jessica",
        "Mariane", "Diana", "Suzana", "Miguel", "Flavia",
        "Karine", "Cassio"
    };

    // Gera um índice aleatório para selecionar um nome da lista
    srand(time(NULL));
    int indice = (rand() + i) % MAX_NOMES;

    // Copia o nome selecionado para o parâmetro de saída
    snprintf(nome, MAX_TAM_NOME, "%s", nomes[indice]);
}

void gerarData(char *data, int i)
{
    int dia, mes, ano;
    //srand(time(NULL));

    // Gera um ano aleatório entre 1980 e 2022
    ano = (rand() + i) % (2022 - 1980 + 1) + 1980;

    // Gera um mês aleatório entre 1 e 12
    mes = rand() % 12 + 1;

    // Verifica o número de dias do mês gerado
    int diasNoMes;
    switch (mes)
    {
    case 2: // Fevereiro
        diasNoMes = (ano % 4 == 0 && (ano % 100 != 0 || ano % 400 == 0)) ? 29 : 28;
        break;
    case 4: // Abril
    case 6: // Junho
    case 9: // Setembro
    case 11: // Novembro
        diasNoMes = 30;
        break;
    default:
        diasNoMes = 31;
        break;
    }

    // Gera um dia aleatório entre 1 e o número de dias do mês
    dia = rand() % diasNoMes + 1;

    // Formata a data no formato "dd/mm/aaaa"
    snprintf(data, 11, "%02d/%02d/%04d", dia, mes, ano);
}

void gerarCPF(char *cpf, int n)
{
    int i, j;
    int digito1 = 0, digito2 = 0;
    srand(time(NULL));

    // Gera os 9 primeiros dígitos do CPF
    for (i = 0; i < 9; i++)
    {
        cpf[i] = (rand() + n) % 10 + '0';
    }

    // Calcula o primeiro dígito verificador
    for (i = 0, j = 10; i < 9; i++, j--)
    {
        digito1 += (cpf[i] - '0') * j;
    }
    digito1 = (digito1 % 11 < 2) ? 0 : 11 - (digito1 % 11);
    cpf[9] = digito1 + '0';

    // Calcula o segundo dígito verificador
    for (i = 0, j = 11; i < 10; i++, j--)
    {
        digito2 += (cpf[i] - '0') * j;
    }
    digito2 = (digito2 % 11 < 2) ? 0 : 11 - (digito2 % 11);
    cpf[10] = digito2 + '0';
    cpf[11] = '\0';

    // Insere os pontos e hífen
    char cpfFormatado[15];
    snprintf(cpfFormatado, sizeof(cpfFormatado), "%c%c%c.%c%c%c.%c%c%c-%c%c",
             cpf[0], cpf[1], cpf[2], cpf[3], cpf[4], cpf[5], cpf[6],
             cpf[7], cpf[8], cpf[9], cpf[10]);

    // Copia o CPF formatado para o parâmetro de saída
    snprintf(cpf, 15, "%s", cpfFormatado);
}

void menuBusca(FILE *out)
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

        TFunc* func = busca_sequencial_funcionario(num, out, tamanho_total_funcionario(out));

        fim = clock();
        tempo_execucao = (double)(fim - inicio) / CLOCKS_PER_SEC;

        if (func != NULL)
        {
            imprime_funcionario(func);
        }
        else
        {
            printf("\n--------Funcionario nao encontrado--------\n\n");
        }

        printf("\n\nTempo de execucao da busca sequencial: %.6f segundos\n\n", tempo_execucao);

        menuInicialFuncionario(out);
        break;

    case 2:
        system("cls");
        int indice;
        printf("----------BUSCA BINARIA----------\n");
        printf("\nDigite o indice a ser procurado: ");
        scanf("%d", &indice);

        inicio = clock();

        TFunc* func1 = busca_binaria_funcionario(indice, out, tamanho_total_funcionario(out));

        fim = clock();
        tempo_execucao = (double)(fim - inicio) / CLOCKS_PER_SEC;

        if (func1 != NULL)
        {
            imprime_funcionario(func1);
        }
        else
        {
            printf("\n--------Funcionario nao encontrado--------\n\n");
        }

        printf("\n\nTempo de execucao da busca binaria: %.6f segundos\n\n", tempo_execucao);

        menuInicialFuncionario(out);
        break;


    case 9:
        system("cls");
        menuInicialFuncionario(out);
        break;

    default:
        system("cls");
        printf("\n----------EXIT----------\n");
    }

}

void menuInicialFuncionario(FILE *out)
{
    printf("**********************************************\n");
    printf("----MENU FUNCIONARIOS----\n");
    printf("1: CRIAR BASE\n");
    printf("2: IMPRIMIR BASE\n");
    printf("3: BUSCA\n");
    printf("4: ORDENAR BASE\n");
    printf("9: VOLTAR\n");
    printf("0: EXIT\n");
    printf("**********************************************\n\n");
    printf("DIGITE A OPCAO DESEJADA: ");

    int opcao;
    scanf("%d", &opcao);

    switch (opcao)
    {
    case 1:
        system("cls");
        menuCriarBaseFuncioarios(out);
        break;
    case 2:
        system("cls");
        printf("----------IMPRIMIR BASE----------\n");
        le_funcionarios(out);
        menuInicialFuncionario(out);
        break;
    case 3:
        system("cls");
        menuBusca(out);
        break;

    case 4:
        system("cls");
        menu_ordenacao_funcionario(out);
        break;
    case 9:
        system("cls");
        main();
        break;
    default:
        system("cls");
        printf("\n----------EXIT----------\n");
    }
}

void menu_ordenacao_funcionario(FILE *out)
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


        menu_ordenacao_funcionario_disco(out);

        break;

    /*case 2:

        system("cls");
        printf("ORDENANDO BASE EM RAM USANDO CLASSIFICACAO INTERNA....\n");

        classificacao_interna(out);

        menuInicial(out);
        break;*/

    case 9:
        system("cls");
        menuInicialFuncionario(out);
        break;

    default:
        system("cls");
        printf("\n-----EXIT-----\n");
    }
}

void menu_ordenacao_funcionario_disco(FILE *out)
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

        insertion_sort_disco_funcionario(out);

        fim = clock();
        tempo_execucao = (double)(fim - inicio) / CLOCKS_PER_SEC;
        printf("\nTempo para ordenar com Insertion Sort: %.6f segundos\n\n", tempo_execucao);
        menuInicialFuncionario(out);
        break;

    case 2:
        system("cls");
        printf("ORDENANDO BASE COM SELECTION SORT....\n");
        inicio = clock();

        selection_sort_disco_funcionario(out);

        fim = clock();
        tempo_execucao = (double)(fim - inicio) / CLOCKS_PER_SEC;
        printf("\nTempo para ordenar com Selection Sort: %.6f segundos\n\n", tempo_execucao);
        menuInicialFuncionario(out);
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

void menuCriarBaseFuncioarios(FILE *out)
{
    printf("-----CRIAR BASE-----\n");
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
        insere_n_funcionarios_ordenados(out);
        fim = clock();
        tempo_execucao = (double)(fim - inicio) / CLOCKS_PER_SEC;
        printf("\nTempo para criar base ordenada: %.6f segundos\n\n", tempo_execucao);
        menuInicialFuncionario(out);
        break;
    case 2:
        system("cls");
        printf("-----CRIAR BASE DESORDENADA-----\n");
        inicio = clock();
        insere_n_funcionarios_desordenados_funcionario(out);
        fim = clock();
        tempo_execucao = (double)(fim - inicio) / CLOCKS_PER_SEC;
        printf("\nTempo para criar base desordenada: %.6f segundos\n\n", tempo_execucao);
        menuInicialFuncionario(out);
        break;

    case 9:
        system("cls");
        menuInicialFuncionario(out);
        break;

    default:
        system("cls");
        printf("\n-----EXIT-----\n");
    }
}

void classificacao_interna(FILE *arq)
{
    int tam = tamanho_total_funcionario(arq);
    float tam_part;
    int qnt_part;

    printf("\nDigite o tamanho de particoes: ");
    scanf("%f", &tam_part);

    if(tam_part > tam){
        printf("\nNao e possivel criar uma particao maior que o tamanho total da base.\n\n");
        classificacao_interna(arq);
    }

    qnt_part = (int)ceil(tam / tam_part);

    printf("\nQuantidade de particoes: %d\n", qnt_part);

    // Array para armazenar temporariamente os registros da partição em RAM
    TFunc *particao_temp = malloc(sizeof(TFunc) * tam_part);

    for(int i=1; i<=qnt_part; i++)
    {
        //declara ponteiro para arquivo
        FILE *particao;
        //abre arquivo
        char nome_arq[16];
        sprintf(nome_arq, "particao-%02d.dat", i);


        if ((particao = fopen(nome_arq, "w+b")) == NULL)
        {
            printf("Erro ao abrir arquivo\n");
            exit(1);
        }
        else
        {
            // Cálculo do início e fim da partição
            int inicio = (i - 1) * tam_part;
            int fim = inicio + tam_part;

            // Verificação para evitar que o fim ultrapasse o tamanho total do arquivo
            if (fim > tam)
            {
                fim = tam;
            }

            // Leitura dos registros do arquivo original e escrita na partição
            fseek(arq, inicio * sizeof(TFunc), SEEK_SET);
            fread(particao_temp, sizeof(TFunc), fim - inicio, arq);

            // Ordenação da partição em RAM (exemplo: uso do algoritmo Bubble Sort)
            for (int j = 0; j < fim - inicio - 1; j++)
            {
                for (int k = 0; k < fim - inicio - j - 1; k++)
                {
                    if (particao_temp[k].cod > particao_temp[k + 1].cod)
                    {
                        TFunc temp = particao_temp[k];
                        particao_temp[k].cod = particao_temp[k + 1].cod;
                        particao_temp[k + 1].cod = temp.cod;
                    }
                }
            }

            fwrite(particao_temp, sizeof(TFunc), fim - inicio, particao);

            //printf("\nParticao %d \n", i);
            //le_funcionarios(particao);

            // Fechamento do arquivo da partição
            fclose(particao);
        }
    }
    free(particao_temp);
    //junta_particoes(qnt_part, "particao-%02d.dat");
}

void junta_particoes(int qnt_part, char nome_arq[]){
    qnt_part = qnt_part; //so pra sair o warning
    nome_arq = nome_arq; //so pra sair o warning
}

void selection_sort_disco_funcionario(FILE *arq){
    //ordenação pelo atributo cod de TFunc
    int tam = tamanho_total_funcionario(arq);
    int i, j, min, cont = 0;
    //faz o selection sort
    for (i = 1; i < tam; i++)
    {
        //posiciona o arquivo no registro i
        fseek(arq, (i-1) * tamanho(), SEEK_SET);
        TFunc *fi = le(arq);
        //printf("\n********* Funcionario atual: %d\n", fi->cod);
        min = i;
        for (j = i+1; j <= tam; j++)
        {
            //posiciona o arquivo no registro j
            fseek(arq, (j-1) * tamanho(), SEEK_SET);
            TFunc *fj = le(arq);
            //printf("fj = %d\n", fj->cod);
            if (fj->cod < fi->cod)
            {
                min = j;
                //printf("min = %d\n", min);
                //posiciona o arquivo no registro min
                fseek(arq, (min-1) * tamanho(), SEEK_SET);
                TFunc *fmin = le(arq);
                //printf("fmin = %d\n", fmin->cod);
                //troca os registros
                fseek(arq, (i-1) * tamanho(), SEEK_SET);
                salva_funcionario(fmin, arq);
                fseek(arq, (min-1) * tamanho(), SEEK_SET);
                salva_funcionario(fi, arq);
                //posiciona o arquivo no registro i
                fseek(arq, (i-1) * tamanho(), SEEK_SET);
                fi = le(arq);
                //printf("fi = %d\n", fi->cod);
                cont++;
            }
            free(fj);
        }
        free(fi);
    }
    printf("\nTotal de iteracoes: %d\n", cont);
}

void insertion_sort_disco_funcionario(FILE *arq)
{
    int tam = tamanho_total_funcionario(arq);
    int i, cont=0;
    //faz o insertion sort
    for (int j = 2; j <= tam; j++)
    {
        //posiciona o arquivo no registro j
        fseek(arq, (j-1) * tamanho(), SEEK_SET);
        TFunc *fj = le(arq);
        //printf("\n********* Funcionario atual: %d\n", fj->cod);
        i = j - 1;
        //posiciona o cursor no registro i
        fseek(arq, (i-1) * tamanho(), SEEK_SET);
        TFunc *fi = le(arq);
        //printf("fi = %d\n", fi->cod);
        while ((i > 0) && (fi->cod > fj->cod))
        {
            //posiciona o cursor no registro i+1
            fseek(arq, i * tamanho(), SEEK_SET);
            //printf("salva_funcionariondo funcionario %d na posicao %d\n", fi->cod, i+1);
            salva_funcionario(fi, arq);
            i = i - 1;
            //lÃª registro i
            fseek(arq, (i-1) * tamanho(), SEEK_SET);
            fi = le(arq);
            //printf("fi = %d; i = %d\n", fi->cod, i);
            cont++;
        }
        //posiciona cursor no registro i + 1
        fseek(arq, (i) * tamanho(), SEEK_SET);
        //printf("*** salva_funcionariondo funcionario %d na posicao %d\n", fj->cod, i+1);
        //salva_funcionario registro j na posiÃ§Ã£o i
        salva_funcionario(fj, arq);
    }
    printf("\nForam gastas %d iteracoes para ordenar!\n", cont);
    //descarrega o buffer para ter certeza que dados foram gravados
    fflush(arq);
}

void insertion_sort_memoria_funcionario(FILE *arq)
{
    int tam = tamanho_total_funcionario(arq);
    TFunc *v[tam];
    //le o arquivo e coloca no vetor
    rewind(arq); //posiciona cursor no inicio do arquivo
    TFunc *f = le(arq);
    int i = 0;
    while (!feof(arq))
    {
        v[i] = f;
        f = le(arq);
        i++;
    }
    //faz o insertion sort
    for (int j = 1; j < tam; j++)
    {
        TFunc *f = v[j];
        i = j - 1;
        while ((i >= 0) && (v[i]->cod > f->cod))
        {
            v[i + 1] = v[i];
            i = i - 1;
        }
        v[i+1] = f;
    }
    //grava vetor no arquivo, por cima do conteÃºdo anterior
    rewind(arq);
    for (int i = 0; i < tam; i++)
    {
        salva_funcionario(v[i], arq);
    }
    //descarrega o buffer para ter certeza que dados foram gravados
    fflush(arq);

}

int tamanho_total_funcionario(FILE *in)
{
    rewind(in);
    TFunc *f;
    int i=0;
    while ((f = le(in)) != NULL)
    {
        free(f);
        i++;
    }
    return i;
}

void le_funcionarios(FILE *in)
{
    printf("\n\nLendo funcionarios do arquivo...\n\n");
    rewind(in);
    TFunc *f;
    while ((f = le(in)) != NULL)
    {
        imprime_funcionario(f);
        free(f);
    }
}

void adiciona_funcionario(FILE *in)
{
    printf("\n\nAdicionando funcionário no final do arquivo...\n\n");
    //pula 5 registros para posicionar no início do final do arquivo
    fseek(in, tamanho() * 5, SEEK_SET);
    TFunc *f = funcionario(6, "Bruna", "666.666.666-66", "06/06/1980", 2500);
    salva_funcionario(f, in);
    free(f);

    //lê funcionário que acabou de ser gravado
    //posiciona novamente o cursor no início desse registro
    fseek(in, tamanho() * 5, SEEK_SET);
    TFunc *f6 = le(in);
    if (f6 != NULL)
    {
        imprime_funcionario(f6);
        free(f6);
    }
}