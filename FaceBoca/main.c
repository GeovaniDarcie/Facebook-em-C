#include <stdio.h>
#include <stdlib.h>
#include "Grafo.h"
#include <time.h>
#include <string.h>
#include <windows.h>
#include <locale.h>

int main()
{
    //cor da tela
    system("color 9F");
    setlocale(LC_ALL, "Portuguese");

    char nome[10];
    int opcao;

    printf("+------------------------+\n");
    printf("| Bem vindo ao Faceboca! |\n");
    printf("+------------------------+\n\n");

    Grafo* gr = cria_Grafo(9,9,1);

        printf("Digite seu primeiro nome: ");
        scanf("%s", &nome);
        printf("Usuário registrado com sucesso!\n\n");
        printf("Olá %s bem vindo ao Faceboca!\n\n", nome);

        insereAresta(gr, nome, "joão", 0, 1, 0, 3);
        insereAresta(gr, nome, "pedro", 0, 2, 0, 4);
        insereAresta(gr, "pedro", "roberto", 2, 3, 0, 5);
        insereAresta(gr, "pedro", "letícia", 2, 4, 0, 6);
        insereAresta(gr, "letícia", "odilon", 4, 5, 0, 7);
        insereAresta(gr, "odilon", "adam", 5, 6, 0, 8);
        insereAresta(gr, "adam", "noah", 6, 7, 0, 9);
        insereAresta(gr, "adam", "jonas", 6, 8, 0, 10);

        int amigos[gr->grau_max];

        do{
            Sleep(1000);
            printf("+-----------------+\n");
            printf("| Menu de opcões: |\n");
            printf("+-----------------+\n\n");
            printf("[1] Lista de amigos\n");
            printf("[2] Pessoas que você talvez conheça\n");
            printf("[3] Sair\n\n");

            printf("Digite a opção desejada: ");
            scanf("%d", &opcao);
            printf("\n");

            switch (opcao){
                case 1: listaAmigos(gr, amigos);
                break;
                case 2: pessoasProximas(gr, nome);
                break;
                case 3: exit(1);
                libera_Grafo(gr);
                break;
            }
        } while(opcao != 3);

    system("pause");
    return 0;
}
