/*
 * grafo_2bim.c
 * Versão ajustada para a Parte 2 do projeto: separa construção do grafo em
 * função `constroiGrafo` e mantém localidades posicionadas em arestas.
 * ANSI C, comentários em português.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHARS 50
#define MAX_VERTICES 50
#define DIST_PADRAO 5 /*apenas para testes, apagar depois*/

/* Cores (úteis para travessias, se necessário) */
#define BRANCO 0
#define CINZA  1
#define PRETO  2

/* Estrutura que representa uma localidade posicionada em uma aresta.
   distancia_v e distancia_v guardam a distância da localidade quando
   a aresta é observada a partir de v1 ou v2 respectivamente. */
typedef struct {
	char nome[MAX_CHARS];
	int distancia_v;
}Localidade;

/* Célula da lista de adjacência (aresta) */
typedef struct Aresta{
	int extremo2;
	struct Aresta *prox;
	int dist_prox; /* distância para o próximo vértice */
	Localidade localidade;
} Aresta;

/* Vértice */
typedef struct Vertice{
	int id;
	int cor;
	Aresta *prim;
} Vert;

/* Protótipos */
void criaGrafo(Vert **G, int ordem);
void destroiGrafo(Vert **G, int ordem);
int  acrescentaAresta(Vert G[], int ordem, int v1, int v2, int dist_prox,
					 char *localidade, int distancia_v1, int distancia_v2);
void imprimeGrafo(Vert G[], int ordem);
void constroiGrafo(Vert **G, int *ordem);

/* Cria vetor de vértices e inicializa listas de adjacência */
void criaGrafo(Vert **G, int ordem){
	int i;
	*G = (Vert*) malloc(sizeof(Vert) * ordem);
	if (*G == NULL){
		fprintf(stderr, "Erro de alocacao\n");
		exit(EXIT_FAILURE);
	}
	for(i = 0; i < ordem; i++){
		(*G)[i].id = i;
		(*G)[i].cor = BRANCO;
		(*G)[i].prim = NULL;
	}
}

/* Destroi grafo liberando listas e vetor */
void destroiGrafo(Vert **G, int ordem){
	int i;
	Aresta *a, *n;
	if (*G == NULL) return;
	for(i = 0; i < ordem; i++){
		a = (*G)[i].prim;
		while(a != NULL){
			n = a->prox;
			free(a);
			a = n;
		}
	}
	free(*G);
	*G = NULL;
}

/* Acrescenta aresta não orientada; armazena até 2 localidades por aresta
   com as distâncias relativas informadas para cada lado (v1 e v2).
   Retorna 1 em sucesso, 0 caso vértices inválidos ou falha de alocação. */
int acrescentaAresta(Vert G[], int ordem, int v1, int v2, int dist_prox,
					 char *localidade, int distancia_v1, int distancia_v2){
	Aresta *A1, *A2;
	Localidade loc, loc_v2;

	if (v1 < 0 || v1 >= ordem) return 0;
	if (v2 < 0 || v2 >= ordem) return 0;

	/* prepara localidades */

	/*Se nome da localidade nao for nulo (ou seja, se existe localidade)*/
	if (localidade != NULL && localidade[0] != '\0'){ /*atribuicao da localidade a aresta*/
		strncpy(loc.nome, localidade, MAX_CHARS-1);
		loc.nome[MAX_CHARS-1] = '\0';
		loc.distancia_v = distancia_v1;

		strncpy(loc_v2.nome, localidade,  MAX_CHARS-1);
		loc_v2.nome[MAX_CHARS-1] = '\0';
		loc_v2.distancia_v = distancia_v2;
	} else { /*sem localidade: manter nulo*/
		loc.nome[0] = '\0';
		loc.distancia_v = 0;

		loc_v2.nome[0] = '\0';
		loc_v2.distancia_v = 0;
	}

	/* cria aresta na lista de v1 */
	A1 = (Aresta*) malloc(sizeof(Aresta));
	if (A1 == NULL) return 0;
	A1->extremo2 = v2;
	A1->prox = G[v1].prim;
	A1->dist_prox = dist_prox;
	A1->localidade = loc;
	G[v1].prim = A1;

	if (v1 == v2) return 1; /* se for um laço */

	/* cria aresta simétrica na lista de v2 */
	A2 = (Aresta*) malloc(sizeof(Aresta));
	if (A2 == NULL) return 0;
	A2->extremo2 = v1;
	A2->prox = G[v2].prim;
	A2->dist_prox = dist_prox;
	A2->localidade = loc_v2;
	G[v2].prim = A2;

	return 1;
}

