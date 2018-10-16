//============================================================================
// Name        : main.cpp
// Author      : Ana Victoria Cavazos Argot
// Practica    : U1_P2_1
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <ctime>

#define SEED 45
#define N 30000


unsigned t0, t1;
int dimension=N;
int max_rutas=dimension+1;
int par=0,impar=0,aristas=0;
int **matriz=NULL;
bool completo=true,conexo=false;

void crear_fichero(char archivo[]){
	FILE *fichero;
	fichero=fopen(archivo,"w");
	if( fichero==NULL )
		printf("Error al abrir el fichero\n");
	else
	{
		//Escritura:
		//printf("\n%d",dimension);
		fprintf(fichero,"%d\n",dimension);
		srand(SEED);
		// Numero de rutas:
		int rutas = rand()%max_rutas+1;
		// Generamos las rutas
		// Se generan rutas*dimensiones para aumentar
		// la posibilidad de rutas distintas
		for(int i=0;i<rutas*(dimension);i++){
			// Nodos aleatorios
			int x = rand()%dimension+1;
			int y = rand()%dimension+1;
			while(y==x)
				y = rand()%dimension+1;
			//printf("\n%d %d",x,y); 
			fprintf(fichero,"%d %d\n",x ,y);
		}
	}
	fclose(fichero);
}

void cargar_fichero(char archivo[]) {
	FILE *fichero;
	fichero=fopen(archivo,"r");
	if( fichero==NULL )
		printf("Error al abrir el fichero\n");
	else
	{
		fscanf(fichero,"%d",&dimension);
		printf("%d\n",dimension);
		
		//Filas
		matriz = (int**)malloc(dimension*sizeof(int*));
		// Columnas
		for(int i=0;i<dimension;i++){
			matriz[i] = (int*)malloc(dimension*sizeof(int));
			// Inicializar la matriz en ceros
			for(int j=0;j<dimension;j++){
				matriz[i][j]=0;
			}
		}
		
		//
		int x,y;
		while( !feof(fichero) ){
			fscanf(fichero,"%d %d",&x,&y);
			//printf("%d %d\n",x,y);
			matriz[x-1][y-1]=1;
			matriz[y-1][x-1]=1;
		}
		/*
		for(int i=0;i<dimension;i++){
			for(int j=0;j<dimension;j++){
				printf("%d|",matriz[i][j]);
			}
			printf("%\n");
		}
		*/
	}
	fclose(fichero);
}

void g_completo(){
	// ------------------------------------------------
	// Variables:
	// int aux: Contador de rutas por nodo.
	// ------------------------------------------------
	// Se necesita que cada nodo posea n-1 rutas para
	// considerar el grafo completo.
	// ------------------------------------------------
	int aux=0;
	// Comenzamos a contar las rutas de todos los nodos.
	for(int i=0;i<dimension;i++){
		for(int j=0;j<dimension;j++){
			// Si el nodo esta conectado a otro
			// sus valores seran igual a 1
			if(matriz[i][j]==1)
				aux+=1;
		}
		// Si el total de rutas del nodo actual
		// es menor que el (total de nodos - 1)
		// significa que el grafo NO ES COMPLETO.
		if(aux<dimension-1){
			completo=false;
			break;
		}
		// Si el nodo actual tiene n-1 rutas 
		// se continua con el sigiente nodo, 
		// y se reinicia el contador.
		aux=0;
	}
}

void g_conexo(){
	for(int i=0;i<dimension;i++){
		for(int j=i;j<dimension;j++){
			if(matriz[i][j]==1) aristas++;
		}
	}
	if(aristas>=(N-1)) conexo=true;
	else conexo=false;
}

void g_euleriano(){
	// ------------------------------------------------
	// Variables:
	// int aux: Contador de rutas por nodo
	// ------------------------------------------------
	int aux=0;
	for(int i=0;i<dimension;i++){
		for(int j=0;j<dimension;j++){
			// Contamos las rutas de cada nodo
			if(matriz[i][j]==1){
				aux++;
			}
		}
		// Contamos los nodos con rutas par o impar
		if(aux%2==0)par++;
		else impar++;
		aux=0;
	}
	// ------------------------------------------------
	// Un grafo posee un ciclo Euleriano:
	// Todos sus nodos tienen rutas par
	// Todos sus nodos tienen rutas par a excepcion de 2
	// ------------------------------------------------
	//printf("Par: %d\n",par);
	//printf("Impar: %d\n",impar);
}

void imprimir_matriz(){
	for(int i=0;i<dimension;i++){
			for(int j=0;j<dimension;j++){
				printf("%d|",matriz[i][j]);
			}
			printf("%\n");
		}
}

void propiedades(){
	//printf("----------\n");
	//imprimir_matriz();
	printf("----------\n");
	g_completo();
	g_conexo();
	g_euleriano();
	printf("Propiedades del grafo: \n");
	printf("Par: %d\n",par);
	printf("Impar: %d\n",impar);
	if(completo)printf("Completo \n");
	else printf("Incompleto \n");
	if(impar>2) printf("No Euleriano\n" );
	else printf("Euleriano\n" );
	if(conexo) printf("Conexo\n" );
	else printf("No Conexo\n" );
	printf("Vertices: %d\n",N);
	printf("Aristas: %d\n",aristas);
	printf("----------\n");
	free (matriz);
}
//============================================================================
// 									M A I N
//============================================================================

int main() {
	t0=clock();
	cargar_fichero("Grafo.txt");
	propiedades();
	t1 = clock();
	double time = (double(t1-t0)/CLOCKS_PER_SEC);
	printf("Tiempo de ejecucion: %f \n",time);
	return 0;
}
