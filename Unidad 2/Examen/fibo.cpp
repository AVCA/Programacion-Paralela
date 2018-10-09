/*********************************************************
fibo.c
Ejemplo de uso de la directiva task en un programa
recursivo para calcular el numero de fibonacci
(version paralela con sleep)
**********************************************************/
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <omp.h>

long fibonacci(int n){
	long fn1, fn2, fn;
	//usleep(1); // El proceso se detiene por 1 milisegundo
	if ( n == 0 || n == 1 ) return(1);
	// if ( n < 30 ) return(fibonacci(n-1) + fibonacci(n-2));
	
	#pragma omp task shared(fn1)
	{
		int hilo = omp_get_thread_num();
		printf("Task 1: %d\n",hilo);
		fn1 = fibonacci(n-1);
	}
	#pragma omp task shared(fn2)
	{
		int hilo = omp_get_thread_num();
		printf("Task 2: %d\n",hilo);
		fn2 = fibonacci(n-2);
	}
	#pragma omp taskwait
		fn = fn1 + fn2;
	return(fn);
}

int main (){
	// ----------------------------------------------------------
	// Variables locales:
	struct timeval t0, t1;
	double tej;
	int nthr=0;
	int n;
	long resul;
	// ----------------------------------------------------------
	printf("Secuencial\n");
	printf("\n Numero a calcular? ");
	scanf("%d", &n);
	gettimeofday(&t0, 0);
	#pragma omp parallel shared (resul)
	{
		#pragma omp single
		{
			int hilos = omp_get_max_threads();
			printf("hilos = %d\n",hilos);
			omp_set_num_threads(hilos);
			resul = fibonacci(n);
		}
	}
	gettimeofday(&t1, 0);
	// ----------------------------------------------------------
	// Tiempo de ejecucion:
	printf ("\nEl numero Fibonacci de %5d es %d", n, resul);
	tej = (t1.tv_sec - t0.tv_sec) + (t1.tv_usec - t0.tv_usec) / 1e6;
	printf("\n T. de ejec. = %1.3f ms \n\n", tej*1000);
	return 0;
}