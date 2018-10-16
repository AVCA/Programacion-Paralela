//============================================================================
// Name        : U1_P9.cpp
// Author      : Ana Victoria Cavazos Argot
// Practica    : U1_P9
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctime>
#include <iostream>
using namespace std;
#define SEED 45
#define N 10 // Precision
#define M 1000000 // Cantidad de valores a calcular
#define A 0.5 // constante x para ña serie geometrica

// Variables globales:
unsigned t0, t1;

// ----------------------------------------------------------
// Funcion exponencial:
long double factorial(int n){
	unsigned long long factorial = 1;
	for(int i = 1; i <=n; ++i)
    {
        factorial *= i;
    }
    return factorial;
}
long double f_exponencial(int x, int max){
	long double e=0;
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
	// ----------------------------------------------------------
	// Funcion exponencial:
	long double exponencial = f_exponencial(X,N);
	// ----------------------------------------------------------
	// Serie geometrica:
	long double s_geometrica = f_s_geometrica(X,A,N);
	// ----------------------------------------------------------
	// Funciones trigonometricas:
	long double sin = f_sin(X,N);
	// ----------------------------------------------------------
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