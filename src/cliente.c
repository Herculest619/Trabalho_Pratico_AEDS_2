#include "cliente.h"
#include "funcionario.h"
#include "main.h"
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void menuInicialCliente(FILE *clien)
{
    int opcao;
    printf("**********************************************\n");
    printf("----MENU CLIENTE----\n");
    printf("1: CRIAR BASE\n");
    printf("2: IMPRIMIR BASE\n");
    printf("3: BUSCA\n");
    printf("4: ORDENAR BASE\n");
    printf("5: EXCLUIR BASE\n");
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

    case 5:
        system("cls");
        printf("----------EXCLUIR BASE----------\n");
        printf("\nDeseja excluir a base de clientes?\nDigite 1 para SIM ou 0 para NAO: ");
        int opcao;
        scanf("%d", &opcao);

        if (opcao == 1)
        {
            fclose(clien);
            remove("cliente.dat");
            clien = fopen("cliente.dat", "w+b");
            printf("\nBase de clientes excluida!\n\n");
        }
        else
        {
            printf("\nBase de clientes mantida!\n\n");
        }

        menuInicialCliente(clien);
        break;

    case 9:
        system("cls");
        main();
        break;
    }
}

void menuCriarBaseCliente(FILE *clien)
{
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

Tclien *cliente(int cod, char *nome, char *cpf, char *data, double mensalidade)
{
    // Aloca memória para o cliente
    Tclien *clien = (Tclien *)malloc(sizeof(Tclien));
    //inicializa espaço de memória com ZEROS
    if (clien)
        memset(clien, 0, sizeof(Tclien));

    clien->cod = cod;
    strcpy(clien->nome, nome);
    strcpy(clien->cpf, cpf);
    strcpy(clien->data_nascimento, data);
    clien->mensalidade = mensalidade;

    return clien;
}

void criarBaseClientesOrdenada(FILE *clien)
{
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

void criarBaseClientesDesordenada(FILE *clien)
{
    int n;
    printf("\nDigite a quantidade de clientes a serem inseridos desordenados: ");
    scanf("%d", &n);
    printf("\nInserindo %d clientes no arquivo...\n", n);
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
        Tclien *c1 = cliente(indices[i], nome, cpf, data, gerarMensalidade(i));
        salva_cliente(c1, clien);
        free(c1);
    }

    free(indices);
}

void salva_cliente(Tclien *clien, FILE *clien_file)
{

    fwrite(&clien->cod, sizeof(int), 1, clien_file);
    fwrite(clien->nome, sizeof(char), sizeof(clien->nome), clien_file);
    fwrite(clien->cpf, sizeof(char), sizeof(clien->cpf), clien_file);
    fwrite(clien->data_nascimento, sizeof(char), sizeof(clien->data_nascimento), clien_file);
    fwrite(&clien->mensalidade, sizeof(double), 1, clien_file);
}

double gerarMensalidade(int i)
{
    // gerar mensalidade aleatória entre R$59,90 e R$199,90
    double mensalidade = ((rand() * i) % 14000) + 5990;
    mensalidade = mensalidade / 100;
    return mensalidade;
}

void imprime_cliente(Tclien *clien)
{
    printf("**********************************************");
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
    printf("\n**********************************************");
    printf("\n\n\n");
}

void le_cliente(FILE *in)
{
    if (tamanho_total_cliente(in) == 0)
    {
        printf("\n\nArquivo vazio!\n\n");
        return;
    }
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
    Tclien *clien = (Tclien *)malloc(sizeof(Tclien));
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

        Tclien *clien = busca_sequencial_cliente(num, out, tamanho_total_cliente(out));

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

        //salvar o tempo de execução em um arquivo txt

        FILE *resultado;
        resultado = fopen("resultados.txt", "a");

        if (resultado == NULL) {
            printf("Erro ao abrir o arquivo.\n");
            menuInicialCliente(out);
            break;
        }

        fprintf(resultado, "-----------------------------\n");
        fprintf(resultado, "Busca sequencial\n");
        fprintf(resultado, "Tempo de execucao: %f segundos\n", tempo_execucao);
        fprintf(resultado, "Tamanho da base: %d\n", tamanho_total_cliente(out));
        fprintf(resultado, "-----------------------------\n\n\n");

        fclose(resultado);

        menuInicialCliente(out);
        break;

    case 2:
        system("cls");
        int indice;
        printf("----------BUSCA BINARIA----------\n");
        printf("\nDigite o indice a ser procurado: ");
        scanf("%d", &indice);

        inicio = clock();

        Tclien *clien1 = busca_binaria_cliente(indice, out, tamanho_total_cliente(out));

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

        //salvar o tempo de execução em um arquivo txt

        FILE *resultado1;
        resultado1 = fopen("resultados.txt", "a");

        if (resultado1 == NULL)
        {
            printf("Erro ao abrir o arquivo.\n");
            menuInicialCliente(out);
            break;
        }

        fprintf(resultado1, "-----------------------------\n");
        fprintf(resultado1, "Busca binaria\n");
        fprintf(resultado1, "Tempo de execucao: %f segundos\n", tempo_execucao);
        fprintf(resultado1, "Tamanho da base: %d\n", tamanho_total_cliente(out));
        fprintf(resultado1, "-----------------------------\n\n\n");

        fclose(resultado1);

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

Tclien *busca_sequencial_cliente(int cod, FILE *arq, int tam)
{
    rewind(arq); // Reinicia o ponteiro de arquivo para o início

    for (int i = 0; i < tam; i++)
    {
        Tclien *clien = ler(arq);
        if (clien != NULL && clien->cod == cod)
        {
            printf("\n**********************************************");
            printf("\nForam gastos %d iteracoes para encontrar o cliente!\n", i + 1);
            return clien; // Cliente encontrado
        }
        free(clien);
    }

    return NULL; // Cliente não encontrado
}

Tclien *busca_binaria_cliente(int cod, FILE *arq, int tam)
{
    int count = 0;
    int left = 0, right = tam - 1;
    while (left <= right)
    {
        count++;
        int middle = (left + right) / 2;
        fseek(arq, middle * tamanho(), SEEK_SET);
        Tclien *clien = ler(arq);
        if (cod == clien->cod)
        {
            printf("\n**********************************************");
            printf("\nForam gastos %d iteracoes para encontrar o cliente!\n", count);
            return clien;
        }
        else if (clien->cod < cod)
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
    int i = 0;
    while ((c = ler(in)) != NULL)
    {
        free(c);
        i++;
    }
    return i;
}

void menuOrdenarCliente(FILE *out)
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

        menuOrdenarClienteDisco(out);

        break;

    case 2:

        system("cls");

        criar_particoes_clien(out);

        menuInicialCliente(out);
        break;

    case 9:
        system("cls");
        menuInicialCliente(out);
        break;

    default:
        system("cls");
        printf("\n-----EXIT-----\n");
    }
}

void criar_particoes_clien(FILE *arq)
{
    int tam = tamanho_total_cliente(arq);
    float tam_part;
    int qnt_part;

    printf("\nDigite o tamanho de particoes (MAX = %d): ", tam);
    scanf("%f", &tam_part);

    if (tam_part > tam)
    {
        printf("\nNao e possivel criar uma particao maior que o tamanho total da base.\n\n");
        criar_particoes_clien(arq);
    }
    else if (tam_part <= 0)
    {
        printf("\nNao e possivel criar uma particao com o tamanho igual ou menor a 0.\n\n");
        criar_particoes_clien(arq);
    }

    qnt_part = (int)ceil(tam / tam_part);

    printf("\nQuantidade de particoes: %d\n", qnt_part);

    // Array para armazenar temporariamente os registros da partição em RAM
    Tclien *particao_temp = malloc(sizeof(Tclien) * tam_part);

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
            fseek(arq, inicio * sizeof(Tclien), SEEK_SET);
            fread(particao_temp, sizeof(Tclien), fim - inicio, arq);

            // Ordenação da partição em RAM usando a Seleção por substituição
            selececao_por_substituicao_clien(particao_temp, fim - inicio);

            fwrite(particao_temp, sizeof(Tclien), fim - inicio, particao);

            if (opcao != 0)
            {
                printf("\n\n\n-+-+-+-+-+-+-+-+- PARTICAO %d -+-+-+-+-+-+-+-+-", i);
                le_cliente(particao);
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
    intercalacao_otima_clien(arq, qnt_part, "clientes_ordenados.dat", cpu_time_used);
}

void selececao_por_substituicao_clien(Tclien *arr, int size)
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
            Tclien temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;
        }
    }
}

