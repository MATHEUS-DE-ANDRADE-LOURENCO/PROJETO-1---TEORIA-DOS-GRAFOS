/*
 * grafo_2bim.c
 * Versão ajustada para a Parte 2 do projeto: separa construção do grafo em
 * função constroiGrafo e mantém localidades posicionadas em arestas.
 * ANSI C, comentários em português.
 */
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHARS 51


#define BRANCO 0
#define CINZA  1
#define PRETO  2


enum letras {
    A=65,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z
};


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
	int pai; /*Id do vertice pai no algoritmo de Dikstra*/
	int dist; /*Distancia do vertice até o vertice inicial no algoritmo de Dikstra*/
} Vert;

/* Protótipos */
void criaGrafo(Vert **G, int ordem);
void destroiGrafo(Vert **G, int ordem);
int  acrescentaAresta(Vert G[], int ordem, int v1, int v2, int dist_prox, char *localidade, int distancia_v1, int distancia_v2);
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
		(*G)[i].pai = NULL;
		(*G)[i].dist = INT_MAX;
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
	int ordemG = 51;
	criaGrafo(G, ordemG);
	*ordem = ordemG;

	/* chamadas para adicionar arestas (distâncias padrões = 0) */
	acrescentaAresta(*G, ordemG, 1, 2, 90, "", 0, 0);
	acrescentaAresta(*G, ordemG, 1,8, 130, "", 0, 0);
	acrescentaAresta(*G, ordemG,2,3, 79, "", 0, 0);
	acrescentaAresta(*G, ordemG,2,7, 135, "", 0, 0);
	acrescentaAresta(*G, ordemG,3,6, 140, "", 0, 0);
	acrescentaAresta(*G, ordemG,3,4, 260, "Oxxo", 30, 230);
	acrescentaAresta(*G, ordemG,4,5, 185, "Bluefit Maria Antonia", 140, 45);
	acrescentaAresta(*G, ordemG,4,47, 352, "Estacao higienopolis Mackenzie", 337, 15);
	acrescentaAresta(*G, ordemG,5,6, 151, "SESC Consolacao", 75, 76);
	acrescentaAresta(*G, ordemG,5,11, 193, "Farmacia", 72, 121);
	acrescentaAresta(*G, ordemG,6,11, 184, "Pao de acucar", 157, 27);
	acrescentaAresta(*G, ordemG,6,7, 87, "", 0, 0);
	acrescentaAresta(*G, ordemG,7,8, 90, "", 0, 0);
	acrescentaAresta(*G, ordemG,7,10, 154, "", 0, 0);
	acrescentaAresta(*G, ordemG,8,9, 123, "Santa Casa", 23, 100);
	acrescentaAresta(*G, ordemG,9,10, 97, "", 0, 0);
	acrescentaAresta(*G, ordemG,9,12, 170, "", 0, 0);
	acrescentaAresta(*G, ordemG,10,23, 166, "", 0, 0);
	acrescentaAresta(*G, ordemG,10,11, 80, "", 0, 0);
	acrescentaAresta(*G, ordemG,11,22, 170, "Palacete", 60, 110);
	acrescentaAresta(*G, ordemG,11,24, 120, "", 0, 0);
	acrescentaAresta(*G, ordemG,12,13, 100, "", 0, 0);
	acrescentaAresta(*G, ordemG,12,15, 133, "", 0, 0);
	acrescentaAresta(*G, ordemG,12,23, 36, "", 0, 0);
	acrescentaAresta(*G, ordemG,13,14, 140, "", 0, 0);
	acrescentaAresta(*G, ordemG,14,17, 160, "", 0, 0);
	acrescentaAresta(*G, ordemG,14,15, 110, "Mambo", 50, 60);
	acrescentaAresta(*G, ordemG,15,21, 80, "", 0, 0);
	acrescentaAresta(*G, ordemG,16,19, 156, "", 0, 0);
	acrescentaAresta(*G, ordemG,16,20, 105, "", 0, 0);
	acrescentaAresta(*G, ordemG,16,21, 67, "", 0, 0);
	acrescentaAresta(*G, ordemG,17,19, 118, "", 0, 0);
	acrescentaAresta(*G, ordemG,18,19, 275, "Shopping Patio Higienopolis", 130, 145);
	acrescentaAresta(*G, ordemG,18,45, 97, "", 0, 0);
	acrescentaAresta(*G, ordemG,19,34, 90, "Posto de Gasolina", 50, 40);
	acrescentaAresta(*G, ordemG,20,25, 151, "", 0, 0);
	acrescentaAresta(*G, ordemG,20,33, 155, "", 0, 0);
	acrescentaAresta(*G, ordemG,20,34, 170, "", 0, 0);
	acrescentaAresta(*G, ordemG,21,22, 95, "", 0, 0);
	acrescentaAresta(*G, ordemG,22,23, 90, "", 0, 0);
	acrescentaAresta(*G, ordemG,22,25, 110, "", 0, 0);
	acrescentaAresta(*G, ordemG,24,25, 140, "", 0, 0);
	acrescentaAresta(*G, ordemG,24,27, 110, "", 0, 0);
	acrescentaAresta(*G, ordemG,25,26, 99, "Padaria", 68, 31);
	acrescentaAresta(*G, ordemG,26,27, 115, "", 0, 0);
	acrescentaAresta(*G, ordemG,26,29, 130, "", 0, 0);
	acrescentaAresta(*G, ordemG,26,33, 160, "Minha Casa", 70, 90);
	acrescentaAresta(*G, ordemG,27,47, 240, "Universidade Persbiteriana Mackenzie", 150, 90);
	acrescentaAresta(*G, ordemG,27,28, 143, "", 0, 0);
	acrescentaAresta(*G, ordemG,28,29, 75, "", 0, 0);
	acrescentaAresta(*G, ordemG,28,50, 37, "", 0, 0);
	acrescentaAresta(*G, ordemG,29,31, 130, "", 0, 0);
	acrescentaAresta(*G, ordemG,29,32, 158, "", 0, 0);
	acrescentaAresta(*G, ordemG,30,31, 35, "", 0, 0);
	acrescentaAresta(*G, ordemG,30,50, 95, "", 0, 0);
	acrescentaAresta(*G, ordemG,31,39, 155, "", 0, 0);
	acrescentaAresta(*G, ordemG,32,33, 130, "", 0, 0);
	acrescentaAresta(*G, ordemG,32,36, 170, "", 0, 0);
	acrescentaAresta(*G, ordemG,32,39, 122, "", 0, 0);
	acrescentaAresta(*G, ordemG,33,35, 163, "", 0, 0);
	acrescentaAresta(*G, ordemG,34,35, 117, "", 0, 0);
	acrescentaAresta(*G, ordemG,34,45, 291, "", 0, 0);
	acrescentaAresta(*G, ordemG,35,36, 137, "Parque buenos aires", 94, 43);
	acrescentaAresta(*G, ordemG,35,44, 220, "", 0, 0);
	acrescentaAresta(*G, ordemG,36,37, 130, "Pao de acucar 2", 40, 90);
	acrescentaAresta(*G, ordemG,36,43, 207, "", 0, 0);
	acrescentaAresta(*G, ordemG,37,38, 173, "", 0, 0);
	acrescentaAresta(*G, ordemG,37,39, 167, "Pizza", 121, 46);
	acrescentaAresta(*G, ordemG,37,42, 203, "", 0, 0);
	acrescentaAresta(*G, ordemG,38,40, 168, "", 0, 0);
	acrescentaAresta(*G, ordemG,38,41, 163, "", 0, 0);
	acrescentaAresta(*G, ordemG,38,49, 161, "Hospital Infantil Sabara", 61, 100);
	acrescentaAresta(*G, ordemG,39,40, 165, "", 0, 0);
	acrescentaAresta(*G, ordemG,41,42, 171, "", 0, 0);
	acrescentaAresta(*G, ordemG,41,48, 193, "", 0, 0);
	acrescentaAresta(*G, ordemG,42,43, 141, "", 0, 0);
	acrescentaAresta(*G, ordemG,43,44, 135, "", 0, 0);
	acrescentaAresta(*G, ordemG,44,45, 163, "", 0, 0);
	acrescentaAresta(*G, ordemG,46,47, 170, "", 0, 0);
	acrescentaAresta(*G, ordemG,46,50, 180, "", 0, 0);
	acrescentaAresta(*G, ordemG,48,49, 60, "", 0, 0);
}