/* Imprime grafo indicando as localidades e as distâncias relativas */
void imprimeGrafo(Vert G[], int ordem){
	int i;
	Aresta *aux;

	printf("\nOrdem: %d\n", ordem);
	printf("Lista de Adjacencia:\n");

	/*itera sobre os vertices do grafo*/
	for(i = 0; i < ordem; i++){
		printf("\n v%d:\n", G[i].id);
		aux = G[i].prim;
		for(; aux != NULL; aux = aux->prox){ /*itera sobre as arestas do vertice*/
			printf("   -> v%d: dist=%d", aux->extremo2, aux->dist_prox);
			if (aux->localidade.nome[0] != '\0'){
				printf("\n     [Local: %s, dist_v=%dm]",
					   aux->localidade.nome,
					   aux->localidade.distancia_v);
			}
			printf("\n");
		}
	}
	printf("\n");
}

/* Constroi o grafo: Contém a configuração das arestas conforme grafo obtido*/
void constroiGrafo(Vert **G, int *ordem){
	int ordemG = 50;
	criaGrafo(G, ordemG);
	*ordem = ordemG;

	/* chamadas para adicionar arestas (distâncias padrões = 0) */
	acrescentaAresta(*G, ordemG, 1, 2, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG, 1,8, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,2,3, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,2,7, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,3,6, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,3,4, DIST_PADRAO, "Oxxo", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,4,5, DIST_PADRAO, "Bluefit Maria Antonia", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,4,47, DIST_PADRAO, "Estação higienopolis Mackenzie", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,5,6, DIST_PADRAO, "SESC Consolação", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,5,11, DIST_PADRAO, "Farmácia", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,6,11, DIST_PADRAO, "Pão de açúcar", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,6,7, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,7,8, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,7,10, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,8,9, DIST_PADRAO, "Santa Casa", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,9,10, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,9,12, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,10,23, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,10,11, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,11,22, DIST_PADRAO, "Palacete", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,11,24, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,12,13, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,12,15, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,12,23, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,13,14, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,14,17, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,14,15, DIST_PADRAO, "Mambo", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,15,21, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,16,19, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,16,20, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,16,21, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,17,19, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,18,19, DIST_PADRAO, "Shopping Pátio Higienópolis", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,18,45, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,19,34, DIST_PADRAO, "Posto de Gasolina", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,20,25, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,20,33, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,20,34, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,21,22, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,22,23, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,22,25, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,24,25, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,24,27, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,25,26, DIST_PADRAO, "Padaria", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,26,27, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,26,29, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,26,33, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,27,47, DIST_PADRAO, "Universidade Persbiteriana Mackenzie", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,27,28, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,28,29, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,28,0, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,29,31, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,29,32, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,30,31, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,30,0, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,31,39, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,32,33, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,32,36, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,32,39, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,33,35, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,34,35, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,34,45, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,35,36, DIST_PADRAO, "Parque buenos aires", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,35,44, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,36,37, DIST_PADRAO, "Pão de açucar 2", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,36,43, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,37,38, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,37,39, DIST_PADRAO, "Pizza", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,37,42, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,38,40, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,38,41, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,38,49, DIST_PADRAO, "Hospital Infantil Sabará", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,39,40, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,41,42, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,41,48, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,42,43, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,43,44, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,44,45, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,46,47, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,46,0, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
	acrescentaAresta(*G, ordemG,48,49, DIST_PADRAO, "", DIST_PADRAO, DIST_PADRAO);
}

/*TODO: PIETRO*/
int dijkstra(Vert *G, Localidade loc_origem, int origem, int destino, int dist[], int pred[]){
    // Implementação do algoritmo de Dijkstra
    // Esta função deve ser implementada conforme os requisitos do projeto
    return 0; // Retornar o custo mínimo do caminho encontrado, caso não encontre caminho, retorna -1
}

/*TODO: PIETRO*/
void imprimeCaminho(int origem, int destino, int pred[]){
	// Implementação para imprimir o caminho mínimo encontrado
	// Esta função deve ser implementada conforme os requisitos do projeto
}

/*Função main*/
int main(){
	Vert *G = NULL;
	int ordem = 0;

	constroiGrafo(&G, &ordem);
	imprimeGrafo(G, ordem);
	destroiGrafo(&G, ordem);

	printf("Pressione uma tecla para terminar\n");
	getchar();
	return 0;
}

