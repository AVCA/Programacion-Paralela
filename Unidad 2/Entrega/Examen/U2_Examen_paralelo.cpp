//============================================================================
// Name        : Examen_U2.cpp
// Author      : Ana Victoria Cavazos Argot
// Examen      : Examen_U2
//============================================================================
/*********************************************************
fibo.c
Ejemplo de uso de la directiva task en un programa
recursivo para calcular el numero de fibonacci
(version paralela con sleep)
**********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <omp.h>
#define H 2

// Variables globales:
unsigned t0, t1;

long fibonacci(int n){
	long fn1, fn2, fn;
	if ( n == 0 || n == 1 ) return(1);
	// Cada TASK sera tomada por un hilo 
	// e independientemente realizaran
	// el calculo recursivo
	#pragma omp task shared(fn1)
	{
		//int hilo = omp_get_thread_num();
		//printf("Task 1: %d\n",hilo);
		fn1 = fibonacci(n-1);
	}
	#pragma omp task shared(fn2)
	{
		//int hilo = omp_get_thread_num();
		//printf("Task 2: %d\n",hilo);
		fn2 = fibonacci(n-2);
	}
	// Una vez que los hilos lleguen aqui
	// estos deberan esperar a que los demas
	// terminen para continuar
	#pragma omp taskwait
		fn = fn1 + fn2;
	return(fn);
}

int main(){
	// ----------------------------------------------------------
	// Variables locales:
	int n;
	long fibo;
	// ----------------------------------------------------------
	printf("Numero a calcular: ");
	scanf("%d", &n);
	t0=clock();
	// ----------------------------------------------------------
	// Paralelismo:
	// Asignacion de hilos:
	omp_set_num_threads(H);
	//Paralelismo de funcion:
	#pragma omp parallel shared(fibo)
	{
		// Esto permite que el calculo solo sea
		// llamado una sola vez
		#pragma omp single
		{
			// Comienza calculo recursivo
			fibo = fibonacci(n);
		}
	}
	// Imprimimos el resultado
	printf("Fibonacci = %ld\n",fibo);
	// ----------------------------------------------------------
	t1 = clock();
	double time = (double(t1-t0)/CLOCKS_PER_SEC);
	printf(" \nTiempo de ejecucion: %f \n",time);
	return 0;
}