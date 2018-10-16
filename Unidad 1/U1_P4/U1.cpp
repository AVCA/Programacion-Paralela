//============================================================================
// Name        : main.cpp
// Author      : Ana Victoria Cavazos Argot
// Practica    : U1_P4_1
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <ctime>

#define SEED 20
#define H 4
#define N 5

using namespace std;
unsigned t0, t1;

int revertir(int valor){
	int reversed=0;
	while(valor > 0)
    {
        reversed = reversed*10 + valor%10;
        valor /= 10;
    }
	return reversed;
}

int palindromo(int valor){
	int numeros[11],i=0;
	while(valor>0){
		numeros[i]= (valor%10);
		valor/=10;
		i++;
	}
	for(int j=0;j<i;j++){
		if(numeros[j]!=numeros[--i])return 0;
	}
	return 1;
}

void Reverse_and_Add(int valor){
	int iteraciones=0;
	int p=0;
	int suma=0,valor2=0;
	if(valor!=196){
	while(p==0 && iteraciones<1000 && valor<4294967295){
		valor2 = revertir(valor);
		suma = valor + valor2;
		p = palindromo(suma);
		valor = suma;
		iteraciones++;
	}
	}
	printf("%d %d\n",iteraciones,valor);
}

//============================================================================
// 									M A I N
//============================================================================

int main() {
	t0=clock();
	int valor=0;
	// ----------------------------------------------------------
	srand(SEED);
	for (int i = 0; i < N; ++i)
	{
		valor = rand();
		Reverse_and_Add(valor);
	}
	t1 = clock();
	double time = (double(t1-t0)/CLOCKS_PER_SEC);
	printf("Tiempo de ejecucion: %f \n",time);
	return 0;
}
