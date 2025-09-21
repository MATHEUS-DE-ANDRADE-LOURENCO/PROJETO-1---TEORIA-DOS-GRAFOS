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
	acrescentaAresta(G,ordemG,1,2, "", "");
	acrescentaAresta(G,ordemG,1,8, "", "");
	acrescentaAresta(G,ordemG,2,3, "", "");
	acrescentaAresta(G,ordemG,2,7, "", "");
	acrescentaAresta(G,ordemG,3,6, "", "");
	acrescentaAresta(G,ordemG,3,4, "Oxxo", "");
	acrescentaAresta(G,ordemG,4,5, "Bluefit Maria Antonia", "");
	acrescentaAresta(G,ordemG,4,47, "Estação higienopolis Mackenzie", "");
	acrescentaAresta(G,ordemG,5,6, "SESC Consolação", "");
	acrescentaAresta(G,ordemG,5,11, "Farmácia", "");
	acrescentaAresta(G,ordemG,6,11, "Pão de açúcar", "");
	acrescentaAresta(G,ordemG,6,7, "", "");
	acrescentaAresta(G,ordemG,7,8, "", "");
	acrescentaAresta(G,ordemG,7,10, "", "");
	acrescentaAresta(G,ordemG,8,9, "Santa Casa", "");
	acrescentaAresta(G,ordemG,9,10, "", ""); 
	acrescentaAresta(G,ordemG,9,12, "", "");
	acrescentaAresta(G,ordemG,10,23, "", "");
	acrescentaAresta(G,ordemG,10,11, "", "");
	acrescentaAresta(G,ordemG,11,22, "Palacete", ""); 
	acrescentaAresta(G,ordemG,11,24, "", "");
	acrescentaAresta(G,ordemG,12,13, "", "");
	acrescentaAresta(G,ordemG,12,15, "", "");
	acrescentaAresta(G,ordemG,12,23, "", "");
	acrescentaAresta(G,ordemG,13,14, "", ""); 
	acrescentaAresta(G,ordemG,14,17, "", "");
	acrescentaAresta(G,ordemG,14,15, "Mambo", ""); 
	acrescentaAresta(G,ordemG,15,21, "", ""); 
	acrescentaAresta(G,ordemG,16,19, "", "");
	acrescentaAresta(G,ordemG,16,20, "", ""); 
	acrescentaAresta(G,ordemG,16,21, "", "");
	acrescentaAresta(G,ordemG,17,19, "", "");
	acrescentaAresta(G,ordemG,18,19, "Shopping Pátio Higienópolis", "");
	acrescentaAresta(G,ordemG,18,45, "", "");
	acrescentaAresta(G,ordemG,19,34, "Posto de Gasolina", ""); 
	acrescentaAresta(G,ordemG,20,25, "", "");
	acrescentaAresta(G,ordemG,20,33, "", "");
	acrescentaAresta(G,ordemG,20,34, "", "");
	acrescentaAresta(G,ordemG,21,22, "", "");
	acrescentaAresta(G,ordemG,22,23, "", "");
	acrescentaAresta(G,ordemG,22,25, "", "");
	acrescentaAresta(G,ordemG,24,25, "", "");
	acrescentaAresta(G,ordemG,24,27, "", "");
	acrescentaAresta(G,ordemG,25,26, "Padaria", "");
	acrescentaAresta(G,ordemG,26,27, "", "");
	acrescentaAresta(G,ordemG,26,29, "", "");
	acrescentaAresta(G,ordemG,26,33, "", "");
	acrescentaAresta(G,ordemG,27,47, "Universidade Persbiteriana Mackenzie", "");
	acrescentaAresta(G,ordemG,27,28, "", "");
	acrescentaAresta(G,ordemG,28,29, "", "");
	acrescentaAresta(G,ordemG,28,0, "", "");
	acrescentaAresta(G,ordemG,29,31, "", "");
	acrescentaAresta(G,ordemG,29,32, "", "");
	acrescentaAresta(G,ordemG,30,31, "", "");
	acrescentaAresta(G,ordemG,30,0, "", "");
	acrescentaAresta(G,ordemG,31,39, "", "");
	acrescentaAresta(G,ordemG,32,33, "", "");
	acrescentaAresta(G,ordemG,32,36, "", "");
	acrescentaAresta(G,ordemG,32,39, "", "");
	acrescentaAresta(G,ordemG,33,35, "", "");
	acrescentaAresta(G,ordemG,34,35, "", "");
	acrescentaAresta(G,ordemG,34,45, "", "");
	acrescentaAresta(G,ordemG,35,36, "Parque buenos aires", "");
	acrescentaAresta(G,ordemG,35,44, "", "");
	acrescentaAresta(G,ordemG,36,37, "Pão de açucar 2", "");
	acrescentaAresta(G,ordemG,36,43, "", "");
	acrescentaAresta(G,ordemG,37,38, "", "");
	acrescentaAresta(G,ordemG,37,39, "Pizza", "");
	acrescentaAresta(G,ordemG,37,42, "", "");
	acrescentaAresta(G,ordemG,38,40, "", "");
	acrescentaAresta(G,ordemG,38,41, "", "");
	acrescentaAresta(G,ordemG,38,49, "Hospital Infantil Sabará", "Petz");
	acrescentaAresta(G,ordemG,39,40, "", "");
	acrescentaAresta(G,ordemG,41,42, "", "");
	acrescentaAresta(G,ordemG,41,48, "", "");
	acrescentaAresta(G,ordemG,42,43, "", "");
	acrescentaAresta(G,ordemG,43,44, "", "");
	acrescentaAresta(G,ordemG,44,45, "", "");
	acrescentaAresta(G,ordemG,46,47, "", "");
	acrescentaAresta(G,ordemG,46,0, "", "");
	acrescentaAresta(G,ordemG,48,49, "", "");
	
	

	imprimeGrafo(G, ordemG);
       
	destroiGrafo(&G, ordemG);
	
	printf("Pressione uma tecla para terminar\n");
    getchar();
	return(0);
}
