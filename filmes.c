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

  IMDB: filmes.c


  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma refência, liste-as abaixo
  para que o seu programa não seja considerada plágio.
  Exemplo:
  - função mallocc retirada de: http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html

  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/

/*----------------------------------------------------------*/
/* filmes.h e a interface para as funcoes neste modulo      */
#include "filmes.h"

/*----------------------------------------------------------*/
#include <stdlib.h> /* NULL, free() */
#include <stdio.h>  /* printf(), scanf() */
#include <string.h> /* strlen(), strncpy(), strcmp(), strtok() */

#include "util.h" /* Bool, mallocSafe() */
#include "iofilmes.h"
#include "st.h" /* freeST(), initST(), putFilmeST(), getFilmeST(),
                        showST(), freeST() */


/*Declaracao da funcao auxiliar criada*/
Filme *extraiFilme(ListaFilmes *lst, Filme *flm);


/*----------------------------------------------------------------------
 *  crieFilme
 *
 *  Recebe informacoes dobre um filme 
 *
 *      - DIST  : distribuicao de notas
 *      - VOTOS : numero de votos
 *      - NOTA  : nota do filme 
 *      - ANO   : ano de producao do filme
 *
 *  e cria uma celula do tipo Filme para armazenar essa informacao. 
 *  A funcao retorna o endereco da celula criada.
 */
Filme *
crieFilme(char dist[], int votos, float nota, char *nome, int ano)
{
    Filme *flm;
    int len = strlen(nome) + 1; /* +1 para o '\0' */

    flm = mallocSafe(sizeof *flm);

    strncpy(flm->dist, dist, TAM_DIST + 1); /* +1 para o '\0' */

    flm->votos = votos;
    flm->nota = nota;

    flm->nome = mallocSafe(len * sizeof(char));
    strncpy(flm->nome, nome, len);

    flm->ano = ano;

    flm->prox = flm->ant = NULL; /* paranoia */

    return flm;
}

/*----------------------------------------------------------------------
 *  crieListaFilmes
 * 
 *  Cria uma estrutura que representa lista de filmes vazia.
 *  A funcao retorna o endereco dessa estrutura.
 *
 *  Um lista de filmes e representada por uma lista encadeada 
 *  duplamente ligada com cabeca. 
 */
ListaFilmes *
crieListaFilmes()
{
    ListaFilmes *lst = mallocSafe(sizeof(ListaFilmes));
    lst->cab = mallocSafe(sizeof(Filme));
    lst->cab->prox = lst->cab->ant = lst->cab;
    lst->nFilmes = 0;
    return lst;
}

/*----------------------------------------------------------------------
 *  libereListaFilmes(lst)
 *
 *  Recebe um ponteiro lst para uma estrutura que representa uma lista 
 *  de filmes e libera toda a memoria alocada para a lista.
 *
 *  Esta funcao utiliza a funcao libereFilme().
 */

void libereListaFilmes(ListaFilmes *lst)
{
    Filme *pont;
    if(lst == NULL) return;
    while (lst->nFilmes > 0)
    {
        pont = extraiFilme(lst, lst->cab->prox);
        libereFilme(pont);
        pont = NULL;
    }
    free(lst->cab);
    lst->cab = NULL;
    free(lst);
    lst = NULL;
    return;
}

/*----------------------------------------------------------------------
 *  libereFilme
 *
 *  Recebe um ponteiro FLM para uma estrutura que representa um 
 *  filme e libera a area alocada.
 *
 */
void libereFilme(Filme *flm)
{
    free(flm->nome);
    free(flm);
    flm = NULL;
    return;
}

/*----------------------------------------------------------------------
 *  insiraFilme
 *
 *  Recebe um ponteito LST para uma estrutura que representa
 *  uma lista de filmes e um ponteiro FLM para uma estrutura
 *  que representa uma filme.
 *
 *  A funcao insere o filme na lista.
 *  
 */
void insiraFilme(ListaFilmes *lst, Filme *flm)
{
    flm->prox = lst->cab;
    flm->ant = lst->cab->ant;
    lst->cab->ant = flm;
    flm->ant->prox = flm;
    lst->nFilmes++;
    return;
}

