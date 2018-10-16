/**********************************************************************************
nested.c
Ejemplo de regiones paralelas anidadas
**********************************************************************************/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0

int main(){
	int tid=-1;
	omp_set_num_threads(4);
	omp_set_nested(TRUE);
	if(! omp_get_nested()){
		printf("Cuidado!!! No se ha activado el paralelismo anidado \n");
	}
	printf("Paralelismo anidado esta %s \n",omp_get_nested() ? "activado":"no activado" );

	#pragma omp parallel private(tid)
	{
		tid=omp_get_thread_num();
		printf("TID (%d) ejecuta la region externa (nivel: %d)\n",tid,omp_get_level() );

		#pragma omp parallel num_threads(3)
		{
			printf("  >>>TID (%d.%d):  El thread %d ejecuta la region interna (nivel:%d) \n",tid,omp_get_thread_num(),omp_get_thread_num(),omp_get_level() );
		}

	}
	return(0);
}