//============================================================================
// Name        : openmp.cpp
// Author      : Ana Victoria Cavazos Argot
// Practica    : Resumen OPENMP
//============================================================================

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
// Variables constantes:
#define N 3 // Cantidad de datos por hilo
#define H 3 // Cantidad de hilos solicitados
#define CHUNKSIZE 10 //Tamaño de particion

// Variables globales:
int nthreads;
int procesadores;
int thread;
int chunk;
int x=0,y=0,z=0;
// Varibles OMP:
// int nthreads = Cantidad de hilos disponibles
// int thread = Hilo actual dentro del paralelismo

//============================================================================
// 									M A I N
//============================================================================
int main (){
	/*
		// 1) Definir la cantidad de hilos a utilizar:
		// omp_set_num_threads(H);
		// ---------------------------------------------------------------------
		// 2) Declaramos #pragma que contendra todas las operaciones
		// que deseamos paralelizar:
		// #pragma omp parallel CONDICIONES ( Variables ):
		// ---------------------------------------------------------------------
		// CONDICIONES:
		// default() =
		// private () = Toma las variables y sin importar que tengan
		//				un valor inicial, estas se "vacian". Cada hilo
		//				tendra un valor distinto.
		// firstprivate() = Conserva el valor inicial de la variable
		// lastprivate() = No puede ser utilizada sin un for
		// shared() = Comparte variables entre procesos.
		// ---------------------------------------------------------------------
		// FUNCIONES:
		// omp_get_thread_num() = Hilo actual dentro del proceso
		// omp_get_num_threads() = Cantidad de hilos disponibles
		// omp_set_num_threads() = Asignar la cantidad de Hilos que requiero
		// omp_get_num_procs() = Cantidad de procesadores
	*/
	// ----------------------------------
	// Variables locales:
	float a[N], b[N], c[N];
	procesadores = omp_get_num_procs();
	omp_set_num_threads(procesadores);
	nthreads = omp_get_num_threads();
	chunk = CHUNKSIZE;
	/* Se inicializan ambos arreglos*/
	for(i=0; i < N; i++)
		a[i] = b[i] = 1.0;
	// ----------------------------------
	printf("==================================\n");
	printf("Cantidad de procesadores = %d\n", procesadores);
	printf("Cantidad de hilos = %d\n", nthreads);
	printf("==================================\n");
	printf("Variable x = %d\n",x);
	printf("Variable y = %d\n",y);
	printf("Variable z = %d\n",z);
	printf("==================================\n");



	#pragma omp parallel shared(nthreads,chunk){
		// ------------------------------
 		// Proceso paralelizado:
 		x = thread = omp_get_thread_num(); 
 		printf("Hilo actual = %d\n", thread);

 		// ------------------------------
 		// EXCLUSION:
 		// ------------------------------
		// Operaciones secuenciales dentro
		// de la paralelizacion:
		#pragma omp critical
			y=y+1;
		// ------------------------------
		// No permite que multiples hilos 
		// modifiquen la variable al mismo tiempo
		#pragma omp atomic
			z=z+2; 
		// ------------------------------
		/*
			// Proceso a paralelizar dentro de un FOR:
			// #pragma omp parallel for CONDICION1 ( Variables )
			// ---------------------------------------------------------------------
			// CONDICION1:
			// firstprivate() = Conserva el valor inicial de la variable
			// lastprivate() = Conserva el valor final que obtiene la variable
			// 				   dentro del proceso paralelizado.
			// reduction(operador : variable) = Reune los resultados obtenidos 
			// 									durante el proceso y los acumula segun
			//									el operador establecido.
			// if(condicion) =
			// ---------------------------------------------------------------------
		*/
		#pragma omp parallel for schedule(dynamic,chunk)//private(x) reduction(+:z) if(z>4)
		for(int i= 0 ; i< H; i++ ) {
			x = i;
			z= x*x;
			printf("z = %d\n",z);
		}
	}
	printf("==================================\n");
    printf("Variable x = %d\n",x);
    printf("Variable y = %d\n",y);
    printf("Variable z = %d\n",z);
	return 0;
}
