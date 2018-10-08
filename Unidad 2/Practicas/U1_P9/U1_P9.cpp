//============================================================================
// Name        : U1_P9.cpp
// Author      : Ana Victoria Cavazos Argot
// Practica    : U1_P9_1
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctime>
#include <iostream>
using namespace std;
#define SEED 45
#define N 10

// Variables globales:
unsigned t0, t1;

// ----------------------------------------------------------
// Funcion exponencial:
float factorial(int n){
	unsigned long long factorial = 1;
	for(int i = 1; i <=n; ++i)
    {
        factorial *= i;
    }
    return factorial;
}
float f_exponencial(int x, int max){
	float e=0;
	for (int i = 0; i < max; ++i)
	{
		e+=(pow(x,i)/factorial(i));
	}
	return e;
}
// ----------------------------------------------------------
// Serie geometrica:
float f_s_geometrica(int a,float x,int max){
	float s=0;
	for (int i = 0; i < max; ++i)
	{
		s+=a*pow(x,i);
	}
	return s;
}
// ----------------------------------------------------------
// Funciones trigonometricas:
// Seno:
float f_sin(int x, int max){
	float sin=0;
	for (int i = 0; i < max; ++i)
	{
		int a = (2*i)+1;
		float b = factorial(a);
		float c = pow(-1,i);
		float d = (c/b);
		float e = pow(x,a);
		printf("%d %f %f %f %f\n",a,b,c,d,e);
		sin+=d*e;
	}
	return sin;
}

int main() {
	t0=clock();
	// ----------------------------------------------------------
	// Funcion exponencial:
	float exponencial = f_exponencial(1,N);
	// ----------------------------------------------------------
	// Serie geometrica:
	float s_geometrica = f_s_geometrica(1,0.5,N);
	// ----------------------------------------------------------
	// Funciones trigonometricas:
	float sin = f_sin(1,3);
	// ----------------------------------------------------------
	printf(" Calculo de valores aproximados: \n");
	printf(" -------\n");
	printf(" Funcion exponencial: %f\n",exponencial);
	printf(" -------\n");
	printf(" Serie geometrica: %f\n",s_geometrica);
	printf(" -------\n");
	printf(" Sin x: %f\n",sin);
	printf(" -------\n");
	printf("\n");
	// ----------------------------------------------------------
	t1 = clock();
	double time = (double(t1-t0)/CLOCKS_PER_SEC);
	printf(" \nTiempo de ejecucion: %f \n",time);
	return 0;
}