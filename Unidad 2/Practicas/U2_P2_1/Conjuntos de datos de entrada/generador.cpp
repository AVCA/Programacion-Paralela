//============================================================================
// Name        : U2_P1.cpp
// Author      : Ana Victoria Cavazos Argot
// Practica    : U2_P1
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <omp.h>
#include <string> 
//#define SEED 90
//#define H 4

int SEED;
int dimension;
int max_rutas=dimension+1;
int **matriz=NULL;

void crear_fichero(char archivo[]);

int main() {
	scanf( "%d %d", &SEED,&dimension);
	crear_fichero("Grafo.txt");
	return 0;
}

void crear_fichero(char archivo[]){
	FILE *fichero;
	fichero=fopen(archivo,"w");
	if( fichero==NULL )
		printf("Error al abrir el fichero\n");
	else
	{
		fprintf(fichero,"%d\n",dimension);
		srand(SEED);
		int rutas = rand()%max_rutas+1;
		for(int i=0;i<rutas*(dimension);i++){
			int x = rand()%dimension+1;
			int y = rand()%dimension+1;
			while(y==x)
				y = rand()%dimension+1;
			fprintf(fichero,"%d %d\n",x ,y);
		}
	}
	fclose(fichero);
}
