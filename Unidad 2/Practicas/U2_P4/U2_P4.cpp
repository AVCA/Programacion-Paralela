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
#define H 4

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

int main() {
	
	int N=0,valor=0;
	cin >> N;
	for (int i = 0; i < N; ++i)
	{
		t0=clock();
		cin >> valor;
		Reverse_and_Add(valor);
		t1 = clock();
		double time = (double(t1-t0)/CLOCKS_PER_SEC);
		printf("Tiempo de ejecucion: %f \n",time);
	}
	
	return 0;
}
