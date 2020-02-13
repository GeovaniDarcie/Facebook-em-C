#include <stdio.h>
#include <stdlib.h>
#include "Grafo.h"
#include <string.h>
#include <locale.h>


Grafo* cria_Grafo(int nro_vertices, int grau_max, int eh_ponderado)
{
    Grafo *gr;
    gr = (Grafo*) malloc(sizeof(struct grafo));
    if(gr != NULL)
    {
        int i;
        gr->nro_vertices = nro_vertices;
        gr->grau_max = grau_max;
        gr->grau = (int*) calloc(nro_vertices,sizeof(int));

        gr->arestas = (Vertice**) malloc(nro_vertices * sizeof(Vertice*));
        for(i=0; i<nro_vertices; i++)
            gr->arestas[i] = (Vertice*) malloc(grau_max * sizeof(Vertice));

        if(gr->eh_ponderado)
        {
            gr->pesos = (float**) malloc(nro_vertices * sizeof(float*));
            for(i=0; i<nro_vertices; i++)
                gr->pesos[i] = (float*) malloc(grau_max * sizeof(float));
        }
    }
    return gr;
}

void libera_Grafo(Grafo* gr)
{
    if(gr != NULL)
    {
        int i;
        for(i=0; i<gr->nro_vertices; i++)
            free(gr->arestas[i]);
        free(gr->arestas);
        if(gr->eh_ponderado)
        {
            for(i=0; i<gr->nro_vertices; i++)
                free(gr->pesos[i]);
            free(gr->pesos);
        }
        free(gr->grau);
        free(gr);
    }
}

int insereAresta(Grafo* gr, char* nome, char* nomeAmigo, int orig, int dest, int eh_digrafo, int peso)
{
    //verifica se o vertice orig e dest são válidos
    if(gr == NULL)
        return 0;
    if(orig < 0 || orig >= gr->nro_vertices)
        return 0;
    if(dest < 0 || dest >= gr->nro_vertices)
        return 0;

    //pega o endereço da aresta e coloca numa variável aux
    Vertice *aux;
    aux = &gr->arestas[orig][gr->grau[orig]];

    //insere o vértice de destino no endereço
    aux->v = dest;

    //inserindo o peso das arestas
    if(gr->eh_ponderado)
        gr->pesos[orig][gr->grau[orig]] = peso;

    //inserindo nome do usuario
    if(eh_digrafo == 0)
    {
        // aqui ele coloca o nome do usuário em todos os graus do vértice orig , como se o vértice orig tivesse um nome
        int i, aux2, j;
        aux2 = orig;
        for(i=0; i<gr->grau_max; i++)
        {
            aux = &gr->arestas[orig][aux2];
            for(j=0; j<strlen(nome); j++)
            {
                aux->nome[j] = nome[j];
            }
            aux2++;
        }

    }
    else
    {
        //inserindo nome do amigo, quando o eh_digrafo retornar 1, cai dentro desse else
        int i;
        for(i=0; i<=strlen(nomeAmigo); i++)
        {
            aux->nome[i] = nomeAmigo[i];
        }
    }
    gr->grau[orig]++;

    if(eh_digrafo == 0)
        insereAresta(gr,nome,nomeAmigo, dest,orig,1,peso);
    return 1;
}

int procuraMenorDistancia(int *dist, int *visitado, int NV)
{
    int i, menor = -1, primeiro = 1;
    for(i=0; i < NV; i++)
    {
        if(dist[i] >= 0 && visitado[i] == 0)
        {
            if(primeiro)
            {
                menor = i;
                primeiro = 0;
            }
            else
            {
                if(dist[menor] > dist[i])
                    menor = i;
            }
        }
    }
    return menor;
}

void menorCaminho_Grafo(Grafo *gr, int ini, int *ant, int *dist)
{
    int i, cont, NV, *visitado, vert;
    Vertice ind;

    cont = NV = gr->nro_vertices;
    visitado = (int*) malloc(NV * sizeof(int));
    for(i=0; i < NV; i++)
    {
        ant[i] = -1;
        dist[i] = -1;
        visitado[i] = 0;
    }
    dist[ini] = 0;
    while(cont > 0)
    {
        vert = procuraMenorDistancia(dist, visitado, NV);
        if(vert == -1)
            break;
        visitado[vert] = 1;
        cont--;
        for(i=0; i<gr->grau[vert]; i++)
        {
            ind = gr->arestas[vert][i];
            if(dist[ind.v] < 0)
            {
                dist[ind.v] = dist[vert] + gr->pesos[vert][i];
                ant[ind.v] = vert;
            }
            else
            {
                if(dist[ind.v] > dist[vert] + gr->pesos[vert][i])
                {
                    dist[ind.v] = dist[vert] + gr->pesos[vert][i];
                    ant[ind.v] = vert;
                }
            }
        }
    }
    free(visitado);

}

int listaAmigos(Grafo *gr, int *verticeAmigos)
{
    setlocale(LC_ALL, "Portuguese");
    // verifica todos os vértices ligados ao vértice 0, e printa na tela o nome do vértice encontrado
    system("cls");
    printf("+--------------------+\n");
    printf(" Lista de amigos--->       \n\n");

    int i, amigo = 0;


    Vertice *aux;


    for(i=0; i<gr->grau_max; i++)
    {
        aux = &gr->arestas[0][i];
        if(aux->v > 0 && aux->v < gr->nro_vertices)
        {
            verticeAmigos[i] = aux->v;
            aux = &gr->arestas[aux->v][0];
            printf(" -%s\n", aux->nome);
            amigo++;
        }
    }
    printf("\n");
    printf("Você tem %d amigo(s)\n\n", amigo);
    printf("+--------------------+\n\n");
    return amigo;

}

void pessoasProximas(Grafo* gr, char* nome)
{
    setlocale(LC_ALL, "Portuguese");
    int amigos[gr->grau_max];
    int qAmigos;
    int opcao;
    static char ignorados[15];
    static int ind;

    qAmigos = listaAmigos(gr, amigos);

    int ant[gr->nro_vertices];
    int dist[gr->nro_vertices];

    menorCaminho_Grafo(gr, 0, ant, dist);

    int primeiro = 1;
    int i, j, menor2=0, menor = dist[primeiro], verticeMenor = 0;
    Vertice *v;

    for(i=0; i<gr->nro_vertices; i++)
    {
        if(dist[i] <= menor && dist[i]> menor2 && i != 0)
        {
            menor = dist[i];
            verticeMenor = i;
        }
        for(j=0; j<qAmigos; j++)
        {
            if(verticeMenor == amigos[j])
            {
                menor2 = dist[i];
                primeiro++;
                menor = dist[primeiro];

            }
        }

         for(j=0; j<strlen(ignorados); j++)
        {
            if(ignorados[j] == verticeMenor)
            {
                menor2 = dist[i];
                primeiro++;
                menor = dist[primeiro];
            }
        }
    }

    v = &gr->arestas[verticeMenor][0];
    system("cls");
    printf("\n");
    printf("Usuário mais próximo de você: %s\n\n", v->nome);

    printf("[1] Enviar solitação de amizade\n");
    printf("[2] Ignorar\n");
    printf("Digite a opção desejada: ");
    scanf("%d", &opcao);
    if(opcao == 1){
        insereAresta(gr, nome, v->nome, 0, verticeMenor, 0, 8);
        printf("%s aceitou sua solitação de amizade!\n\n", v->nome);
    }
    if(opcao == 2){
         ignorados[ind] = verticeMenor;
         ind++;
    }
}




