/*
  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__

  AO PREENCHER ESSE CABEÇALHO COM O MEU NOME E O MEU NÚMERO USP, 
  DECLARO QUE SOU O ÚNICO AUTOR E RESPONSÁVEL POR ESSE PROGRAMA. 
  TODAS AS PARTES ORIGINAIS DESSE EXERCÍCIO PROGRAMA (EP) FORAM 
  DESENVOLVIDAS E IMPLEMENTADAS POR MIM SEGUINDO AS INSTRUÇÕES DESSE EP
  E QUE PORTANTO NÃO CONSTITUEM PLÁGIO. DECLARO TAMBÉM QUE SOU RESPONSÁVEL
  POR TODAS AS CÓPIAS DESSE PROGRAMA E QUE EU NÃO DISTRIBUI OU FACILITEI A
  SUA DISTRIBUIÇÃO. ESTOU CIENTE QUE OS CASOS DE PLÁGIO SÃO PUNIDOS COM 
  REPROVAÇÃO DIRETA NA DISCIPLINA.

  Nome: Alexandre Muller Jones
  NUSP: 8038149

  IMDB: main.c 


  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma refência, liste-as abaixo
  para que o seu programa não seja considerada plágio.
  Exemplo:
  - função mallocc retirada de: http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html

  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/

#include <stdio.h>  /* printf(), scanf() */
#include <time.h>   /* CLOCKS_PER_SECOND */
#include <string.h> /* strlen()     */

#include "main.h"

#include "filmes.h"   /* crieListaFilmes(), crieFilme(), 
                         inseraFilme(),  mergeSortFilmes() */
#include "iofilmes.h" /* carregueListaFilmes(), graveListaFilmes(), 
                         mostreFilme(), mostreListaFilmes() */
#include "util.h"     /* mallocSafe(), leiaString() */
#include "st.h"       /* initST(), putFilmeST(), getFilmeST(), showST(),
                         freeST() */

/*------------------------------------------------------------------- 
  P R O T O T I P O S 
*/

/* A funcao leiaOpcao so e usada neste arquivo. */
static char leiaOpcao();