/*---------------------------------------------------------------------
 *  contemFilme
 *
 *  Recebe um ponteito LST para uma estrutura que representa
 *  uma lista de filmes e um ponteiro FLM para uma estrutura
 *  que representa uma filme.
 *
 *  A funcao retorna  TRUE se o filme esta na lista e 
 *  FALSE em caso contrario. 
 *
 *  Consideramos que dois filmes f e g sao iguais se
 *
 *       - f->nome e' igual a g->nome 
 *       - f->nota e' igual a g->nota
 *       - f->ano  e' igual a g->ano 
 *
 *  Para comparar dois nomes voce pode usar alguma funcao da 
 *  bibliteca do c  como strcmp, strncmp (string,h) 
 *  ou a funcao strCmp (util.h).
 *
 */
Bool contemFilme(ListaFilmes *lst, Filme *flm)
{
    Filme *aux = lst->cab->prox;
    while (aux != lst->cab)
    {
        if (aux->nota == flm->nota)
        {
            if (aux->ano == flm->ano)
            {
                if (!strCmp(aux->nome, flm->nome))
                {
                    return TRUE;
                }
            }
        }
        else
        {
            aux = aux->prox;
        }
    }
    return FALSE;
}

/*----------------------------------------------------------------------
 *  removaFilme
 *
 *  Remove da lista de filmes LST o filme apontado por FLM.
 *    
 *  Pre-condicao: a funcao supoe que o filme FLM esta 
 *                na lista LST.
 */
void removaFilme(ListaFilmes *lst, Filme *flm)
{
    Filme *aux = lst->cab->prox;
    while (aux != lst->cab)
    {
        if (aux->nota == flm->nota)
        {
            if (aux->ano == flm->ano)
            {
                if (!strCmp(aux->nome, flm->nome))
                {
                    break;
                }
            }
        }
        else
        {
            aux = aux->prox;
        }
    }
    /*Nesse ponto, aux contem o apontador para o filme*/
    libereFilme(aux);
    return;
}

/*----------------------------------------------------------------------
 *  mergeSortFilmes
 *
 *  Recebe uma lista de filmes LST e ordena a lista utilizando o
 *  algoritmo mergeSort recursivo adaptado para listas encadeadas
 *  duplamente ligadas com cabeca.
 *
 *  A funcao recebe ainda um parametro CRITERIO tal que:
 *
 *  Se CRITERIO == NOTA, entao a lista deve ser ordenada
 *      em ordem decrescente de nota.
 *
 *  Se CRITERIO == NOME, entao a lista deve ser ordenada
 *      em ordem crescente de nome (ordem alfabetica).
 *
 *  ------------------------------------------------------------------
 *  OBSERVACAO IMPORTANTE:
 *
 *  A ordenacao deve ser feita 'in-place', ou seja o conteudo das
 *  celulas _nao deve_ ser copiado, apenas os ponteiros devem ser
 *  alterados.
 *
 *  A funcao so deve utilizar espaco extra O(1).  
 *
 *  Hmmm, ok, sem levar em consideracao o espaco O(lg n) utilizado
 *  pela pilha da recursao.  Em outras palavras, a funcao pode conter
 *  apenas declaracoes de umas poucas variaveis (um vetor v[0..n]
 *  conta como n variaveis e nao vale).
 *
 *  ------------------------------------------------------------------
 *  Para ordenar por nome, veja a funcao strCmp em util.[h|c].
 */
void mergeSortFilmes(ListaFilmes *lst, Criterio criterio)
{
    ListaFilmes *aux1, *aux2;
    Filme *pont = NULL;
    int i, nFilmes = lst->nFilmes;
    if (lst->nFilmes == 1)
        return;
    aux1 = crieListaFilmes();
    aux2 = crieListaFilmes();
    for (i = 0; i < nFilmes / 2; i++)
    {
        /*Insere os primeiros nFilmes/2 na lista aux1*/
        pont = extraiFilme(lst, lst->cab->prox);
        insiraFilme(aux1, pont);
    }
    while (lst->nFilmes > 0)
    {
        /*Insere os outros em aux2*/
        pont = extraiFilme(lst, lst->cab->prox);
        insiraFilme(aux2, pont);
    }
    mergeSortFilmes(aux1, criterio); /*Aplica a recursao*/
    mergeSortFilmes(aux2, criterio);

    /*Da merge nas 2 listas*/
    while (aux1->nFilmes > 0 || aux2->nFilmes > 0)
    {
        switch (criterio)
        {
        case NOTA:
            if (aux1->nFilmes == 0)
            {
                pont = extraiFilme(aux2, aux2->cab->prox);
                insiraFilme(lst, pont);
                break;
            }
                
            if (aux2->nFilmes == 0)
            {
                pont = extraiFilme(aux1, aux1->cab->prox);
                insiraFilme(lst, pont);
                break;
            }
                
            if (aux1->cab->prox->nota < aux2->cab->prox->nota)
            {
                pont = extraiFilme(aux1, aux1->cab->prox);
                insiraFilme(lst, pont);
            }
            else
            {
                pont = extraiFilme(aux2, aux2->cab->prox);
                insiraFilme(lst, pont);
            }
            break;

        case NOME:
            if (aux1->nFilmes == 0)
            {
                pont = extraiFilme(aux2, aux2->cab->prox);
                insiraFilme(lst, pont);
                break;
            }
            if (aux2->nFilmes == 0)
            {
                pont = extraiFilme(aux1, aux1->cab->prox);
                insiraFilme(lst, pont);
                break;
            }
            if (strCmp(aux1->cab->prox->nome, aux2->cab->prox->nome) < 0)
            {
                pont = extraiFilme(aux1, aux1->cab->prox);
                insiraFilme(lst, pont);
            }
            else
            {
                pont = extraiFilme(aux2, aux2->cab->prox);
                insiraFilme(lst, pont);
            }
            break;

        default:
            break;
        }
    }

    libereListaFilmes(aux1);
    libereListaFilmes(aux2);
}

