//Arquivo Grafo.h

struct vertice{
    int v;
    char nome[10];
};

typedef struct vertice Vertice;

struct grafo{
    int eh_ponderado;
    int nro_vertices;
    int grau_max;
    Vertice** arestas;
    float** pesos;
    int* grau;
};

typedef struct grafo Grafo;


Grafo* cria_Grafo(int nro_vertices, int grau_max, int eh_ponderado);
void libera_Grafo(Grafo* gr);
int insereAresta(Grafo* gr, char* nome, char* nomeAmigo, int orig, int dest, int eh_digrafo, int peso);

void menorCaminho_Grafo(Grafo *gr, int ini, int *antecessor, int *distancia);

int listaAmigos(Grafo *gr, int *verticeAmigos);
void pessoasProximas(Grafo* gr, char *nome);