void intercalacao_otima_clien(FILE *arq, int qnt_part, const char *nome_base, float time_used)
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
    Tclien *registro_atual = malloc(sizeof(Tclien) * qnt_part);

    // Leitura inicial do primeiro registro de cada partição
    for (int i = 0; i < qnt_part; i++)
    {
        if (fread(&registro_atual[i], sizeof(Tclien), 1, particoes[i]) != 1)
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
        fwrite(&registro_atual[menor_indice], sizeof(Tclien), 1, saida);

        // Lê o próximo registro da partição que tinha o menor registro
        if (fread(&registro_atual[menor_indice], sizeof(Tclien), 1, particoes[menor_indice]) != 1)
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
        le_cliente(saida);
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
    copiar_arquivo_clien(arq, "clientes_ordenados.dat");

    remove("clientes_ordenados.dat");
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
        menuInicialCliente(arq);
        //break;
    }

    fprintf(resultado, "-----------------------------\n");
    fprintf(resultado, "Ordenação com Particionamento e Intercalação Otima\n");
    fprintf(resultado, "Tempo de execucao: %f segundos\n", cpu_time_used + time_used);
    fprintf(resultado, "Tamanho da base: %d\n", tamanho_total_cliente(arq));
    fprintf(resultado, "Tamanho da particao: %d\n", tamanho_total_cliente(arq) / qnt_part);
    fprintf(resultado, "Quantidade de particoes: %d\n", qnt_part);
    fprintf(resultado, "-----------------------------\n\n\n");

    fclose(resultado);
}

