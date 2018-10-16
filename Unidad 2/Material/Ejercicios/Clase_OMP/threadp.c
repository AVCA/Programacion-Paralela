/* threadp.c OMP
Ejemplo de uso de variables threadprivate
RP1: copyin(x) --> toma el valor inicial del master: y/z sin inicializar (0)
RP2: x/y mantienen el valor de la region paralela anterior (threadprivate)
RP3: una rutina: x/z mantienen el valor, pero z no es privada (var. global original)
     tid si es privada porque se pasa como parámetro.  
*/
#include <stdio.h>
#ifdef _OPENMP
 #include <omp.h>
 #define TRUE 1
 #define FALSE 0
#else
 #define omp_get_thread_num() 0
 #define omp_get_num_threads() 1
#endif

int x=0,y=0,z=0,i=0,tid=0;
#pragma omp threadprivate(x,y)

void imprimir(int tid){
	printf("Thread %d: x=%d  y=%d  z=%d \n",tid,x,y,z );
}

main(){
	x=y=z=5;
	printf("\n\n Valores antes de la region paralela: x=%d  y=%d  z=%d \n",x,y,z );

	printf("\n Region paralela 1: x/y threadprivate; x copyin; z/tid private \n\n");

	#pragma omp parallel private (z,tid) copyin(x)
	{
	tid=omp_get_thread_num();
	printf("	A: Thread %d: x=%d  y=%d  z=%d \n",tid,x,y,z );

	x=x+tid+1;
	y=tid;
	z=tid+20;
	#pragma omp barrier
	printf("	B: Thread %d: x=%d  y=%d  z=%d \n",tid,x,y,z );
	}

	printf("\n >>>>>>>>>>>>>> Ejecucion de un trozo en serie >>>>>>>>>>>>>>>>\n");

	printf("\n Region paralela 2: x/y threadprivate; z/tid private \n\n");
	#pragma omp parallel private (z,tid) 
	{
	tid=omp_get_thread_num();
	printf("	Thread %d: x=%d  y=%d  z=%d \n",tid,x,y,z );
	}

	printf("\n >>>>>>>>>>>>>> Ejecucion de un trozo en serie >>>>>>>>>>>>>>>>\n");

	printf("\n Region paralela 3: se imprime desde una rutina: tid como parametro \n\n");
	#pragma omp parallel private (z,tid) 
	{
	tid=omp_get_thread_num();
	imprimir(tid);
	}

	printf("\n Main: se imprime desde una rutina: -1 como parametro \n\n");
	imprimir(-1);
	printf("\n Fin del main: x=%d  y=%d  z=%d \n\n",x,y,z);
}
