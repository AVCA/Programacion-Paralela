/*
tarea.c
Ejemplo para ver el efecto del reparto en el tiempo de ejecución
Ejecutar modificando el scheduling: static, static 4, dynamic
*/

#include <omp.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

struct timeval t0,t1;
double 	tej;
#define  N 4000

double calculo(int veces){
	usleep(veces);
	return(1);
}

int i,A[N],nth=-1;
double total;

main(){
	//inicializacion vector de tamaño de tareas
	for(i=0;i<N; i++){
		A[i]=1;
	}
	A[1]=100000;
	A[5]=100000;

	gettimeofday(&t0,0);
	total=0.0;

	#pragma omp parallel for reduction(+:total) schedule(dynamic)//(static,4) //(static) //(runtime) 
	for (i = 0; i < N; i++)
	{
		total+=calculo(A[i]);
	}
	gettimeofday(&t1,0);
	tej=(t1.tv_sec - t0.tv_sec) + (t1.tv_usec - t0.tv_usec) /1e6;

	#ifdef _OPENMP
		nth=omp_get_max_threads();
	#endif

	printf("\n\n Tej con %d hilos (-1=serie) = %1.3f s\n\n Total= %.2f \n\n",nth,tej,total );
}