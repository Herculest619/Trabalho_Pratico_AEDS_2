#include "equipamento.h"
#include "funcionario.h"
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
        menuCriarBaseEquipamentos(equip);
        break;

    case 2:
        system("cls");
        listarEquipamentos(equip);
        menuInicialEstoque(equip);
        break;

    case 3:
        system("cls");
        menuBuscaEquipamento(equip);
        break;

    case 4:
        system("cls");
        menuOrdenarEquipamento(equip);
        break;

    case 5:
        system("cls");
        printf("----------EXCLUIR BASE----------\n");
        printf("\nDeseja excluir a base de equipamentos?\nDigite 1 para SIM ou 0 para NAO: ");
        int opcao;
        scanf("%d", &opcao);

        if (opcao == 1)
        {
            fclose(equip);
            remove("equipamento.dat");
            equip = fopen("equipamento.dat", "w+b");
            printf("\nBase de equipamentos excluida!\n\n");
        }
        else
        {
            printf("\nBase de equipamentos nao excluida!\n\n");
        }

        menuInicialEstoque(equip);
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

        //salvar o tempo de execução em um arquivo txt
        FILE *resultado;
        resultado = fopen("resultados.txt", "a");

        if (resultado == NULL) {
            printf("Erro ao abrir o arquivo.\n");
            menuInicialEstoque(equip);
            break;
        }

        fprintf(resultado, "-----------------------------\n");
        fprintf(resultado, "Ordenção em disco usando Insertion Sort\n");
        fprintf(resultado, "Tempo de execucao: %f segundos\n", tempo_execucao);
        fprintf(resultado, "Tamanho da base: %d\n", tamanho_total_equipamento(equip));
        fprintf(resultado, "-----------------------------\n\n\n");

        fclose(resultado);

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

        //salvar o tempo de execução em um arquivo txt
        FILE *resultadob;
        resultadob = fopen("resultados.txt", "a");

        if (resultadob == NULL) {
            printf("Erro ao abrir o arquivo.\n");
            menuInicialEstoque(equip);
            break;
        }

        fprintf(resultadob, "-----------------------------\n");
        fprintf(resultadob, "Ordenção em disco usando Selection Sort\n");
        fprintf(resultadob, "Tempo de execucao: %f segundos\n", tempo_execucao);
        fprintf(resultadob, "Tamanho da base: %d\n", tamanho_total_equipamento(equip));
        fprintf(resultadob, "-----------------------------\n\n\n");

        fclose(resultadob);

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

