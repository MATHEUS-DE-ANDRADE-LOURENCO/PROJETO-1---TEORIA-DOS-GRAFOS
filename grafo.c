/* 
 * REPRESENTACAO DE GRAFOS - Versao 2025/2
 */

#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<memory.h>
#include <string.h>

#define MAX_CHARS 50
#define MAX_LOCAIS 2
#define BRANCO 0
#define CINZA  1
#define PRETO  2

/* 
 * Estrutura de dados para representar grafos
 */
typedef struct a{ /* Celula de uma lista de arestas */
	int    extremo2;
	struct a *prox;
	char localidades[MAX_LOCAIS][MAX_CHARS];
}Arest;

typedef struct v{  /* Cada vertice tem um ponteiro para uma lista de arestas incidentes nele */
	int nome;
	int cor;
	Arest *prim;
}Vert;

/*
 * Declaracoes das funcoes para manipulacao de grafos 
 */
void criaGrafo(Vert **G, int ordem);
void destroiGrafo(Vert **G, int ordem);
int  acrescentaAresta(Vert G[], int ordem, int v1, int v2, char *local1, char *local2);
void imprimeGrafo(Vert G[], int ordem);

 
/*
 * Criacao de um grafo com ordem predefinida (passada como argumento),
 *   e, inicilamente, sem nenhuma aresta 
 */
void criaGrafo(Vert **G, int ordem){
	int i;
	*G= (Vert*) malloc(sizeof(Vert)*ordem); /* Alcacao dinamica de um vetor de vertices */
	
	for(i=0; i<ordem; i++){
		(*G)[i].nome= i;
		(*G)[i].cor= BRANCO;
		(*G)[i].prim= NULL;    /* Cada vertice sem nenhuma aresta incidente */
	}
}

/*
 * Deasaloca a memoria dinamica usada para armazenar um grafo.
 */
void destroiGrafo(Vert **G, int ordem){
	int i;
    Arest *a, *n;
    
	for(i=0; i<ordem; i++){ /* Remove lista de adjacencia de cada vertice */
	    a= (*G)[i].prim;
        while (a!= NULL){
              n= a->prox;
              free(a);
              a= n;
        }
	}
    free(*G);  /* Remove o vetor de vertices */
}

/*  
 * Acrescenta uma nova aresta em um grafo previamente criado. 
 *   Devem ser passados os extremos v1 e v2 da aresta a ser acrescentada 
 * Como o grafo nao e orientado, para uma aresta com extremos i e j, quando
 *   i != j, serao criadas, na estrutura de dados, arestas (i,j) e (j,i) .
 */
int acrescentaAresta(Vert G[], int ordem, int v1, int v2, char *local1, char *local2){
    Arest * A1, *A2;
    
	if (v1<0 || v1 >= ordem) /* Testo se vertices sao validos */
	   return 0;
	if (v2<0 || v2 >= ordem)
	   return 0;
	
	/* Acrescento aresta na lista do vertice v1 */
	A1= (Arest *) malloc(sizeof(Arest));
	A1->extremo2= v2;
	A1->prox= G[v1].prim;
	strcpy(A1->localidades[0], local1);
	strcpy(A1->localidades[1], local2);
	G[v1].prim= A1;

	if (v1 == v2) return 1; /* Aresta e um laco */

	/* Acrescento aresta na lista do vertice v2 se v2 != v1 */	
	A2= (Arest *) malloc(sizeof(Arest));
	A2->extremo2= v1;
	A2->prox= G[v2].prim;
	G[v2].prim= A2;
	
	return 1;
}


/*  
 * Imprime um grafo com uma notacao similar a uma lista de adjacencia.
 */
void imprimeGrafo(Vert G[], int ordem){
	int i;
	Arest *aux;

	printf("\nOrdem:   %d",ordem);
	printf("\nLista de Adjacencia:\n");

	for (i=0; i<ordem; i++){
		printf("\n  v%d: ", i);
		aux= G[i].prim;
		for( ; aux != NULL; aux= aux->prox)
			printf("\n    v%d - (%s, %s)", aux->extremo2, aux->localidades[0], aux->localidades[1]);

		
	}
	printf("\n\n");

}

/*
 * Programa simples para testar a representacao de grafo
 */
int main(int argc, char *argv[]) {
    int i;
	Vert *G;
	int ordemG= 10; /* Vertices identificado de 0 ate 9 */
		
	criaGrafo(&G, ordemG);
	acrescentaAresta(G,ordemG,0,1, "A", "B");
	acrescentaAresta(G,ordemG,0,2, "A", "C");
	acrescentaAresta(G,ordemG,0,7, "A", "H");
	acrescentaAresta(G,ordemG,2,4, "C", "E");
	acrescentaAresta(G,ordemG,2,5, "C", "F");
	acrescentaAresta(G,ordemG,2,5, "C", "F");
	acrescentaAresta(G,ordemG,3,5, "D", "F");
	acrescentaAresta(G,ordemG,4,6, "E", "G");
	acrescentaAresta(G,ordemG,3,6, "D", "G");
	acrescentaAresta(G,ordemG,7,7, "H", "H");
	acrescentaAresta(G,ordemG,8,9, "I", "J");

	imprimeGrafo(G, ordemG);
       
	destroiGrafo(&G, ordemG);
	
	printf("Pressione uma tecla para terminar\n");
    getchar();
	return(0);
}