/*----------------------------------------------------------------------
 *  quickSortFilmes [opcional]
 *
 *  Recebe uma lista de filmes LST e ordena a lista utilizando o
 *  algoritmo quickSort adaptado para listas encadeadas duplamente
 *  ligadas com cabeca.
 *
 *  A funcao recebe ainda um parametro CRITERIO tal que:
 * 
 *  Se CRITERIO == NOTA, entao a lista deve ser ordenada
 *      em ordem decrescente de nota.
 *
 *  Se CRITERIO == NOME, entao a lista deve ser ordenada
 *      em ordem crescente de nome (ordem alfabetica).
 *
 *  ------------------------------------------------------------------
 *  OBSERVACAO IMPORTANTE:
 *
 *  A ordenacao deve ser feita 'in-place', ou seja o conteudo das
 *  celulas _nao deve_ ser copiado, apenas os ponteiros devem ser
 *  alterados.
 *
 *  A funcao so deve utilizar espaco extra O(1).  
 *
 *  Hmmm, ok, sem levar em consideracao o espaco O(lg n) utilizado
 *  pela pilha da recursao.  Em outras palavras, a funcao pode conter
 *  apenas declaracoes de umas poucas variaveis (um vetor v[0..n]
 *  conta como n variaveis e nao vale).
 *
 *  ------------------------------------------------------------------
 *  Para ordenar por nome, veja a funcao strCmp em util.[h|c].
 */
void quickSortFilmes(ListaFilmes *lst, Criterio criterio)
{
    AVISO(quickSortFilmes em filmes.c
          : Vixe ainda nao fiz essa funcao...);
}

/*----------------------------------------------------------------------
 *  hashFilmes [opcional]
 *
 *  Recebe uma lista de filmes LST e distribui as palavras que
 *  ocorrem nos nomes do filmes em uma tabela de dispersao 
 *  (hash table):
 *
 *     http://www.ime.usp.br/~pf/mac0122-2002/aulas/hashing.html
 *     http://www.ime.usp.br/~pf/mac0122-2003/aulas/symb-table.html
 *
 *  Antes de inserir uma palavra na tabela de dispersao todas a
 *  letras da palavra devem ser convertidas para minusculo. Com
 *  isto faremos que a busca de filmes que possuam uma dada
 *  palavra em seu nome nao seja 'case insensitive'. Para essa
 *  tarefa a funcao tolower() pode ser utilizada.
 *
 *  Esta funcao utiliza as funcoes freeST(), initST() e putFilmeST()
 *
 *  Para obter as palavras podemos escrever uma funcao peguePalavra()
 *  inspirada na funcao pegueNome do modulo lexer.c do EP3/EP4 ou
 *  ainda utilizar a funcao strtok() da biblioteca string.h:
 *  
 *      http://linux.die.net/man/3/strtok    (man page)
 *
 */
void hashFilmes(ListaFilmes *lst)
{
    AVISO(hashFilmes em filmes.c
          : Vixe ainda nao fiz essa funcao...);
}

/*Extrai um filme de uma lista e retorna seu endereco*/
Filme *extraiFilme(ListaFilmes *lst, Filme *flm)
{
    flm->prox->ant = flm->ant;
    flm->ant->prox = flm->prox;
    flm->prox = NULL;
    flm->ant = NULL;
    lst->nFilmes--;
    return flm;
}