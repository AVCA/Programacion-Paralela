//============================================================================
// Name        : U2_P4.cpp
// Author      : Ana Victoria Cavazos Argot
// Practica    : U2_P4
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <ctime>
#include <omp.h>
#define SEED 20
#define H 16
#define N 500000

using namespace std;
unsigned t0, t1;
long long aux=0;
int iteraciones=0;
long long p=0;
long long suma=0,valor2=0;

long long revertir(long long valor){
	long long reversed=0;
	while(valor > 0)
    {
        reversed = reversed*10 + valor%10;
        valor /= 10;
    }
	return reversed;
}

long long palindromo(long long valor){
	long long numeros[11],i=0;
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

void Reverse_and_Add(long long valor){
	//#pragma omp task shared(valor) if(valor!=196)
	if(valor!=196)
	{
		if(p==0 && iteraciones<1000 && valor<4294967295){
			valor2 = revertir(valor);
			suma = valor + valor2;
			p = palindromo(suma);
			valor = suma;
			iteraciones++;
			//printf("[%d]\n",suma);
			Reverse_and_Add(valor);
		}
	}
}

void calculo(long long valor){
	//----------------------
	aux=0;
	iteraciones=0;
	p=0;
	suma=0;
	valor2=0;
	// ----------------------
	printf("%d |",valor);
	// ----------------------------------------------------------
	Reverse_and_Add(valor);
	printf(" %d",suma);
	printf(" %d\n",iteraciones);
}

//============================================================================
// 									M A I N
//============================================================================

int main() {
	t0=clock();
	srand(SEED);
	long long valor;
	#pragma omp parallel for
	for (int i = 0; i < N; ++i)
	{
		valor = rand();
		calculo(valor);
	}
	t1 = clock();
	double time = (double(t1-t0)/CLOCKS_PER_SEC);
	printf("Tiempo de ejecucion: %f \n",time);
	return 0;
}