/*------------------------------------------------------------------- 
  M A I N 
*/
int main(int argc, char *argv[])
{
    ListaFilmes *lst = crieListaFilmes();
    char opcao;
    clock_t start, end; /* usadas para medir tempo de processamento */
    double elapsed;
    /* declaracao das demais variaveis do main */

    /*------------------------------------------------------------*/
    /* imprima o cabecalho */
    printf("MAC0121 2019 - EP5\n");
    printf("The Internet Movie Database (%s, %s)\n", __DATE__, __TIME__);
    printf("[GCC %s] on %s\n", __VERSION__, SYSTEM);

    /*-----------------------------------------------------*/
    /* iterara ate o usuario digitar 'x' para sair         */
    do
    {

        opcao = leiaOpcao();

        /* comeca a cronometrar */
        start = clock();

        switch (opcao)
        {
            /*---------------------------------------------*/
        case CARREGAR_SER:
        {
            carregueListaFilmes(lst, FALSE);
            break;
        }

        /*---------------------------------------------*/
        case CARREGAR_ER:
        {
            carregueListaFilmes(lst, TRUE);
            break;
        }

        /*---------------------------------------------*/
        case GRAVAR:
        {
            graveListaFilmes(lst);
            break;
        }

        /*---------------------------------------------*/
        case PROCURAR:
        {
            char *termo = mallocSafe(TAM_STR * sizeof(char));
            int tamTermo = 0, tamNome = 0;
            Bool terminou = FALSE, achouAlgum = FALSE;
            Filme *aux = NULL;

            printf("Digite parte do nome do filme a ser procurado: ");
            tamTermo = leiaString(termo, TAM_STR);
            aux = lst->cab->prox;
            while (!terminou)
            {
                if (aux == lst->cab && achouAlgum == FALSE)
                {
                    printf("Nenhum filme encontrado com o termo digitado!\n");
                    terminou = TRUE;
                }
                else if (achePalavra((unsigned char *) termo, tamTermo, (unsigned char *) aux->nome, strlen(aux->nome)))
                {
                    achouAlgum = TRUE;
                    mostreFilme(aux);
                    printf("Esse e' o filme procurado? [s/n/x] (x para sair): ");
                    scanf("%c", &opcao);
                    if (opcao == 's' || opcao == 'x')
                    {
                        terminou = TRUE;
                    }
                }
                aux = aux->prox;
            }
            free(termo);
            break;
        }

        /*---------------------------------------------*/
        case HASH: /* opcional */
        {
            break;
        }

        /*---------------------------------------------*/
        case PROCURAR_HASH: /* opcional */
        {
            break;
        }

        /*---------------------------------------------*/
        case MOSTRAR_HASH: /* opcional */
        {
            break;
        }

        /*---------------------------------------------*/
        case LIMPAR_HASH: /* opcional */
        {
            break;
        }

        /*---------------------------------------------*/
        case INSERIR:
        {
            Filme *flm = NULL;

            char dist[TAM_DIST + 1];
            int votos;
            float nota;
            char nome[TAM_STR + 1];
            int ano;

            printf("Digite o nome do filme: ");
            leiaString(nome, TAM_STR);

            printf("Digite o ano: ");
            scanf("%d", &ano);

            printf("Digite a nota: ");
            scanf("%f", &nota);

            printf("Digite o numero de votos: ");
            scanf("%d", &votos);

            printf("Digite a distribuicao: ");
            leiaString(dist, TAM_DIST + 1);

            flm = crieFilme(dist, votos, nota, nome, ano);
            mostreFilme(flm);

            insiraFilme(lst, flm);
            printf("Filme inserido na lista de filmes.\n");
            break;
        }

        /*---------------------------------------------*/
        case REMOVER:
        {
            char *termo = mallocSafe(TAM_STR * sizeof(char));
            int tamTermo = 0, tamNome = 0;
            Bool terminou = FALSE, achouAlgum = FALSE;
            Filme *aux = NULL;

            printf("Digite parte do nome do filme a ser procurado: ");
            tamTermo = leiaString(termo, TAM_STR);
            aux = lst->cab->prox;
            while (!terminou)
            {
                if (aux == lst->cab)
                {
                    if (achouAlgum == FALSE)
                    {
                        printf("Nenhum filme encontrado com o termo digitado!\n");
                    }
                    else
                    {
                        printf("Nenhum outro filme encontrado com o termo digitado!\n");
                    }
                    terminou = TRUE;
                }
                else if (achePalavra((unsigned char *) termo, tamTermo, (unsigned char *) aux->nome, strlen(aux->nome)))
                {
                    achouAlgum = TRUE;
                    mostreFilme(aux);
                    printf("Esse e' o filme procurado? [s/n/x] (x para sair): ");
                    scanf("%c", &opcao);
                    if (opcao == 'x')
                    {
                        terminou = TRUE;
                    }
                    else if(opcao == 's')
                    {
                        removaFilme(lst, aux);
                        printf("Filme removido.\n");
                        terminou = TRUE;
                    }
                }
                aux = aux->prox;
            }
            free(termo);
            break;
        }

        /*---------------------------------------------*/
        case ORDENAR_NOTA_M:
        {
            mergeSortFilmes(lst, NOTA);
            printf("Lista ordenada!\n");
            break;
        }

        /*---------------------------------------------*/
        case ORDENAR_NOME_M:
        {
            mergeSortFilmes(lst, NOME);
            printf("Lista ordenada!\n");
            break;
        }

        /*---------------------------------------------*/
        case ORDENAR_NOTA_Q: /* opcional */
        {
            break;
        }

        /*---------------------------------------------*/
        case ORDENAR_NOME_Q: /* opcional */
        {
            break;
        }

        /*---------------------------------------------*/
        case MOSTRAR:
        {
            mostreListaFilmes(lst);
            break;
        }

        /*---------------------------------------------*/
        case MOSTRAR_MENOR:
        {
            break;
        }

        /*---------------------------------------------*/
        case MOSTRAR_MAIOR:
        {
            break;
        }

        /*---------------------------------------------*/
        case LIMPAR:
        {
            break;
        }

        /*---------------------------------------------*/
        case SAIR:
        {
            libereListaFilmes(lst);
            break;
        }

        default:
        {
            printf("main: opcao '%c' nao reconhecida\n", opcao);
            break;
        }

        } /* switch */

        /* trava o cronometro */
        end = clock();

        /* calcula o tempo */
        elapsed = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("\n(%g segundos)\n", elapsed);
    } while (opcao != SAIR);

    freeST();

    return 0;
}

/*---------------------------------------------------------------------
  leiaOpcao()
   
  Le e retorna o caractere correspondente a opcao do usuario.
*/

char leiaOpcao()
{
    char opcao;
    char listaOpcoes1[] =
        "\n================================================"
        "======================\n"
        "  (c) carregar um arquivo de dados sem eliminar repeticoes\n"
        "  (C) carregar um arquivo de dados eliminando repeticoes\n"
        "  (g) gravar   a lista atual em um arquivo\n"
        "  (p) procurar a nota de um filme\n"
        "  (h) criar    a  TS com as palavras em nomes de filmes (opcional)\n"
        "  (P) procurar na TS a nota de um filme (opcional)\n"
        "  (M) mostrar  a  TS (opcional)\n"
        "  (L) limpar   a  TS (opcional)\n"
        "  (i) inserir  um filme\n"
        "  (r) remover  um filme\n";
    char listaOpcoes2[] =
        "  (o) ordenar  a lista de filmes por nota (mergeSort)\n"
        "  (O) ordenar  a lista de filmes por nome (mergeSort)\n"
        "  (q) ordenar  a lista de filmes por nota (quickSort, opcional)\n"
        "  (Q) ordenar  a lista de filmes por nome (quickSort, opcional)\n"
        "  (m) mostrar  todos os filmes\n"
        "  (<) mostrar  N filmes com nota _menor_ que X e pelo menos V votos\n"
        "  (>) mostrar  N filmes com nota _maior_ que X e pelo menos V votos\n"
        "  (l) limpar   a lista de filmes\n"
        "  (x) sair     do programa\n\n";

    printf("%s", listaOpcoes1);
    printf("%s", listaOpcoes2);
    printf("Digite uma opcao: ");
    scanf(" %c", &opcao);

    /* main.c: warning: string length ‘679’ is greater than the length 
       ‘509’ ISO C90 compilers are required to support 
       [-Woverlength-strings] */

    return opcao;
}
