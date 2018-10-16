//============================================================================
// Name        : U2_P1.cpp
// Author      : Ana Victoria Cavazos Argot
// Practica    : U2_P1
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <omp.h>
#define SEED 45
#define N 30000
#define H 4

unsigned t0, t1;
int dimension=N;
int max_rutas=dimension+1;
int par=0,impar=0,aristas=0;
int **matriz=NULL;
bool completo=true,conexo=false;

void imprimir_matriz();
void rango_matriz(int inicio,int final);
void crear_fichero(char archivo[]);
void cargar_fichero(char archivo[]);
void propiedades();
void g_completo(int inicio,int final);
void g_euleriano(int inicio,int final);
void g_conexo();

//============================================================================
// 									M A I N
//============================================================================

int main() {
	t0=clock();
	// --------------------------------------
	//crear_fichero("Grafo.txt");
	cargar_fichero("Grafo.txt");
	propiedades();
	// --------------------------------------
	t1 = clock();
	double time = (double(t1-t0)/CLOCKS_PER_SEC);
	printf("Tiempo de ejecucion: %f \n",time);
	return 0;
}

void propiedades(){
	//printf("----------\n");
	//imprimir_matriz();
	printf("----------\n");
	//int hilos = omp_get_num_procs();
	//printf("%d \n",hilos);
	omp_set_num_threads(H);
	g_conexo();
	#pragma omp parallel shared(completo, par, impar)
	{
		int tid = omp_get_thread_num();
		int nth = omp_get_num_threads();
		int inicio = tid * N / nth;
		int fin = (tid+1) * N / nth;
		g_completo(inicio,fin);
	}
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

void g_conexo(){
	#pragma omp parallel for shared(aristas)
	for(int i=0;i<dimension;i++){ 
		#pragma omp parallel for reduction(+:aristas)
		for(int j=i;j<dimension;j++){
			if(matriz[i][j]==1) aristas++;
		}
	}
	if(aristas>=(N-1)) conexo=true;
	else conexo=false;
}

void g_completo(int inicio, int final){
	int aux=0;
	// Comenzamos a contar las rutas de todos los nodos.
	for(int i=inicio;i<final;i++){
		#pragma omp parallel for reduction(+:aux)
		for(int j=0;j<dimension;j++){
			// Si el nodo esta conectado a otro
			// sus valores seran igual a 1
			if(matriz[i][j]==1)
				aux++;
		}
		// Contamos los nodos con rutas par o impar
		if(aux%2==0)par++;
		else impar++;
		if(aux<dimension-1){
			//printf("Incompleto\n");
			completo=false;
		}
		aux=0;
	}
}

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
		//int rutas = max_rutas;
		// Generamos las rutas
		// Se generan rutas*dimensiones para aumentar
		// la posibilidad de rutas distintas
		for(int i=0;i<rutas*(dimension);i++){
			// Nodos aleatorios
			int x = rand()%dimension+1;
			int y = rand()%dimension+1;
			while(y==x)
				y = rand()%dimension+1;
			fprintf(fichero,"%d %d\n",x ,y);
			//printf("\n%d %d",x,y); 
		}
	}
	fclose(fichero);
}

void cargar_fichero(char archivo[]) {
	FILE *fichero;
	fichero=fopen(archivo,"r");
	if( fichero==NULL )
		printf("Error al abrir el fichero\n");
	else{
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
		//
		//imprimir_matriz();
	}
	fclose(fichero);
}