int menorVertice(Vert G[], int ordem){

    int idMenor = -1;
    int menorDist = INT_MAX;

    for(int i = 1; i < ordem; i++){
        if(G[i].cor != PRETO && G[i].dist < menorDist){
            menorDist = G[i].dist;
            idMenor = i;
        }
    }
    return idMenor;
}



int dijkstra(Vert G[], int ordem, char *origem, char *destino){
    //Inicialização do dijkstra
    for(int i = 0; i < ordem; i++){
		G[i].pai = NULL;
		G[i].cor = BRANCO;
		G[i].dist = INT_MAX;

    }
    int verticeAtual;
    int verticeOrigem1;
    int verticeOrigem2;
    int distanciaOrigem1;
    int distanciaOrigem2;
    int verticeDestino1;
    int verticeDestino2;
    int distanciaDestino1;
    int distanciaDestino2;
    Aresta *aux;

    //Encontrar o id do vertice a partir do nome da localidade de origem
    //Os dois extremos da aresta da localidade são armazenados pois ainda não é possivel saber
    //a partir de qual vertice a rota sera feita
    for(int i = 1; i < ordem; i++){
		aux = G[i].prim;
		for(; aux != NULL; aux = aux->prox){
			if (strcmp(aux->localidade.nome, origem)==0){
		        verticeOrigem2 = i;
                verticeOrigem1 = aux->extremo2;
                distanciaOrigem1 =  aux->localidade.distancia_v;
                distanciaOrigem2 = aux->dist_prox-aux->localidade.distancia_v;

			}
		}
	}
	//Encontrar o id do vertice a partir do nome da localidade de destino
    for(int i = 1; i < ordem; i++){
		aux = G[i].prim;
		for(; aux != NULL; aux = aux->prox){
			if (strcmp(aux->localidade.nome, destino)==0){
                verticeDestino1 = i;
                verticeDestino2 = aux->extremo2;
                distanciaDestino1 =  aux->localidade.distancia_v;
                distanciaDestino2 = aux->dist_prox-aux->localidade.distancia_v;

			}
		}
	}

    G[verticeOrigem1].dist=distanciaOrigem2;
	G[verticeOrigem2].dist=distanciaOrigem1;
	G[verticeOrigem1].pai = -2;
	G[verticeOrigem2].pai = -2;

    /*Determina o menor caminho dos vertices adjacentes ao vertice de menor valor*/
	for(int i = 0;(verticeAtual= menorVertice(G,ordem))!=-1;i++){
        aux = G[verticeAtual].prim;
		for(; aux != NULL; aux = aux->prox){
            if(G[aux->extremo2].dist>aux->dist_prox+G[verticeAtual].dist){
                G[aux->extremo2].dist = aux->dist_prox+G[verticeAtual].dist; /*Redefine a distancia do vertice*/
                G[aux->extremo2].pai = verticeAtual;/*Altera o atributo pai do vertice*/

            }
        }
			G[verticeAtual].cor = PRETO; /*Quando todos os vertices adjacentes são explorados, o vertice se torna preto */
	}
	//Adiciona a distancia do vertice até a localidade
    int distancia1 = G[verticeDestino1].dist+distanciaDestino1;
    int distancia2 = G[verticeDestino2].dist+distanciaDestino2;

    //Escolhe qual o melhor vertice a ser utilizado como "vertice destino" pelo motivo da localidade estar na aresta
    if(distancia1<distancia2){
        //A distancia1 é retornada como negativo para facil identificação do "vertice de destino"
        return -distancia1;
    }else{
        return distancia2;
    }
}



