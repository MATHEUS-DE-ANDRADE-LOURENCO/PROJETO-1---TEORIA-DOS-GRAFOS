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
#define MAX_LOCAIS 2
#define MAX_VERTICES 50

/* Cores (úteis para travessias, se necessário) */
#define BRANCO 0
#define CINZA  1
#define PRETO  2

/* Estrutura que representa uma localidade posicionada em uma aresta.
   distancia_v1 e distancia_v2 guardam a distância da localidade quando
   a aresta é observada a partir de v1 ou v2 respectivamente. */
typedef struct {
	char nome[MAX_CHARS];
	int distancia_v1;
	int distancia_v2;
} Localidade;

/* Célula da lista de adjacência (aresta) */
typedef struct Aresta{
	int extremo2;
	struct Aresta *prox;
	Localidade localidades[MAX_LOCAIS];
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
int  acrescentaAresta(Vert G[], int ordem, int v1, int v2,
					  char *local1, int distancia_1_v1, int distancia_1_v2,
					  char *local2, int distancia_2_v1, int distancia_2_v2);
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
int acrescentaAresta(Vert G[], int ordem, int v1, int v2,
					 char *local1, int distancia_1_v1, int distancia_1_v2,
					 char *local2, int distancia_2_v1, int distancia_2_v2){
	Aresta *A1, *A2;
	Localidade loc1, loc2;

	if (v1 < 0 || v1 >= ordem) return 0;
	if (v2 < 0 || v2 >= ordem) return 0;

	/* prepara localidades */
	if (local1 != NULL && local1[0] != '\0'){
		strncpy(loc1.nome, local1, MAX_CHARS-1);
		loc1.nome[MAX_CHARS-1] = '\0';
		loc1.distancia_v1 = distancia_1_v1;
		loc1.distancia_v2 = distancia_1_v2;
	} else {
		loc1.nome[0] = '\0';
		loc1.distancia_v1 = loc1.distancia_v2 = 0;
	}

	if (local2 != NULL && local2[0] != '\0'){
		strncpy(loc2.nome, local2, MAX_CHARS-1);
		loc2.nome[MAX_CHARS-1] = '\0';
		loc2.distancia_v1 = distancia_2_v1;
		loc2.distancia_v2 = distancia_2_v2;
	} else {
		loc2.nome[0] = '\0';
		loc2.distancia_v1 = loc2.distancia_v2 = 0;
	}

	/* cria aresta na lista de v1 */
	A1 = (Aresta*) malloc(sizeof(Aresta));
	if (A1 == NULL) return 0;
	A1->extremo2 = v2;
	A1->prox = G[v1].prim;
	A1->localidades[0] = loc1;
	A1->localidades[1] = loc2;
	G[v1].prim = A1;

	if (v1 == v2) return 1; /* laço */

	/* cria aresta simétrica na lista de v2 */
	A2 = (Aresta*) malloc(sizeof(Aresta));
	if (A2 == NULL) return 0;
	A2->extremo2 = v1;
	A2->prox = G[v2].prim;
	/* para o lado de v2, trocamos as distâncias: quando a localidade foi
	   fornecida para local1, as distâncias vistas a partir de v2 são as
	   que foram passadas em distancia_1_v2 (parâmetro) */
	A2->localidades[0] = loc1;
	A2->localidades[1] = loc2;
	G[v2].prim = A2;

	return 1;
}

/* Imprime grafo indicando as localidades e as distâncias relativas */
void imprimeGrafo(Vert G[], int ordem){
	int i;
	Aresta *aux;

	printf("\nOrdem: %d\n", ordem);
	printf("Lista de Adjacencia:\n");

	for(i = 0; i < ordem; i++){
		printf("\n v%d:\n", i);
		aux = G[i].prim;
		for(; aux != NULL; aux = aux->prox){
			printf("   -> v%d", aux->extremo2);
			if (aux->localidades[0].nome[0] != '\0'){
				printf("    [Local1: %s, dist_v1=%dm, dist_v2=%dm]",
					   aux->localidades[0].nome,
					   aux->localidades[0].distancia_v1,
					   aux->localidades[0].distancia_v2);
			}
			if (aux->localidades[1].nome[0] != '\0'){
				printf("    [Local2: %s, dist_v1=%dm, dist_v2=%dm]",
					   aux->localidades[1].nome,
					   aux->localidades[1].distancia_v1,
					   aux->localidades[1].distancia_v2);
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
	acrescentaAresta(*G, ordemG,1,2, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,1,8, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,2,3, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,2,7, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,3,6, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,3,4, "Oxxo", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,4,5, "Bluefit Maria Antonia", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,4,47, "Estação higienopolis Mackenzie", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,5,6, "SESC Consolação", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,5,11, "Farmácia", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,6,11, "Pão de açúcar", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,6,7, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,7,8, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,7,10, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,8,9, "Santa Casa", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,9,10, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,9,12, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,10,23, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,10,11, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,11,22, "Palacete", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,11,24, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,12,13, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,12,15, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,12,23, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,13,14, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,14,17, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,14,15, "Mambo", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,15,21, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,16,19, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,16,20, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,16,21, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,17,19, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,18,19, "Shopping Pátio Higienópolis", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,18,45, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,19,34, "Posto de Gasolina", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,20,25, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,20,33, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,20,34, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,21,22, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,22,23, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,22,25, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,24,25, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,24,27, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,25,26, "Padaria", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,26,27, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,26,29, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,26,33, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,27,47, "Universidade Persbiteriana Mackenzie", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,27,28, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,28,29, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,28,0, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,29,31, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,29,32, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,30,31, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,30,0, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,31,39, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,32,33, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,32,36, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,32,39, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,33,35, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,34,35, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,34,45, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,35,36, "Parque buenos aires", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,35,44, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,36,37, "Pão de açucar 2", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,36,43, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,37,38, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,37,39, "Pizza", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,37,42, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,38,40, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,38,41, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,38,49, "Hospital Infantil Sabará", 0,0, "Petz", 0,0);
	acrescentaAresta(*G, ordemG,39,40, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,41,42, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,41,48, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,42,43, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,43,44, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,44,45, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,46,47, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,46,0, "", 0,0, "", 0,0);
	acrescentaAresta(*G, ordemG,48,49, "", 0,0, "", 0,0);

}

int dijkstra(Vert *G, Localidade loc_origem, int origem, int destino, int dist[], int pred[]){
    // Implementação do algoritmo de Dijkstra
    // Esta função deve ser implementada conforme os requisitos do projeto
    return 0; // Retornar o custo mínimo do caminho encontrado, caso não encontre caminho, retorna -1
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

