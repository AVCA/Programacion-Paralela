//============================================================================
// Name        : U2_P9.cpp
// Author      : Ana Victoria Cavazos Argot
// Practica    : U2_P9
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctime>
#include <omp.h>
#include <iostream>
using namespace std;
#define SEED 45
#define H 10
#define N 30 // Precision
#define M 100000 // Cantidad de valores a calcular
#define A 0.5 // constante x para ña serie geometrica

// Variables globales:
unsigned t0, t1;

// ----------------------------------------------------------
// Funcion exponencial:
long double factorial(int n){
	unsigned long long factorial = 1;
	#pragma omp parallel for shared(factorial)
	for(int i = 1; i <=n; ++i)
    {
        factorial *= i;
    }
    return factorial;
}
long double f_exponencial(int x, int max){
	long double e=0;
	#pragma omp parallel for shared(e)
	for (int i = 0; i <= max; ++i)
	{
		double b = factorial(i);
		if(b==0)b=1;
		e+=(pow(x,i)/b);
	}
	return e;
}
// ----------------------------------------------------------
// Serie geometrica:
long double f_s_geometrica(int a,double x,int max){
	long double s=0;
	#pragma omp parallel for shared(s)
	for (int i = 0; i <= max; ++i)
	{
		s+=a*pow(x,i);
	}
	return s;
}
// ----------------------------------------------------------
// Funciones trigonometricas:
// Seno:
long double f_sin(int x, int max){
	long double sin=0;
	#pragma omp parallel shared(sin)
	for (int i = 0; i <= max; ++i)
	{
		int a = (2*i)+1;
		double b = factorial(a);
		if(b==0)b=1;
		double c = pow(-1,i);
		double d = (c/b);
		double e = pow(x,a);
		sin+=d*e;
	}
	return sin;
}
// ----------------------------------------------------------
// Funciones:
void funciones(int X){
	long double exponencial = 0,s_geometrica=0,sin=0;
	// ----------------------------------------------------------
	// Paralelismo:
	// Asignacion de hilos:
	omp_set_num_threads(H);
	// ----------------------------------------------------------
	//Paralelismo de funciones:
	// Funcion exponencial:
	//exponencial = f_exponencial(X,N);
	//s_geometrica = f_s_geometrica(X,A,N);
	//sin = f_sin(X,N);
	#pragma omp parallel //shared(exponencial,s_geometrica,sin)
	{
		#pragma omp single
		{
			#pragma omp task shared(exponencial)
			exponencial = f_exponencial(X,N);

			// Serie geometrica: 
			#pragma omp task  shared(s_geometrica)
			s_geometrica = f_s_geometrica(X,A,N);

			// Funciones trigonometricas:
			#pragma omp task  shared(sin)
			sin = f_sin(X,N);
		}
	}
	// -*---------------------------------------------------------
	printf(" X = %d\n",X);
	printf(" Funcion exponencial: ");
	cout << exponencial << endl;
	printf(" Serie geometrica: ");
	cout << s_geometrica << endl;
	printf(" Sin x: ");
	cout << sin << endl;
	// ----------------------------------------------------------
}
int main() {
	t0=clock();
	printf(" Calculo de valores aproximados: \n");
	for(int i=0;i<M;i++){
		int X = rand();
		funciones(X);
		printf("----------------------------------\n");
	}
	t1 = clock();
	double time = (double(t1-t0)/CLOCKS_PER_SEC);
	printf(" \nTiempo de ejecucion: %f \n",time);
	return 0;
}