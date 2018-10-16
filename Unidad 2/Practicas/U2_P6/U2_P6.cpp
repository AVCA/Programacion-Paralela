//============================================================================
// Name        : main.cpp
// Author      : Ana Victoria Cavazos Argot
// Practica    : U1_P6_1
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <ctime>
using namespace std;
#define SEED 45
#define H 100
#define N 30000

// Variables globales:
unsigned t0, t1;
int m=1,n=0;
int c=1;

void conjetura_collatz(){
	#pragma omp task shared(n,c)
	{
		if(n!=1){
			if(n%2==0)// Par
				n=n/2;
			else // Impar
				n=(3*n)+1;
			c++;
			printf("%d ",n);
			conjetura_collatz();	
		}
	}
}

void f_3n1(){
	n = rand()%N+1;
	printf("n: %d\n",n);
	c=1;
	printf("secuencia: %d ",n);
	conjetura_collatz();
	printf("\n");
	printf("numeros generados: %d \n",c );
	printf("-------------------------\n");
}

int main() {
	//t0=clock();
	// ------------------------------------
	// Generamos los numeros
	t0=clock();
	srand(SEED);
	for (int i = 0; i < N; ++i)
	{
		f_3n1();
	}
	t1 = clock();
	double time = (double(t1-t0)/CLOCKS_PER_SEC);
	printf("Tiempo de ejecucion: %f \n",time);
	// ------------------------------------
	//t1 = clock();
	//double time = (double(t1-t0)/CLOCKS_PER_SEC);
	//printf("Tiempo de ejecucion: %f \n",time);
	return 0;
}