void imprimeCaminho(Vert G[], int destino){
    int vertice = destino;
    while(vertice !=-2){
        printf("Vertice : %d\n", G[vertice].id);
        vertice = G[vertice].pai;
    }
}

int fatorial(int n){
    int total = 1;
    for(;n!=1;n--){
        total *= n;
    }
    return total;
}

void trocaValores(char *vetor, int primeiro, int segundo){
    char temp = vetor[primeiro];
    vetor[primeiro]  = vetor[segundo];
    vetor[segundo] = temp;
}

//Algoritmo com backtracking para definir todas as permutações de localidades possiveis
void geraPermutacoes(char vetor[], int inicio, int fim, char rotas[][26], int *indice) {
    if(inicio == fim) {
        strcpy(rotas[*indice], vetor);
        (*indice)++;
        return;
    }

    for(int i = inicio; i <= fim; i++) {
        char temp = vetor[inicio];
        vetor[inicio] = vetor[i];
        vetor[i] = temp;
        geraPermutacoes(vetor, inicio + 1, fim, rotas, indice);
        temp = vetor[inicio];
        vetor[inicio] = vetor[i];
        vetor[i] = temp;
    }
}



void melhorRota(Vert G[], int ordem, char *lugares[], int nLugares){
    int distancias[nLugares][nLugares];
    int distanciaMin = INT_MAX;
    int temp;
    int verticeDestino1;
    int verticeDestino2;

    char rotaAtual[26];
    char melhorRota[26];
    int possibilidades = fatorial(nLugares);
    char rotas[possibilidades][26];
    int indice = 0;

    //atribui a rota inicial = ordem das localidades fornecidas
    for(int i = 0; i < nLugares; i++){
        rotaAtual[i] = 'A' + i;
    }

    //gera as permutações de rotas e as armazena em rotas
    geraPermutacoes(rotaAtual, 0, nLugares - 1, rotas, &indice);
    //Calcula as distancias entre todas as localidades e armazena em uma matriz de adjacencia
    for(int i = 0; i < nLugares; i++){
        for(int j = 0; j < nLugares; j++){
            if(i == j){
                distancias[i][j] = -1;
            } else {
                temp = dijkstra(G, ordem, lugares[i], lugares[j]);
                if(temp<0){
                    temp = -temp;
                }
                distancias[i][j] = temp;
            }
        }
    }
    //Itera sobre todas as rotas, calculando a distancia total
    for(int r = 0; r < possibilidades; r++){
        int distancia = 0;
        int temp;
        //calcula a distancia da casa ate a primeira localidade
        temp = dijkstra(G, ordem, "Minha Casa", lugares[rotas[r][0] - 'A']);
        if(temp<0){
            temp = -temp;
        }

        distancia += temp;
        //Itera pela string da rota atual, somando os valores das distancias entre cada local
        for(int j = 0; j < nLugares - 1; j++){
            distancia += distancias[rotas[r][j] - 'A'][rotas[r][j+1] - 'A'];
        }
        //calcula a distancia da ultima localidade ate casa
        temp = dijkstra(G, ordem, lugares[rotas[r][nLugares - 1] - 'A'], "Minha Casa");

        if(temp<0){
            temp = -temp;
        }

        distancia += temp;
        //Sempre armazena a melhor rota ate o momento
        if(distancia < distanciaMin){
            distanciaMin = distancia;
            strcpy(melhorRota, rotas[r]);
        }
    }

    printf("Ordem dos locais visitados\n");
    printf("Localidade : Casa\n");
    for(int i = 0;i<nLugares;i++){
        printf("Localidade : %s\n",lugares[melhorRota[i]-'A']);
    }
    printf("Localidade : Casa\n");
    printf("Distancia Total  = %dm\n", distanciaMin);

    printf("\nRota completa:\n");


    //casa ate 1 ponto
    temp = dijkstra(G,ordem,lugares[melhorRota[0]-'A'],"Minha Casa");
    printf("Trajeto: Casa ate %s\n",lugares[melhorRota[0]-'A'] );
    Aresta *aux;
    for(int i = 0; i < ordem; i++){
		aux = G[i].prim;
		for(; aux != NULL; aux = aux->prox){
			if (strcmp(aux->localidade.nome,"Minha Casa" )==0){
                verticeDestino1= i;
                verticeDestino2= aux->extremo2;

			}
		}
	}
	if(temp <0){
    temp = - temp;
	imprimeCaminho(G,verticeDestino1);
	}else{
	imprimeCaminho(G,verticeDestino2);
	}
    printf("Distancia do trajeto: %dm\n\n",temp);


    for(int j = 0;j<nLugares-1;j++){
        temp = dijkstra(G,ordem,lugares[melhorRota[j+1]-'A'],lugares[melhorRota[j]-'A']);
        printf("Trajeto: %s ate %s\n",lugares[melhorRota[j]-'A'],lugares[melhorRota[j+1]-'A']);
        for(int i = 0; i < ordem; i++){
            aux = G[i].prim;
            for(; aux != NULL; aux = aux->prox){
                if (strcmp(aux->localidade.nome,lugares[melhorRota[j]-'A'] )==0){
                verticeDestino1= i;
                verticeDestino2= aux->extremo2;
                }
            }
        }
        if(temp <0){
        temp = - temp;

        imprimeCaminho(G,verticeDestino1);
        }else{
        imprimeCaminho(G,verticeDestino2);
        }
        printf("distancia %dm\n\n",temp);
    }




    //ultimo ponto ate casa

    temp = dijkstra(G, ordem,"Minha Casa",lugares[melhorRota[nLugares-1]-'A']);
    printf("Trajeto: %s ate Minha casa\n",lugares[melhorRota[nLugares-1]-'A']);
    for(int i = 0; i < ordem; i++){
		aux = G[i].prim;
		for(; aux != NULL; aux = aux->prox){

			if (strcmp(aux->localidade.nome,lugares[melhorRota[nLugares-1]-'A'] )==0){
                verticeDestino1= i;
                verticeDestino2= aux->extremo2;

			}
		}
	}
	if(temp <0){
    temp = - temp;
	imprimeCaminho(G,verticeDestino1);
	}else{
	imprimeCaminho(G,verticeDestino2);
	}
    printf("Distancia do trajeto %dm\n",temp);


}


int main(){
	Vert *G = NULL;
	int ordem = 51;
	constroiGrafo(&G, &ordem);

	//vetor de localidades utilizadas no algoritmo. Altere as localidades para obter outras rotas
	char *locais[] = {"Pizza","Mambo","Shopping Patio Higienopolis"};
	int n = sizeof(locais) / sizeof(locais[0]);
	melhorRota(G, ordem, locais, n);
	//imprimeGrafo(G,ordem);
	destroiGrafo(&G, ordem);
	return 0;
}
