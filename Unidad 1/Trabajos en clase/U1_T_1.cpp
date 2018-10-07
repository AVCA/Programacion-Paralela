//============================================================================
// Name        : U1_T_2.cpp
// Author      : Ana Victoria Cavazos Argot
// Practica    : U1_T_2
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
//----
//#define SEED 45;
// 
#include <omp.h>

//===========================================================================
// 									M A I N
//============================================================================

int main() {
	int N=100; // Numeros a generar
	double x,y; // Coordenadas
	int count=0; // Conteo de numeros dentro del circulo
	double z;
	double pi;
	int numthreads=2,tid;

	#pragma omp parallel //firstprivate(x,y,z,i) reduction(+:count) numthreads
	{
		srand(45);
		for(int i=0;i<N;i++){
			x = ((double)rand())/RAND_MAX;
			y = ((double)rand())/RAND_MAX;
			z=x*x+y*y;
			if(z<=1){count++;
			}
		}
	}
	pi = ((double)count/(double)(N*numthreads))*4.0;
	printf("Pi: %f\n",pi);
	return 0;
}