void insertion_sort_disco_equipamento(FILE *arq)
{

    int tam = tamanho_total_equipamento(arq);
    int i, cont = 0;
    //faz o insertion sort
    for (int j = 2; j <= tam; j++)
    {
        //posiciona o arquivo no registro j
        fseek(arq, (j - 1) * tamanho_equipamento_bytes(), SEEK_SET);
        TEquip *fj = leEquip(arq);
        //printf("\n********* equipamento atual: %d\n", fj->cod);
        i = j - 1;
        //posiciona o cursor no registro i
        fseek(arq, (i - 1) * tamanho_equipamento_bytes(), SEEK_SET);
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
            fseek(arq, (i - 1) * tamanho_equipamento_bytes(), SEEK_SET);
            fi = leEquip(arq);
            //printf("fi = %d; i = %d\n", fi->cod, i);
            cont++;
        }
        //posiciona cursor no registro i + 1
        fseek(arq, (i)*tamanho_equipamento_bytes(), SEEK_SET);
        //printf("*** Salvando equipamento %d na posicao %d\n", fj->cod, i+1);
        //salva registro j na posiÃ§Ã£o i
        salvaEquipamento(fj, arq);
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

void selection_sort_discos_equipamento(FILE *arq)
{
    int tam = tamanho_total_equipamento(arq);
    int i, j, min, cont = 0;
    //faz o selection sort
    for (i = 1; i < tam; i++)
    {
        //posiciona o arquivo no registro i
        fseek(arq, (i - 1) * tamanho_equipamento_bytes(), SEEK_SET);
        TEquip *fi = leEquip(arq);
        //printf("\n********* equipamento atual: %d\n", fi->cod);
        min = i;
        for (j = i + 1; j <= tam; j++)
        {
            //posiciona o arquivo no registro j
            fseek(arq, (j - 1) * tamanho_equipamento_bytes(), SEEK_SET);
            TEquip *fj = leEquip(arq);
            //printf("fj = %d\n", fj->cod);
            if (fj->num_serie < fi->num_serie)
            {
                min = j;
                //printf("min = %d\n", min);
                //posiciona o arquivo no registro min
                fseek(arq, (min - 1) * tamanho_equipamento_bytes(), SEEK_SET);
                TEquip *fmin = leEquip(arq);
                //printf("fmin = %d\n", fmin->cod);
                //troca os registros
                fseek(arq, (i - 1) * tamanho_equipamento_bytes(), SEEK_SET);
                salvaEquipamento(fmin, arq);
                fseek(arq, (min - 1) * tamanho_equipamento_bytes(), SEEK_SET);
                salvaEquipamento(fi, arq);
                //posiciona o arquivo no registro i
                fseek(arq, (i - 1) * tamanho_equipamento_bytes(), SEEK_SET);
                fi = leEquip(arq);
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

void menuOrdenarEquipamento(FILE *equip)
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

        menu_ordenacao_disco_equipamento(equip);

        break;

    case 2:

        system("cls");

        criar_particoes_equip(equip);

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

        FILE *resultado;
        resultado = fopen("resultados.txt", "a");

        if (resultado == NULL) {
            printf("Erro ao abrir o arquivo.\n");
            menuInicialEstoque(equip);
            break;
        }

        fprintf(resultado, "-----------------------------\n");
        fprintf(resultado, "Busca sequencial\n");
        fprintf(resultado, "Tempo de execucao: %f segundos\n", tempo_execucao);
        fprintf(resultado, "Tamanho da base: %d\n", tamanho_total_equipamento(equip));
        fprintf(resultado, "-----------------------------\n\n\n");

        fclose(resultado);

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

        printf("\n\nTempo de execucao da busca binaria: %.6f segundos\n\n", tempo_execucao);

        FILE *resultado1;
        resultado1 = fopen("resultados.txt", "a");

        if (resultado1 == NULL)
        {
            printf("Erro ao abrir o arquivo.\n");
            menuInicialEstoque(equip);
            break;
        }

        fprintf(resultado1, "-----------------------------\n");
        fprintf(resultado1, "Busca binaria\n");
        fprintf(resultado1, "Tempo de execucao: %f segundos\n", tempo_execucao);
        fprintf(resultado1, "Tamanho da base: %d\n", tamanho_total_equipamento(equip));
        fprintf(resultado1, "-----------------------------\n\n\n");

        fclose(resultado1);

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
            return equip;
        }
        free(equip);
    }

    return NULL;
}

void listarEquipamentos(FILE *in)
{
    if (tamanho_total_equipamento(in) == 0)
    {
        printf("\nArquivo vazio!\n\n");
        return;
    }
    printf("\n\nLendo equipamentos do arquivo...\n\n");
    rewind(in);
    TEquip *e;
    while ((e = leEquip(in)) != NULL)
    {
        imprime_equip(e);
        free(e);
    }
}

void imprime_equip(TEquip *e)
{
    printf("**********************************************");
    printf("\nNumero de serie: %d\n", e->num_serie);
    printf("Nome: %s\n", e->nome);
    printf("MAC: %s\n", e->mac);
    printf("\n\n**********************************************");
    printf("\n\n\n");
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
    const char *nomeequips[MAX_NOMES_EQUIP] = {"Linksys WRT3200ACM",
                                               "TP-Link Archer C7",
                                               "Huawei EchoLife HG8245H",
                                               "ZTE F660",
                                               "Cisco RV260W",
                                               "Netgear Nighthawk X6",
                                               "Nokia G-240W-B",
                                               "Fiberhome AN5506-02-FG",
                                               "Asus RT-AC68U",
                                               "D-Link DIR-882",
                                               "Calix 716GE-I",
                                               "TP-Link TX-VG1530",
                                               "MikroTik RB3011UiAS-RM",
                                               "Ubiquiti UniFi Dream Machine",
                                               "Huawei MA5608T",
                                               "ZTE C320",
                                               "Nokia ISAM 7360",
                                               "Fiberhome AN5516-04",
                                               "Alcatel-Lucent G-010G-A",
                                               "Alcatel-Lucent G-240W-C",
                                               "TP-Link TX-6610",
                                               "Huawei B525s-23a",
                                               "Alcatel-Lucent 7360 ISAM FX-8"};

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

void criar_particoes_equip(FILE *arq)
{
    int tam = tamanho_total_equipamento(arq);
    float tam_part;
    int qnt_part;

    printf("\nDigite o tamanho de particoes (MAX = %d): ", tam);
    scanf("%f", &tam_part);

    if (tam_part > tam)
    {
        printf("\nNao e possivel criar uma particao maior que o tamanho total da base.\n\n");
        criar_particoes_equip(arq);
    }
    else if (tam_part <= 0)
    {
        printf("\nNao e possivel criar uma particao com o tamanho igual ou menor a 0.\n\n");
        criar_particoes_equip(arq);
    }

    qnt_part = (int)ceil(tam / tam_part);

    printf("\nQuantidade de particoes: %d\n", qnt_part);

    if (qnt_part > 505){
        printf("\nNao e possivel criar mais de 505 particoes.\n\n");
        criar_particoes_equip(arq);
    }

    // Array para armazenar temporariamente os registros da partição em RAM
    TEquip *particao_temp = malloc(sizeof(TEquip) * tam_part);

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
            fseek(arq, inicio * sizeof(TEquip), SEEK_SET);
            fread(particao_temp, sizeof(TEquip), fim - inicio, arq);

            // Ordenação da partição em RAM usando a Seleção por substituição
            selececao_por_substituicao_equip(particao_temp, fim - inicio);

            fwrite(particao_temp, sizeof(TEquip), fim - inicio, particao);

            if (opcao != 0)
            {
                printf("\n\n\n-+-+-+-+-+-+-+-+- PARTICAO %d -+-+-+-+-+-+-+-+-", i);
                listarEquipamentos(particao);
            }

            // Fechamento do arquivo da partição
            fclose(particao);
        }
    }
    free(particao_temp);

        end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    //printf("\nTempo de execucao: %f segundos\n", cpu_time_used);

    printf("\n-----INTERCALANDO PARTICOES COM INTERCALACAO OTIMA-----\n");
    intercalacao_otima_equip(arq, qnt_part, "equipamentos_ordenados.dat", cpu_time_used);

}

void selececao_por_substituicao_equip(TEquip *arr, int size)
{
    int i, j, min_idx;
    for (i = 0; i < size - 1; i++)
    {
        min_idx = i;
        for (j = i + 1; j < size; j++)
        {
            if (arr[j].num_serie < arr[min_idx].num_serie)
            {
                min_idx = j;
            }
        }
        if (min_idx != i)
        {
            TEquip temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;
        }
    }
}

void intercalacao_otima_equip(FILE *arq, int qnt_part, const char *nome_base, float time_used)
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
    TEquip *registro_atual = malloc(sizeof(TEquip) * qnt_part);

    // Leitura inicial do primeiro registro de cada partição
    for (int i = 0; i < qnt_part; i++)
    {
        if (fread(&registro_atual[i], sizeof(TEquip), 1, particoes[i]) != 1)
        {
            // Se a partição estiver vazia, coloca um valor máximo no registro para evitar problemas
            registro_atual[i].num_serie = INT_MAX;
        }
    }

    // Intercalação dos registros até que todas as partições estejam vazias
    while (1)
    {
        // Encontra o menor registro atual dentre todas as partições
        int menor_indice = -1;
        for (int i = 0; i < qnt_part; i++)
        {
            if (registro_atual[i].num_serie != INT_MAX &&
                (menor_indice == -1 || registro_atual[i].num_serie < registro_atual[menor_indice].num_serie))
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
        fwrite(&registro_atual[menor_indice], sizeof(TEquip), 1, saida);

        // Lê o próximo registro da partição que tinha o menor registro
        if (fread(&registro_atual[menor_indice], sizeof(TEquip), 1, particoes[menor_indice]) != 1)
        {
            // Se a partição estiver vazia, coloca um valor máximo no registro para evitar problemas
            registro_atual[menor_indice].num_serie = INT_MAX;
        }
    }

    end = clock();

    printf("\nDeseja imprimir as base completa ordenada?\nDigite 1 para SIM ou 0 para NAO: ");
    int opcao;
    scanf("%d", &opcao);

    if (opcao != 0)
    {
        printf("\n\n\n-+-+-+-+-+-+-+-+-+-+- BASE ORDENADA -+-+-+-+-+-+-+-+-+-+-");
        listarEquipamentos(saida);
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
    copiar_arquivo_equip(arq, "equipamentos_ordenados.dat");

    remove("equipamentos_ordenados.dat");
    free(particoes);
    free(registro_atual);

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\nTempo de execucao: %f segundos\n\n\n", cpu_time_used + time_used);


    FILE *resultado;
    resultado = fopen("resultados.txt", "a");

    if (resultado == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        menuInicialEstoque(arq);
        //break;
    }

    fprintf(resultado, "-----------------------------\n");
    fprintf(resultado, "Ordenação com Particionamento e Intercalação Otima\n");
    fprintf(resultado, "Tempo de execucao: %f segundos\n", cpu_time_used + time_used);
    fprintf(resultado, "Tamanho da base: %d\n", tamanho_total_equipamento(arq));
    fprintf(resultado, "Tamanho da particao: %d\n", tamanho_total_equipamento(arq) / qnt_part);
    fprintf(resultado, "Quantidade de particoes: %d\n", qnt_part);
    fprintf(resultado, "-----------------------------\n\n\n");

    fclose(resultado);
}

void copiar_arquivo_equip(FILE *arq_destino, const char *nome_arquivo_origem)
{
    // Abre o arquivo de destino para escrita binária (truncando o arquivo)
    //FILE *arq_destino;
    if ((arq_destino = fopen("equipamento.dat", "wb")) == NULL)
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

    TEquip equipamento;

    while (fread(&equipamento, sizeof(TEquip), 1, arq_origem) == 1)
    {
        fwrite(&equipamento, sizeof(TEquip), 1, arq_destino);
    }

    fclose(arq_origem);
    fclose(arq_destino);
}
