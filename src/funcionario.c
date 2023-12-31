#include "funcionario.h"
#include "main.h"
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// imprime_funcionario funcionario
void imprime_funcionario(TFunc *func)
{
    printf("**********************************************");
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
    printf("\n**********************************************");
    printf("\n\n\n");
}

// Cria funcionario. Lembrar de usar free(func)
TFunc *funcionario(int cod, char *nome, char *cpf, char *data_nascimento, double salario)
{
    TFunc *func = (TFunc *)malloc(sizeof(TFunc));
    //inicializa espaço de memória com ZEROS
    if (func)
        memset(func, 0, sizeof(TFunc));
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
    TFunc *func = (TFunc *)malloc(sizeof(TFunc));
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
    return sizeof(int)         //cod
           + sizeof(char) * 50 //nome
           + sizeof(char) * 15 //cpf
           + sizeof(char) * 11 //data_nascimento
           + sizeof(double);   //salario
}

TFunc *busca_binaria_funcionario(int cod, FILE *arq, int tam)
{
    int count = 0;
    int left = 0, right = tam - 1;
    while (left <= right)
    {
        count++;
        int middle = (left + right) / 2;
        fseek(arq, middle * tamanho(), SEEK_SET);
        TFunc *func = le(arq);
        if (cod == func->cod)
        {
            printf("\n**********************************************");
            printf("\nForam gastos %d iteracoes para encontrar o funcionario!\n", count);
            return func;
        }
        else if (func->cod < cod)
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

TFunc *busca_sequencial_funcionario(int cod, FILE *arq, int tam)
{
    rewind(arq); // Reinicia o ponteiro de arquivo para o início

    for (int i = 0; i < tam; i++)
    {
        TFunc *func = le(arq);
        if (func != NULL && func->cod == cod)
        {
            printf("\n**********************************************");
            printf("\nForam gastos %d iteracoes para encontrar o funcionario!\n", i + 1);
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

    for (int i = 1; i <= n; i++)
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

    int *indices = (int *)malloc(n * sizeof(int));
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

    double salario = salarioMinimo + ((double)(rand() * i) / RAND_MAX) * (salarioMaximo - salarioMinimo);
    return salario;
}

void gerarNome(char *nome, int i)
{
    const char *nomes[MAX_NOMES] = {
        "Maria",     "Joao",     "Ana",     "Jose",    "Antonio",  "Francisco", "Carlos",   "Paula",      "Marcia",
        "Luis",      "Fernanda", "Sandra",  "Pedro",   "Rafael",   "Camila",    "Bruno",    "Diego",      "Teresa",
        "Fabio",     "Amanda",   "Roberto", "Laura",   "Larissa",  "Gustavo",   "Isabela",  "Marcelo",    "Ricardo",
        "Simone",    "Rodrigo",  "Vitor",   "Lucas",   "Andre",    "Adriana",   "Cristina", "Eduardo",    "Vanessa",
        "Carolina",  "Fernando", "Valeria", "Silvia",  "Hugo",     "Fabiana",   "Sonia",    "Gisele",     "Mariana",
        "Natalia",   "Raul",     "Luana",   "Beatriz", "Camilo",   "Vinicius",  "Leticia",  "Alessandra", "Giovanna",
        "Glaucio",   "Gabriel",  "Romulo",  "Cecilia", "Samuel",   "Renato",    "Victor",   "Rosa",       "Milton",
        "Leandro",   "Bruna",    "Marina",  "Lucia",   "Dalila",   "Sara",      "Adriano",  "Julia",      "Simara",
        "Thiago",    "Elisa",    "Carla",   "Juliana", "Renata",   "Raphael",   "Daniele",  "Larissa",    "Luciana",
        "Alexandre", "Cintia",   "Helen",   "Marcela", "Priscila", "Bruno",     "Patricia", "Caua",       "Jessica",
        "Mariane",   "Diana",    "Suzana",  "Miguel",  "Flavia",   "Karine",    "Cassio"};

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
    case 4:  // Abril
    case 6:  // Junho
    case 9:  // Setembro
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
    snprintf(cpfFormatado,
             sizeof(cpfFormatado),
             "%c%c%c.%c%c%c.%c%c%c-%c%c",
             cpf[0],
             cpf[1],
             cpf[2],
             cpf[3],
             cpf[4],
             cpf[5],
             cpf[6],
             cpf[7],
             cpf[8],
             cpf[9],
             cpf[10]);

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

        TFunc *func = busca_sequencial_funcionario(num, out, tamanho_total_funcionario(out));

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

        FILE *resultado;
        resultado = fopen("resultados.txt", "a");

        if (resultado == NULL) {
            printf("Erro ao abrir o arquivo.\n");
            menuInicialFuncionario(out);
            break;
        }

        fprintf(resultado, "-----------------------------\n");
        fprintf(resultado, "Busca sequencial\n");
        fprintf(resultado, "Tempo de execucao: %f segundos\n", tempo_execucao);
        fprintf(resultado, "Tamanho da base: %d\n", tamanho_total_funcionario(out));
        fprintf(resultado, "-----------------------------\n\n\n");

        fclose(resultado);

        menuInicialFuncionario(out);
        break;

    case 2:
        system("cls");
        int indice;
        printf("----------BUSCA BINARIA----------\n");
        printf("\nDigite o indice a ser procurado: ");
        scanf("%d", &indice);

        inicio = clock();

        TFunc *func1 = busca_binaria_funcionario(indice, out, tamanho_total_funcionario(out));

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

        FILE *resultado1;
        resultado1 = fopen("resultados.txt", "a");

        if (resultado1 == NULL)
        {
            printf("Erro ao abrir o arquivo.\n");
            menuInicialFuncionario(out);
            break;
        }

        fprintf(resultado1, "-----------------------------\n");
        fprintf(resultado1, "Busca binaria\n");
        fprintf(resultado1, "Tempo de execucao: %f segundos\n", tempo_execucao);
        fprintf(resultado1, "Tamanho da base: %d\n", tamanho_total_funcionario(out));
        fprintf(resultado1, "-----------------------------\n\n\n");

        fclose(resultado1);

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
    printf("5: EXCLUIR BASE\n");
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
    case 5:
        system("cls");
        printf("----------EXCLUIR BASE----------\n");
        printf("\nDeseja excluir a base de funcionarios?\nDigite 1 para SIM ou 0 para NAO: ");
        int opcao;
        scanf("%d", &opcao);

        if (opcao == 1)
        {
            fclose(out);
            remove("funcionario.dat");
            out = fopen("funcionario.dat", "w+b");
            printf("\nBase de funcionarios excluida com sucesso!\n\n");
        }
        else
        {
            printf("\nBase de funcionarios nao excluida!\n\n");
        }

        menuInicialFuncionario(out);
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
    printf("2: ORDENACAO EM RAM (PARTICOES)\n");
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

    case 2:

        system("cls");

        criar_particoes_func(out);

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

        //salvar o tempo de execução em um arquivo txt
        FILE *resultadob;
        resultadob = fopen("resultados.txt", "a");

        if (resultadob == NULL) {
            printf("Erro ao abrir o arquivo.\n");
            menuInicialFuncionario(out);
            break;
        }

        fprintf(resultadob, "-----------------------------\n");
        fprintf(resultadob, "Ordenção em disco usando Insertion Sort\n");
        fprintf(resultadob, "Tempo de execucao: %f segundos\n", tempo_execucao);
        fprintf(resultadob, "Tamanho da base: %d\n", tamanho_total_funcionario(out));
        fprintf(resultadob, "-----------------------------\n\n\n");

        fclose(resultadob);

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

        //salvar o tempo de execução em um arquivo txt
        FILE *resultado;
        resultado = fopen("resultados.txt", "a");

        if (resultado == NULL)
        {
            printf("Erro ao abrir o arquivo.\n");
            menuInicialFuncionario(out);
            break;
        }

        fprintf(resultado, "-----------------------------\n");
        fprintf(resultado, "Ordenção em disco usando Selection Sort\n");
        fprintf(resultado, "Tempo de execucao: %f segundos\n", tempo_execucao);
        fprintf(resultado, "Tamanho da base: %d\n", tamanho_total_funcionario(out));
        fprintf(resultado, "-----------------------------\n\n\n");

        fclose(resultado);

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

void criar_particoes_func(FILE *arq)
{
    int tam = tamanho_total_funcionario(arq);
    float tam_part;
    int qnt_part;

    printf("\nDigite o tamanho de particoes (MAX = %d): ", tam);
    scanf("%f", &tam_part);

    if (tam_part > tam)
    {
        printf("\nNao e possivel criar uma particao maior que o tamanho total da base.\n\n");
        criar_particoes_func(arq);
    }
    else if (tam_part <= 0)
    {
        printf("\nNao e possivel criar uma particao com o tamanho igual ou menor a 0.\n\n");
        criar_particoes_func(arq);
    }

    qnt_part = (int)ceil(tam / tam_part);

    printf("\nQuantidade de particoes: %d\n", qnt_part);

    if (qnt_part > 505){
        printf("\nNao e possivel criar mais de 505 particoes.\n\n");
        criar_particoes_func(arq);
    }

    // Array para armazenar temporariamente os registros da partição em RAM
    TFunc *particao_temp = malloc(sizeof(TFunc) * tam_part);

    printf("\n------CRIANDO PARTICOES ORDENADAS------\n");

    int opcao = 0;
    printf("\nDeseja imprimir as particoes ordenadas?\nDigite 1 para SIM ou 0 para NAO: ");
    scanf("%d", &opcao);

    clock_t start, end;
    double cpu_time_used;

    start = clock();

    for (int i = 1; i <= qnt_part; i++)
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

            // Ordenação da partição em RAM usando a Seleção por substituição
            selececao_por_substituicao_func(particao_temp, fim - inicio);

            fwrite(particao_temp, sizeof(TFunc), fim - inicio, particao);

            if (opcao != 0)
            {
                printf("\n\n\n-+-+-+-+-+-+-+-+- PARTICAO %d -+-+-+-+-+-+-+-+-", i);
                le_funcionarios(particao);
            }

            // Fechamento do arquivo da partição
            fclose(particao);
        }
    }
    free(particao_temp);

    end = clock();

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    //printf("\nTempo de execucao: %f segundos\n", cpu_time_used);

    printf("\n-----INTERCALANDO PARTICOES COM INTERCALACAO OTIMA-----\n");
    intercalacao_otima_func(arq, qnt_part, "funcionarios_ordenados.dat", cpu_time_used);
}

void intercalacao_otima_func(FILE *arq, int qnt_part, const char *nome_base, float time_used)
{
    clock_t start, end;
    double cpu_time_used;

    start = clock();

    // Abre o arquivo de saída para a intercalação final
    FILE *saida;
    if ((saida = fopen(nome_base, "w+b")) == NULL)
    {
        printf("Erro ao abrir arquivo de saida\n");
        exit(1);
    }

    // Abre os arquivos das partições para leitura e intercalação
    FILE **particoes = malloc(sizeof(FILE *) * qnt_part);
    for (int i = 0; i < qnt_part; i++)
    {
        char nome_particao[16];
        sprintf(nome_particao, "particao-%02d.dat", i + 1);

        //printf("\n%d", i+1);

        if ((particoes[i] = fopen(nome_particao, "rb")) == NULL)
        {
            printf("Erro ao abrir arquivo de particao %d\n", i + 1);
            exit(1);
        }
        //printf("\nteste\n");
        //puts(nome_particao);
        //fclose(nome_particao);
    }

    // Array para armazenar temporariamente o próximo registro de cada partição
    TFunc *registro_atual = malloc(sizeof(TFunc) * qnt_part);

    // Leitura inicial do primeiro registro de cada partição
    for (int i = 0; i < qnt_part; i++)
    {
        if (fread(&registro_atual[i], sizeof(TFunc), 1, particoes[i]) != 1)
        {
            // Se a partição estiver vazia, coloca um valor máximo no registro para evitar problemas
            registro_atual[i].cod = INT_MAX;
        }
    }

    // Intercalação dos registros até que todas as partições estejam vazias
    while (1)
    {
        // Encontra o menor registro atual dentre todas as partições
        int menor_indice = -1;
        for (int i = 0; i < qnt_part; i++)
        {
            if (registro_atual[i].cod != INT_MAX &&
                (menor_indice == -1 || registro_atual[i].cod < registro_atual[menor_indice].cod))
            {
                menor_indice = i;
            }
        }

        // Se todas as partições estão vazias, a intercalação está completa
        if (menor_indice == -1)
        {
            break;
        }

        // Escreve o menor registro no arquivo de saída
        fwrite(&registro_atual[menor_indice], sizeof(TFunc), 1, saida);

        // Lê o próximo registro da partição que tinha o menor registro
        if (fread(&registro_atual[menor_indice], sizeof(TFunc), 1, particoes[menor_indice]) != 1)
        {
            // Se a partição estiver vazia, coloca um valor máximo no registro para evitar problemas
            registro_atual[menor_indice].cod = INT_MAX;
        }
    }

    end = clock();

    printf("\nDeseja imprimir as base completa ordenada?\nDigite 1 para SIM ou 0 para NAO: ");
    int opcao;
    scanf("%d", &opcao);

    if (opcao != 0)
    {
        printf("\n\n\n-+-+-+-+-+-+-+-+-+-+- BASE ORDENADA -+-+-+-+-+-+-+-+-+-+-");
        le_funcionarios(saida);
    }

    printf("\nBASE ATUALIZADA PARA SUA VERSAO ORDENADA!\n");

    printf("\nLIBERANDO MEMORIA...\n");
    // Fecha os arquivos das partições e o arquivo de saída
    for (int i = 0; i < qnt_part; i++)
    {
        fclose(particoes[i]);
        //remover os arquivo de partições
        char nome_particao[20];
        sprintf(nome_particao, "particao-%02d.dat", i + 1);
        remove(nome_particao);
    }

    // Libera a memória alocada
    fclose(saida);

    // Copia o conteúdo do arquivo ordenado para o arquivo original
    copiar_arquivo_func(arq, "funcionarios_ordenados.dat");

    remove("funcionarios_ordenados.dat");
    free(particoes);
    free(registro_atual);

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\nTempo de execucao: %f segundos\n\n\n", cpu_time_used + time_used);

    //salvar o tempo de execução em um arquivo txt
    FILE *resultado;
    resultado = fopen("resultados.txt", "a");

    if (resultado == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        menuInicialFuncionario(arq);
        //break;
    }

    fprintf(resultado, "-----------------------------\n");
    fprintf(resultado, "Ordenação com Particionamento e Intercalação Otima\n");
    fprintf(resultado, "Tempo de execucao: %f segundos\n", cpu_time_used + time_used);
    fprintf(resultado, "Tamanho da base: %d\n", tamanho_total_funcionario(arq));
    fprintf(resultado, "Tamanho da particao: %d\n", tamanho_total_funcionario(arq) / qnt_part);
    fprintf(resultado, "Quantidade de particoes: %d\n", qnt_part);
    fprintf(resultado, "-----------------------------\n\n\n");

    fclose(resultado);
}

void copiar_arquivo_func(FILE *arq_destino, const char *nome_arquivo_origem)
{

    // Abre o arquivo de destino para escrita binária (truncando o arquivo)
    //FILE *arq_destino;
    if ((arq_destino = fopen("funcionario.dat", "wb")) == NULL)
    {
        printf("Erro ao abrir arquivo de destino\n");
        return;
    }

    // Abre o arquivo de origem para leitura binária
    FILE *arq_origem;
    if ((arq_origem = fopen(nome_arquivo_origem, "rb")) == NULL)
    {
        printf("Erro ao abrir arquivo de origem\n");
        fclose(arq_destino); // Fecha o arquivo de destino antes de retornar
        return;
    }

    TFunc funcionario;

    while (fread(&funcionario, sizeof(TFunc), 1, arq_origem) == 1)
    {
        fwrite(&funcionario, sizeof(TFunc), 1, arq_destino);
    }

    fclose(arq_origem);
    fclose(arq_destino);
}

void selececao_por_substituicao_func(TFunc *arr, int size)
{
    int i, j, min_idx;
    for (i = 0; i < size - 1; i++)
    {
        min_idx = i;
        for (j = i + 1; j < size; j++)
        {
            if (arr[j].cod < arr[min_idx].cod)
            {
                min_idx = j;
            }
        }
        if (min_idx != i)
        {
            TFunc temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;
        }
    }
}

void selection_sort_disco_funcionario(FILE *arq)
{
    //ordenação pelo atributo cod de TFunc
    int tam = tamanho_total_funcionario(arq);
    int i, j, min, cont = 0;
    //faz o selection sort
    for (i = 1; i < tam; i++)
    {
        //posiciona o arquivo no registro i
        fseek(arq, (i - 1) * tamanho(), SEEK_SET);
        TFunc *fi = le(arq);
        //printf("\n********* Funcionario atual: %d\n", fi->cod);
        min = i;
        for (j = i + 1; j <= tam; j++)
        {
            //posiciona o arquivo no registro j
            fseek(arq, (j - 1) * tamanho(), SEEK_SET);
            TFunc *fj = le(arq);
            //printf("fj = %d\n", fj->cod);
            if (fj->cod < fi->cod)
            {
                min = j;
                //printf("min = %d\n", min);
                //posiciona o arquivo no registro min
                fseek(arq, (min - 1) * tamanho(), SEEK_SET);
                TFunc *fmin = le(arq);
                //printf("fmin = %d\n", fmin->cod);
                //troca os registros
                fseek(arq, (i - 1) * tamanho(), SEEK_SET);
                salva_funcionario(fmin, arq);
                fseek(arq, (min - 1) * tamanho(), SEEK_SET);
                salva_funcionario(fi, arq);
                //posiciona o arquivo no registro i
                fseek(arq, (i - 1) * tamanho(), SEEK_SET);
                fi = le(arq);
                //printf("fi = %d\n", fi->cod);
                cont++;
                free(fmin);
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
    int i, cont = 0;
    //faz o insertion sort
    for (int j = 2; j <= tam; j++)
    {
        //posiciona o arquivo no registro j
        fseek(arq, (j - 1) * tamanho(), SEEK_SET);
        TFunc *fj = le(arq);
        //printf("\n********* Funcionario atual: %d\n", fj->cod);
        i = j - 1;
        //posiciona o cursor no registro i
        fseek(arq, (i - 1) * tamanho(), SEEK_SET);
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
            fseek(arq, (i - 1) * tamanho(), SEEK_SET);
            fi = le(arq);
            //printf("fi = %d; i = %d\n", fi->cod, i);
            cont++;
        }
        //posiciona cursor no registro i + 1
        fseek(arq, (i)*tamanho(), SEEK_SET);
        //printf("*** salva_funcionariondo funcionario %d na posicao %d\n", fj->cod, i+1);
        //salva_funcionario registro j na posiÃ§Ã£o i
        salva_funcionario(fj, arq);
        if (fi != NULL)
        {
            free(fi);
        }
        if (fj != NULL)
        {
            free(fj);
        }
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
        v[i + 1] = f;
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
    int i = 0;
    while ((f = le(in)) != NULL)
    {
        free(f);
        i++;
    }
    return i;
}

void le_funcionarios(FILE *in)
{
    if (tamanho_total_funcionario(in) == 0)
    {
        printf("\nArquivo vazio!\n\n");
        return;
    }
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