void copiar_arquivo_clien(FILE *arq_destino, const char *nome_arquivo_origem)
{
    // Abre o arquivo de destino para escrita binária (truncando o arquivo)
    //FILE *arq_destino;
    if ((arq_destino = fopen("cliente.dat", "w+b")) == NULL)
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

    Tclien cliente;

    while (fread(&cliente, sizeof(Tclien), 1, arq_origem) == 1)
    {
        fwrite(&cliente, sizeof(Tclien), 1, arq_destino);
    }

    fclose(arq_origem);
    fclose(arq_destino);
}

void menuOrdenarClienteDisco(FILE *out)
{
    printf("-----ORDENAR BASE-----\n");
    printf("1: INSERTION SORT\n");
    printf("2: SELECTION SORT\n");
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

        //salvar o tempo de execução em um arquivo txt
        FILE *resultado;
        resultado = fopen("resultados.txt", "a");

        if (resultado == NULL) {
            printf("Erro ao abrir o arquivo.\n");
            menuInicialCliente(out);
            break;
        }

        fprintf(resultado, "-----------------------------\n");
        fprintf(resultado, "Ordenção em disco usando Insertion Sort\n");
        fprintf(resultado, "Tempo de execucao: %f segundos\n", tempo_execucao);
        fprintf(resultado, "Tamanho da base: %d\n", tamanho_total_cliente(out));
        fprintf(resultado, "-----------------------------\n\n\n");

        fclose(resultado);

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

        //salvar o tempo de execução em um arquivo txt
        FILE *resultadob;
        resultadob = fopen("resultados.txt", "a");

        if (resultadob == NULL) {
            printf("Erro ao abrir o arquivo.\n");
            menuInicialCliente(out);
            break;
        }

        fprintf(resultadob, "-----------------------------\n");
        fprintf(resultadob, "Ordenção em disco usando Selection Sort\n");
        fprintf(resultadob, "Tempo de execucao: %f segundos\n", tempo_execucao);
        fprintf(resultadob, "Tamanho da base: %d\n", tamanho_total_cliente(out));
        fprintf(resultadob, "-----------------------------\n\n\n");

        fclose(resultadob);

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
    int i, cont = 0;
    //faz o insertion sort
    for (int j = 2; j <= tam; j++)
    {
        //posiciona o arquivo no registro j
        fseek(arq, (j - 1) * tamanho(), SEEK_SET);
        Tclien *fj = ler(arq);
        //printf("\n********* Cliente atual: %d\n", fj->cod);
        i = j - 1;
        //posiciona o cursor no registro i
        fseek(arq, (i - 1) * tamanho(), SEEK_SET);
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
            fseek(arq, (i - 1) * tamanho(), SEEK_SET);
            fi = ler(arq);
            //printf("fi = %d; i = %d\n", fi->cod, i);
            cont++;
        }
        //posiciona cursor no registro i + 1
        fseek(arq, (i)*tamanho(), SEEK_SET);
        //printf("*** salva_cliente cliente %d na posicao %d\n", fj->cod, i+1);
        //salva_cliente registro j na posiÃ§Ã£o i
        salva_cliente(fj, arq);
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

void selection_sort_disco_cliente(FILE *arq)
{
    //ordenação pelo atributo cod de Tclien
    int tam = tamanho_total_cliente(arq);
    int i, j, min, cont = 0;
    //faz o selection sort
    for (i = 1; i < tam; i++)
    {
        //posiciona o arquivo no registro i
        fseek(arq, (i - 1) * tamanho(), SEEK_SET);
        Tclien *fi = ler(arq);
        //printf("\n********* cliente atual: %d\n", fi->cod);
        min = i;
        for (j = i + 1; j <= tam; j++)
        {
            //posiciona o arquivo no registro j
            fseek(arq, (j - 1) * tamanho(), SEEK_SET);
            Tclien *fj = ler(arq);
            //printf("fj = %d\n", fj->cod);
            if (fj->cod < fi->cod)
            {
                min = j;
                //printf("min = %d\n", min);
                //posiciona o arquivo no registro min
                fseek(arq, (min - 1) * tamanho(), SEEK_SET);
                Tclien *fmin = ler(arq);
                //printf("fmin = %d\n", fmin->cod);
                //troca os registros
                fseek(arq, (i - 1) * tamanho(), SEEK_SET);
                salva_cliente(fmin, arq);
                fseek(arq, (min - 1) * tamanho(), SEEK_SET);
                salva_cliente(fi, arq);
                //posiciona o arquivo no registro i
                fseek(arq, (i - 1) * tamanho(), SEEK_SET);
                fi = ler(arq);
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
