/*
Escribir un programa que efectúe el siguiente cálculo con matrices y vectores:
double A(NxN), B(N), C(N), D(N), X
C(N) = A(NxN) × B(N)
D(N) = A(NxN) × C(N)
X = C(N) . D(N)
El programa debe pedir al principio el tamaño de los vectores, N (máximo, N = 1000) y el número de
threads.
*/

//============================================================================
// Name        : matvec.cpp
// Author      : Ana Victoria Cavazos Argot
// Practica    : matvec
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctime>
#include <omp.h>
#define SEED 45
#define N 500
#define H 2

// Variables globales:
unsigned t0, t1;
double A[N][N];
double B[N];
double C[N];
double D[N];
double X;

void multiplicacion(){
	printf("--------------\n");
	#pragma opm parallel for
	for(int i=0;i<N;i++){
		int aux=0;
		#pragma opm parallel for reduction(+:aux)
		for(int j=0;j<N;j++){
			aux+= A[i][j]*B[j];
		}
		C[i]=aux;
	}
	#pragma opm parallel for
	for(int i=0;i<N;i++){
		int aux=0;
		#pragma opm parallel for reduction(+:aux)
		for(int j=0;j<N;j++){
			aux+= A[i][j]*C[j];
		}
		D[i]=aux;
	}
	#pragma opm parallel for reduction(+:X)
	for(int i=0;i<N;i++){
		X+=C[i]*D[i];
	}
}

void imprimir(){
	printf(" A: \n");
	for (int i=0; i < N; i++){
		printf(" | ");
		for (int j = 0; j < N; j++)
		{
	    	printf("%.0f ",A[i][j]);
		}
		printf("|\n");
	}
	printf ("\n");
	// ------------------------------------
	// Inicializamos el vector
	printf(" B: \n");
	printf(" [ ");
	for (int i=0; i < N; i++){
	    printf("%.0f ",B[i]);
	}
	printf("]\n");
	printf ("\n");
	// ------------------------------------
	// Inicializamos el vector
	printf(" C: \n");
	printf(" [ ");
	for (int i=0; i < N; i++){
	    printf("%.0f ",C[i]);
	}
	printf("]\n");
	printf ("\n");
	// ------------------------------------
	// Inicializamos el vector
	printf(" D: \n");
	printf(" [ ");
	for (int i=0; i < N; i++){
	    printf("%.0f ",D[i]);
	}
	printf("]\n");
	printf ("\n");
	// ------------------------------------
	// Inicializamos el vector
	printf(" X: %.0f\n",X);
	printf ("\n");
}

int main() {
	t0=clock();
	// ------------------------------------
	// Generamos los numeros
	srand(SEED);
	// ------------------------------------
	// Inicializamos la matriz y vectores
	X= 0;
	for (int i=0; i < N; i++){
		B[i]= rand()%10+1;
		C[i]= 0;
		D[i]= 0;
		for (int j = 0; j < N; j++)
		{
			A[i][j] = rand()%10+1;
		}
	}
	// ----------------------------------------------------------
	// Paralelismo:
	// Asignacion de hilos:
	omp_set_num_threads(H);
	imprimir();
	multiplicacion();
	imprimir();
	// ----------------------------------------------------------
	t1 = clock();
	double time = (double(t1-t0)/CLOCKS_PER_SEC);
	printf(" \nTiempo de ejecucion: %f \n",time);
	return 0;
}