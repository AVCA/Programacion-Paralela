/*********************************************************
fibo_ser.c (usleep)
Ejemplo de uso de la directiva task en un programa
recursivo para calcular el numero de fibonacci
f(n) = f(n-1) + f(n-2) con f(0)=1 y f(1)=1
version serie con usleep
**********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctime>

// Variables globales:
unsigned t0, t1;

// Calculo recursivo del fibonacci
long fibonacci(int n){
	long fn1, fn2, fn;
	if ( n == 0 || n == 1 ) return(1);
	fn1 = fibonacci(n-1);
	fn2 = fibonacci(n-2);
	fn = fn1 + fn2;
	return(fn);
}

int main() {
	// ----------------------------------------------------------
	// Variables locales:
	int n;
	long fibo;
	// ----------------------------------------------------------
	printf("Numero a calcular: ");
	scanf("%d", &n);
	t0=clock();
	fibo = fibonacci(n);
	printf("Fibonacci = %ld\n",fibo);
	// ----------------------------------------------------------
	t1 = clock();
	double time = (double(t1-t0)/CLOCKS_PER_SEC);
	printf(" \nTiempo de ejecucion: %f \n",time);
	return 0;
}