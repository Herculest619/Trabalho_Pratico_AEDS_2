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
    //testar se a partição 0 existe, caso exista ir para o menuHashCliente
    FILE *particao;
    char nomeArquivo[20];
    sprintf(nomeArquivo, "hashTablePartition0.dat");
    if ((particao = fopen(nomeArquivo, "r+b")) != NULL)
    {
        fclose(particao);
        menuHashCliente(clien);
    }

    int opcao;
    printf("**********************************************\n");
    printf("----MENU CLIENTE----\n");
    printf("1: CRIAR BASE\n");
    printf("2: IMPRIMIR BASE\n");
    printf("3: BUSCA\n");
    printf("4: ORDENAR BASE\n");
    printf("5: EXCLUIR BASE\n");
    printf("6: TRASFORMAR EM TABELA HASH\n");
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

    case 6:
        system("cls");
        hashTable(clien);
        break;

    case 9:
        system("cls");
        main();
        break;

    case 0:
        system("cls");
        printf("EXIT\n");
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
    printf("\nCONCLUIDO\n");
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
        resultado = fopen("resultados_tempo.txt", "a");

        if (resultado == NULL)
        {
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
        resultado1 = fopen("resultados_tempo.txt", "a");

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

        //system("cls");
        printf("\nteste\n");
        //selececao_por_substituicao_cliente(out);
        int qnt_part = substituicao_com_selecao(out, "particao_");
        intercalacao_otima_clien(out, qnt_part+1, "particao_");
        printf("\nteste2\n");

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

int allVetFrozen (int vet[], int M ) {

    int cont = 0;

    for (int i = 0; i < M; ++i) {
        if (vet[i] == 1) {
            cont++;
        }
    }

    if (cont == M) {
        return 1;
    } else {
        return 0;
    }
}

void printPartitionEmployeeID(FILE *file, char nome_particao[])
{

    printf("\nParticao %s:  ", nome_particao);

    for (int i = 0; i < sizeFile(file, 0); ++i)
    {

        fseek(file, i * sizeof(Tclien), SEEK_SET);
        Tclien *aux = ler_registro_Tclien(file);

        printf(" %i ", aux->cod);
    }

    printf("\n");
}

Tclien *ler_registro_Tclien(FILE *in)
{
    /*
    typedef struct {
        int cod;
        char nome[50];
        char cpf[15];
        char data_nascimento[11];
        double mensalidade;
    } Tclien;
    */

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

int tamanho_cliente_bytes()
{
    return sizeof(int)         //cod
           + sizeof(char) * 50 //nome
           + sizeof(char) * 15 //cpf
           + sizeof(char) * 11 //data_nascimento
           + sizeof(double);   //mensalidade
}

void saveRegisterEmployee(Tclien *employee, FILE *file)
{
    fwrite(&(employee->cod), sizeof(int), 1, file);
    fwrite(employee->nome, sizeof(char), sizeof(employee->nome), file);
    fwrite(employee->cpf, sizeof(char), sizeof(employee->cpf), file);
    fwrite(employee->data_nascimento, sizeof(char), sizeof(employee->data_nascimento), file);
    fwrite(&(employee->mensalidade), sizeof(double), 1, file);
}

int sizeFile(FILE *file, int contSizeFile) {

    int bytesAUX = 0;

    while(!feof(file)) {

        fseek(file, bytesAUX * sizeof(Tclien), SEEK_SET);

        Tclien *aux = ler_registro_Tclien(file);
        if(aux != NULL) {
            contSizeFile++;
        }

        bytesAUX++;
    }

    return contSizeFile;
}

int substituicao_com_selecao(FILE *file, char nome_arq_particao[])
{
    //int M = (int)ceil(sqrt(tamanho_total_cliente(file)));
    int M =6;
    int posicao = M;

    int num_particao = 0, contSizeFile = 0, menor_elementoposicao = 0, menor_elemento = 999999, sizeFileAux = 0;

    Tclien *clientes = (Tclien *)malloc(sizeof(Tclien) * M);

    int *vetor_aux_clien = (int *)calloc(M, sizeof(int));

    rewind(file);

    /*
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return -1;
    }*/

    sizeFileAux = sizeFile(file, contSizeFile);

    printf("\nRealizando selecao por substituicao...");

    for (int i = 0; i < M; ++i)
    {
        fseek(file, i * sizeof(Tclien), SEEK_SET);

        Tclien *aux = ler_registro_Tclien(file);
        clientes[i] = *aux;
    }

    for (int i = 0; i < M; ++i)
    {
        vetor_aux_clien[i] = clientes[i].cod;
    }

    while (posicao != sizeFileAux)
    {
        char nome_particao[100];
        char str1[100];
        char str2[100] = ".dat";

        itoa(num_particao, str1, 10);
        strcat(strcpy(nome_particao, nome_arq_particao), str1);
        strcat(strcpy(nome_particao, nome_particao), str2);

        FILE *filePartition = fopen(nome_particao, "wb+");

        int *vetor_congelado_aux = (int *)calloc(M, sizeof(int));

        while (posicao != sizeFileAux) // Adicionei a verificação para verificar se a partição atual já foi completamente preenchida
        {
            menor_elemento = 9999999;

            for (int i = 0; i < M; ++i)
            {
                if (menor_elemento > vetor_aux_clien[i] && vetor_congelado_aux[i] != 1)
                {
                    menor_elemento = vetor_aux_clien[i];
                    menor_elementoposicao = i;
                }
            }

            saveRegisterEmployee(&clientes[menor_elementoposicao], filePartition);

            fseek(file, posicao * sizeof(Tclien), SEEK_SET);

            Tclien *aux = ler_registro_Tclien(file);
            /*
            if (aux == NULL) {
                printf("Erro ao ler registro do arquivo.\n");
                return -1;
            }*/

            posicao++;

            vetor_aux_clien[menor_elementoposicao] = aux->cod;
            clientes[menor_elementoposicao] = *aux;

            if (aux->cod < menor_elemento)
            {
                vetor_congelado_aux[menor_elementoposicao] = 1;
            }

            if(allVetFrozen(vetor_congelado_aux, M) == 1) {
                num_particao++;
                break;
            }
        }

        fclose(filePartition);

        if (posicao >= sizeFileAux)
        {
            break;
        }
    }

    char nome_particao[100];
    char str1[100];
    char str2[100] = ".dat";

    itoa(num_particao, str1, 10);
    strcat(strcpy(nome_particao, nome_arq_particao), str1);
    strcat(strcpy(nome_particao, nome_particao), str2);

    FILE *filePartitionFinal = fopen(nome_particao, "ab+");

    Tclien ClienAux;

    // Ordena os registros da partição
    for (int k = 1; k < M; k++)
    {

        for (int j = 0; j < M-k; j++)
        {

            if (clientes[j].cod > clientes[j + 1].cod)
            {
                ClienAux = clientes[j];
                clientes[j] = clientes[j + 1];
                clientes[j + 1] = ClienAux;
            }
        }
    }

    rewind(filePartitionFinal);

    // Salva os registros ordenados no arquivo da partição
    for (int i = 0; i < M; ++i)
    {
        saveRegisterEmployee(&clientes[i], filePartitionFinal);
    }

    fclose(filePartitionFinal);


    //vamos abrir a ultima partição e ordenar ela
    char nome_particao2[100];
    char str3[100];
    char str4[100] = ".dat";

    itoa(num_particao, str3, 10);
    strcat(strcpy(nome_particao2, nome_arq_particao), str3);
    strcat(strcpy(nome_particao2, nome_particao2), str4);

    FILE *filePartitionFinal2 = fopen(nome_particao2, "rb+");

    //quantidade de elementos na partição
    int sizeFileAux2 = sizeFile(filePartitionFinal2, contSizeFile);
    printf("\nsizeFileAux2: %d\n", sizeFileAux2);


    Tclien *clientes2 = (Tclien *)malloc(sizeof(Tclien) * sizeFileAux2);

    // Lê os registros da partição
    for (int i = 0; i < sizeFileAux2; ++i)
    {
        fseek(filePartitionFinal2, i * sizeof(Tclien), SEEK_SET);

        Tclien *aux = ler_registro_Tclien(filePartitionFinal2);
        clientes2[i] = *aux;
    }

    Tclien ClienAux2;

    // Ordena os registros da partição
    for (int k = 0; k < sizeFileAux2 -1; k++)
    {

        for (int j = 0; j < sizeFileAux2-1-k; j++)
        {

            if (clientes2[j].cod > clientes2[j + 1].cod)
            {
                ClienAux2 = clientes2[j];
                clientes2[j] = clientes2[j + 1];
                clientes2[j + 1] = ClienAux2;
            }
        }
    }

/*
    for (int k = 0; k < sizeFileAux2; k++)
    {
        printf("\nteste");
        printf("\n%d\n", clientes2[k].cod);
    }*/

    rewind(filePartitionFinal2);
    // Salva os registros ordenados no arquivo da partição
    for (int i = 0; i < sizeFileAux2; i++)
    {
        saveRegisterEmployee(&clientes2[i], filePartitionFinal2);
    }

    fclose(filePartitionFinal2);




    // Imprime o conteúdo de cada partição
    for (int i = 0; i <= num_particao; ++i)
    {
        //printf("\n---%d\n", num_particao);

        itoa(i, str1, 10);
        strcat(strcpy(nome_particao, nome_arq_particao), str1);
        strcat(strcpy(nome_particao, nome_particao), str2);

        FILE *filePartition = fopen(nome_particao, "rb+"); // Abre o arquivo da partição para leitura e escrita

        //printf("\nteste %s", nome_particao);

        printPartitionEmployeeID(filePartition, nome_particao);

        fclose(filePartition);
    }

    free(vetor_aux_clien);
    free(clientes);
    free(clientes2);

    //printf("\n%d\n", num_particao);
    return num_particao;
}

void intercalacao_otima_clien(FILE *arq, int qnt_part, char nome_base[]) {
    FILE **partFiles = (FILE **)malloc(sizeof(FILE *) * qnt_part);

    for (int i = 0; i < qnt_part; i++) {
        char partitionFileName[100];
        snprintf(partitionFileName, sizeof(partitionFileName), "%s%d.dat", nome_base, i);
        partFiles[i] = fopen(partitionFileName, "rb");
    }

    /*
    //imprimir as partições a partir do partFiles
    for(int i = 0; i < qnt_part; i++) {
        printf("\nParticao %d\n", i);

        Tclien registro;
        while (fread(&registro, sizeof(Tclien), 1, partFiles[i]) == 1) {
            printf("\n%d\n", registro.cod);
        }

        rewind(partFiles[i]);
    }
    */

    int currentPartitions = qnt_part; // Quantidade de partições que ainda não foram intercaladas

    int x=1;
    while (currentPartitions > 1) {
        FILE *mergedFile = tmpfile(); // Cria um arquivo temporário para a intercalação

        printf("\n\n\nteste intercalacao while %d\n", x);

        // Encontra as duas menores partições para intercalar
        int Part1 = currentPartitions - 1;
        int Part2 = currentPartitions - 2;

        /*for (int i = 0; i < currentPartitions; i++) {
            if (partFiles[i] != NULL) {
                if (Part1 == -1 || Part2 == -1) {
                    if (Part1 == -1) {
                        Part1 = i;
                    } else {
                        Part2 = i;
                    }
                } else {
                    if (comparaRegistros(partFiles[i], partFiles[Part1]) < 0) {
                        Part2 = Part1;
                        Part1 = i;
                    } else if (comparaRegistros(partFiles[i], partFiles[Part2]) < 0) {
                        Part2 = i;
                    }
                }
            }
        }*/

        //printf("\npassou do for\n");
        printf("\nPart1: %d\n", Part1);
        printf("\nPart2: %d\n", Part2);


        // Intercale as duas partições
        intercalarDuasParticoes(partFiles[Part1], partFiles[Part2], mergedFile);

        printf("\npassou da intercalacao\n");

        // Feche os arquivos das partições intercaladas
        fclose(partFiles[Part1]);
        fclose(partFiles[Part2]);

        // Atualize o array de arquivos de partições com o arquivo temporário intercalado
        partFiles[Part2] = mergedFile;



        // Atualiza o array de arquivos de partições com o arquivo temporário que está com a intercalação
        //copiarRegistros(mergedFile, partFiles[Part2]);

        //partFiles[Part2] = mergedFile;

        //impirmir a partição intercalada
        rewind(partFiles[Part2]);
        Tclien registro;
        printf("\nParticao intercalada dentro da funcao intercalacao_otima_clien\n");
        while (fread(&registro, sizeof(Tclien), 1, partFiles[Part2]) == 1) {
            printf("%d\n", registro.cod);
        }

        //partFiles[Part2] = partFiles[currentPartitions - 1];

        // Fecha os arquivos das partições intercaladas
        //fclose(partFiles[Part1]);
        //fclose(partFiles[Part2]);
        //fclose(mergedFile);

        // Redefina o arquivo temporário mergedFile
        mergedFile = tmpfile();
        rewind(partFiles[Part1]);
        rewind(partFiles[Part2]);


        currentPartitions--;
        x++;
    }
    printf("\nteste intercalacao 3\n");

    // O último arquivo restante contém a saída final ordenada

    // Copia os registros do arquivo final ordenado para o arquivo original
    rewind(partFiles[0]);
    copiarRegistros(partFiles[0], arq);


    // Imprime o arquivo todo ordenado
    rewind(arq);
    Tclien registro;
    printf("\nArquivo Ordenado:\n");

    while (fread(&registro, sizeof(Tclien), 1, arq) == 1) {
        printf("%d\n", registro.cod);
    }


    for (int i = 0; i < qnt_part; ++i) {
        fclose(partFiles[i]);
    }

    free(partFiles);
    printf("\nacabou a intercalacao");
}

// Função para comparar dois registros
int comparaRegistros(const void *a, const void *b) {
    const Tclien *registro1 = (const Tclien *)a;
    const Tclien *registro2 = (const Tclien *)b;

    if (registro1->cod < registro2->cod) {
        return -1;
    } else if (registro1->cod > registro2->cod) {
        return 1;
    } else {
        return 0;
    }
}

// Função para intercalar duas partições
void intercalarDuasParticoes(FILE *part1, FILE *part2, FILE *mergedFile) {
    Tclien registro1, registro2;

    // Leia o primeiro registro de cada partição
    if (fread(&registro1, sizeof(Tclien), 1, part1) != 1) {
        printf("\nA primeira particao está vazia, então apenas escreva os registros da segunda");
        return; // A partição está vazia
    }
    if (fread(&registro2, sizeof(Tclien), 1, part2) != 1) {
        // A segunda partição está vazia, então apenas escreva os registros da primeira
        printf("\nA segunda particao está vazia, então apenas escreva os registros da primeira");
        fwrite(&registro1, sizeof(Tclien), 1, mergedFile);
        while (fread(&registro1, sizeof(Tclien), 1, part1) == 1) {
            fwrite(&registro1, sizeof(Tclien), 1, mergedFile);
        }
        return;
    }

    while (1) {
        if (comparaRegistros(&registro1, &registro2) <= 0) {
            fwrite(&registro1, sizeof(Tclien), 1, mergedFile);
            if (fread(&registro1, sizeof(Tclien), 1, part1) != 1) {
                break;
            }
        } else {
            fwrite(&registro2, sizeof(Tclien), 1, mergedFile);
            if (fread(&registro2, sizeof(Tclien), 1, part2) != 1) {
                break;
            }
        }
    }

    // Escreva os registros restantes de part1, se houver
    while (fread(&registro1, sizeof(Tclien), 1, part1) == 1) {
        fwrite(&registro1, sizeof(Tclien), 1, mergedFile);
    }

    // Escreva os registros restantes de part2, se houver
    while (fread(&registro2, sizeof(Tclien), 1, part2) == 1) {
        fwrite(&registro2, sizeof(Tclien), 1, mergedFile);
    }

    //imprime o arquivo ordenado
    rewind(mergedFile);
    Tclien registro;
    printf("\n\n\nProcesso de intercalacao dentro da funcao intercalarDuasParticoes\n");

    while (fread(&registro, sizeof(Tclien), 1, mergedFile) == 1) {
        printf("%d\n", registro.cod);
    }
    rewind(mergedFile);
}

// Função para copiar registros de um arquivo para outro
void copiarRegistros(FILE *origem, FILE *destino) {
    Tclien registro;

    while (fread(&registro, sizeof(Tclien), 1, origem) == 1)
    {
        fwrite(&registro, sizeof(Tclien), 1, destino);
    }
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
        resultado = fopen("resultados_tempo.txt", "a");

        if (resultado == NULL)
        {
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
        resultadob = fopen("resultados_tempo.txt", "a");

        if (resultadob == NULL)
        {
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

void menuHashCliente(){

    int opcao;
    printf("\n*************************************************\n");
    printf("******** METODO DA DIVISAO - (Cod mod M) ********\n");
    printf("************* ENCADEAMENTO EXTERIOR *************\n");
    printf("** CADA ARQUIVO SE COMPORTA COMO A TABELA BASE **\n\n");
    printf("----MENU HASH----\n");
    printf("1: INSERIR EM MASSA\n");
    printf("2: BUSCAR\n");
    printf("3: REMOVER UM REGISTRO\n");
    printf("4: REMOVER TODOS OS REGISTROS\n");
    printf("5: IMPRIMIR\n");
    printf("0: EXIT\n");
    printf("*************************************************\n\n");
    printf("DIGITE A OPCAO DESEJADA: ");

    scanf("%d", &opcao);
    switch (opcao)
    {
    case 1:
        system("cls");
        criarBaseClientesDesordenadaHash();
        break;

    case 2:
        system("cls");
        menuBuscarEmHashCliente();
        break;

    case 3:
        system("cls");
        //menuRemoverEmHashCliente();
        break;

    case 4:
        system("cls");
        //menuRemoverTodosEmHashCliente();
        break;

    case 5:
        system("cls");
        imprimirEmHashCliente();
        break;

    case 0:
        system("cls");
        printf("\n-----EXIT-----\n");
        exit(0);

    default:
        system("cls");
        printf("\n-----OPCAO INVALIDA-----\n");
        break;
    }
}

void criarBaseClientesDesordenadaHash()
{
    // abre o arquivo de particoes
    FILE *filePartition;

    // abre o arquivo que contem o ultimo codigo de funcionario
    FILE *fileLastCode = fopen("lastCode.dat", "rb+");

    // le o ultimo codigo de funcionario
    int lastCode;
    fread(&lastCode, sizeof(int), 1, fileLastCode);
    printf("lastCode inicio = %d\n", lastCode);
    int aux = lastCode;

    int n;
    printf("\nDigite a quantidade de clientes a serem inseridos desordenados na HASH: ");
    scanf("%d", &n);

    if(n <= 0){
        printf("\nQuantidade invalida!\n\n");
        criarBaseClientesDesordenadaHash();
    }

    //ler do arquivo "numberOfPartition.dat" o numero de particoes
    int numberOfPartition;
    FILE *fileNumberOfPartition = fopen("numberOfPartition.dat", "rb"); // Abre o arquivo em modo de leitura binária

    if (fileNumberOfPartition == NULL) {
        numberOfPartition = sqrt(n);
    } else {
        fread(&numberOfPartition, sizeof(int), 1, fileNumberOfPartition);
        fclose(fileNumberOfPartition); // Fecha o arquivo após a leitura
    }

    printf("\nInserindo %d clientes nos arquivos HASH...\n", n);

    int *indices = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        indices[i] = aux + i + 1;
        //printf("indices[%d] = %d\n", i, indices[i]);
    }
    shuffle(indices, n); //vetor, tamanho

    //printf("\nTeste Shuffle: \n");

    for (int i = 0; i < n; i++)
    {
        char cpf[15];
        gerarCPF(cpf, indices[i]);
        char data[11];
        gerarData(data, indices[i]);
        char nome[MAX_TAM_NOME];
        gerarNome(nome, indices[i]);
        Tclien *c1 = cliente(indices[i], nome, cpf, data, gerarMensalidade(i));

        //printf("\nTeste0\n");

        //calcula o nome do arquivo, "hashTablePartition" + i
        char partitionName[100];
        char str1[100];
        char str2[100] = ".dat";

        //printf("\nTesteitoa\n");

        //printf("\nindices[%d] = %d\n", i, indices[i]);
        //printf("\nnumero de particoes = %d\n", numberOfPartition);
        //printf("\nresto = %d\n", indices[i] % numberOfPartition);

        itoa(indices[i] % numberOfPartition, str1, 10);

        //printf("\nresto = %d\n", indices[i] % numberOfPartition);

        strcat(strcpy(partitionName, "hashTablePartition"), str1);
        strcat(strcpy(partitionName, partitionName), str2);

        //printf("\nteste1\n");

        //abre o arquivo de particoes
        filePartition = fopen(partitionName, "ab+");

        //printf("\nteste2\n");

        //salva o cliente no arquivo de particoes
        salva_cliente(c1, filePartition);

        //fecha o arquivo de particoes
        fclose(filePartition);

        free(c1);
        lastCode++;
        //printf("\nCliente %d\n", indices[i]);
    }

    printf("\nNumero de particoes: %d\n", numberOfPartition);
    printf("\nLastCode final = %d\n", lastCode);

    //salva o ultimo codigo de funcionario
    fseek(fileLastCode, 0, SEEK_SET);
    fwrite(&lastCode, sizeof(int), 1, fileLastCode);
    fclose(fileLastCode);

    free(indices);
    fclose(filePartition);
    printf("\nCONCLUIDO\n\n");
    menuHashCliente();
}

// converte a base de dados para tabela hash
void hashTable(FILE *file) {

    int numberOfPartition = 0;

    // variaveis auxiliares
    int lastCode = 0;

    // calcula o tamanho do arquivo
    int sizeFile = tamanho_total_cliente(file);

    // calcula o numero de particoes
    numberOfPartition = sqrt(sizeFile);

    rewind(file); // volta o ponteiro para o inicio do arquivo

    char partitionName[100];
    char str1[100];
    char str2[100] = ".dat";


    // percorre o arquivo e salva os registros nas particoes
    for (int i = 0; i < sizeFile; ++i) {

        Tclien *aux = ler_registro_Tclien(file);

        int selectedParticipation = aux->cod % numberOfPartition;

        itoa(selectedParticipation, str1, 10);
        strcat(strcpy(partitionName, "hashTablePartition"), str1);
        strcat(strcpy(partitionName, partitionName), str2);

        FILE *filePartition = fopen(partitionName,"ab+");

        saveRegisterEmployee(aux, filePartition);

        fclose(filePartition);

        // armazena o ultimo codigo de cliente
        if (aux->cod > lastCode){
            lastCode = aux->cod;
        }

    }

    // imprime os codigos dos cliente em cada particao
    for (int i = 0; i < numberOfPartition; ++i) {

        itoa(i, str1, 10);
        strcat(strcpy(partitionName, "hashTablePartition"), str1);
        strcat(strcpy(partitionName, partitionName), str2);

        FILE *filePartition = fopen(partitionName,"rb+");

        printPartitionEmployeeID(filePartition, partitionName);

        fclose(filePartition);
    }

    //armazena o ultimo codigo de funcionario em um arquivo .dat para ser usado na insercao
    FILE *fileLastCode = fopen("lastCode.dat", "wb+");
    fwrite(&lastCode, sizeof(int), 1, fileLastCode);
    fclose(fileLastCode);

    //imprime o ultimo codigo de funcionario do arquivo para teste
    /*FILE *fileLastCode_ = fopen("lastCode.dat", "rb+");
    int lastCodeTest;
    fread(&lastCodeTest, sizeof(int), 1, fileLastCode_);
    printf("\n\nlastCodeTest: %d\n\n", lastCodeTest);
    fclose(fileLastCode_);*/

    //armazena o numero de particoes em um arquivo .dat para ser usado na insercao
    FILE *fileNumberOfPartition = fopen("numberOfPartition.dat", "wb+");
    fwrite(&numberOfPartition, sizeof(int), 1, fileNumberOfPartition);
    fclose(fileNumberOfPartition);

    // excluir arquivo original
    fclose(file);
    remove("cliente.dat");

    menuHashCliente();
}

// imprime os clientes em cada particao
void imprimirEmHashCliente()
{
    // abre o arquivo de particoes
    FILE *filePartition;

    // abre o arquivo que contem o ultimo codigo de funcionario
    FILE *fileLastCode = fopen("lastCode.dat", "rb+");

    // le o ultimo codigo de funcionario
    int lastCode;
    fread(&lastCode, sizeof(int), 1, fileLastCode);
    printf("lastCode = %d\n", lastCode);
    int aux = lastCode;

    //ler do arquivo "numberOfPartition.dat" o numero de particoes
    int numberOfPartition;
    FILE *fileNumberOfPartition = fopen("numberOfPartition.dat", "rb"); // Abre o arquivo em modo de leitura binária

    if (fileNumberOfPartition == NULL) {
        printf("Erro ao abrir arquivo de particoes\n");
        exit(1);
    } else {
        fread(&numberOfPartition, sizeof(int), 1, fileNumberOfPartition);
        fclose(fileNumberOfPartition); // Fecha o arquivo após a leitura
    }

    printf("\nNumero de particoes: %d\n", numberOfPartition);

    //calcula o nome do arquivo, "hashTablePartition" + i
    char partitionName[100];
    char str1[100];
    char str2[100] = ".dat";

    // imprime os codigos dos cliente em cada particao
    for (int i = 0; i < numberOfPartition; ++i) {

        itoa(i, str1, 10);
        strcat(strcpy(partitionName, "hashTablePartition"), str1);
        strcat(strcpy(partitionName, partitionName), str2);

        FILE *filePartition = fopen(partitionName,"rb+");

        printPartitionEmployeeID(filePartition, partitionName);

        fclose(filePartition);
    }

    menuHashCliente();

}

void menuBuscarEmHashCliente()
{
    // abre o arquivo que contem o ultimo codigo de funcionario
    FILE *fileLastCode = fopen("lastCode.dat", "rb+");

    // le o ultimo codigo de funcionario
    int lastCode;
    fread(&lastCode, sizeof(int), 1, fileLastCode);
    printf("lastCode = %d\n", lastCode);
    int aux = lastCode;

    //ler do arquivo "numberOfPartition.dat" o numero de particoes
    int numberOfPartition;
    FILE *fileNumberOfPartition = fopen("numberOfPartition.dat", "rb"); // Abre o arquivo em modo de leitura binária

    if (fileNumberOfPartition == NULL) {
        printf("Erro ao abrir arquivo de particoes\n");
        exit(1);
    } else {
        fread(&numberOfPartition, sizeof(int), 1, fileNumberOfPartition);
        fclose(fileNumberOfPartition); // Fecha o arquivo após a leitura
    }

    printf("\nNumero de particoes: %d\n", numberOfPartition);

    //calcula o nome do arquivo, "hashTablePartition" + i
    char partitionName[100];
    char str1[100];
    char str2[100] = ".dat";

    // procura o cliente na particao, cod mod numero de particoes
    printf("\n\nDigite o codigo do cliente a ser buscado: ");
    int cod;
    scanf("%d", &cod);

    if(cod > lastCode){
        printf("\nCodigo maior que a quantidade de clientes cadastrados\n\n");
        menuHashCliente();
    }

    itoa(cod % numberOfPartition, str1, 10);
    strcat(strcpy(partitionName, "hashTablePartition"), str1);
    strcat(strcpy(partitionName, partitionName), str2);
    printf("partitionName = %s\n", partitionName);

    FILE *filePartition = fopen(partitionName,"rb+");
    fseek(filePartition, sizeof(Tclien), SEEK_SET);

    // calcula o tamanho do arquivo
    int sizeFile = tamanho_total_cliente(filePartition);
    printf("sizeFile = %d\n", sizeFile);

    int cont = 1;

    for(int i=0; i<sizeFile; i++){

        fseek(filePartition, i * sizeof(Tclien), SEEK_SET); // posiciona o ponteiro do arquivo na posicao i
        Tclien *aux = ler_registro_Tclien(filePartition);

        if(aux->cod == cod){
            printf("\nForam feitas %d comparacoes\n\n\n", cont);
            imprime_cliente(aux);

            // fecha o arquivo de particoes
            fclose(filePartition);

            // fecha o arquivo que contem o ultimo codigo de funcionario
            fclose(fileLastCode);

            menuHashCliente();
        }
        cont++;
    }
        printf("Cliente nao encontrado\n");

    // fecha o arquivo de particoes
    fclose(filePartition);

    // fecha o arquivo que contem o ultimo codigo de funcionario
    fclose(fileLastCode);

    menuHashCliente();
}
