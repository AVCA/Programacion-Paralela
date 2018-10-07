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

// Variables globales:
unsigned t0, t1;
int m=1;

void conjetura_collatz(int n){
	printf("n: %d\n",n);
	int c=1;
	printf("secuencia: %d ",n);
	while(n!=1){
		if(n%2==0)// Par
			n=n/2;
		else // Impar
			n=(3*n)+1;
			printf("%d ",n);
			c++;
	}
	printf("\n");
	printf("numeros generados: %d \n",c );
	printf("-------------------------\n");
}

int main() {
	//t0=clock();
	// ------------------------------------
	// Generamos los numeros
	cin >> m;
	while(m != 0){
		t0=clock();
		srand(SEED);
		for (int i = 0; i < m; ++i)
		{
			conjetura_collatz(rand()%m+1);
		}
		t1 = clock();
		double time = (double(t1-t0)/CLOCKS_PER_SEC);
		printf("Tiempo de ejecucion: %f \n",time);
		cin >> m;
	}
	// ------------------------------------
	//t1 = clock();
	//double time = (double(t1-t0)/CLOCKS_PER_SEC);
	//printf("Tiempo de ejecucion: %f \n",time);
	return 0;
}




