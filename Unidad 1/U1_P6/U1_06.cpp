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

unsigned t0, t1;

void conjetura_collatz(int i,int j){
	int max=0;
	for(int k=i;k<=j;k++){
		int c=1;
		int n=k;
		while(n!=1){
			if(n%2==0)// Par
				n=n/2;
			else
				n=(3*n)+1;
			c++;
		}
		if(c>max)max=c;
	}
	printf("%d %d %d\n",i,j,max);
}

int main() {
	//t0=clock();
	// ------------------------------------
	// Numero para iniciar la secuencia
	int i,j;
	cin >> i;
	cin >> j;
	// Generamos los numeros 
	conjetura_collatz(i,j);
	// ------------------------------------
	//t1 = clock();
	//double time = (double(t1-t0)/CLOCKS_PER_SEC);
	//printf("Tiempo de ejecucion: %f \n",time);
	return 0;
}




