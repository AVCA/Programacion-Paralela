#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define CHUNKSIZE   10 //Tamaño de particion
#define N       100//Tamaño de los arreglos

int main (int argc, char *argv[]) 
{
int nthreads, tid, i, chunk;
float a[N], b[N], c[N];

/* Se inicializan ambos arreglos*/
for(i=0; i < N; i++)
	a[i] = b[i] = 1.0;

chunk = CHUNKSIZE;

#pragma omp parallel shared(a,b,c,nthreads,chunk) private(i,tid)
  {
  tid = omp_get_thread_num();//Solcita el numero del hilo
  if (tid == 0)
    {
    nthreads = omp_get_num_threads();//Solicita la cantidad de hilos
    printf("Number of threads = %d\n", nthreads);
    }
  printf("Thread %d starting...\n",tid);

  #pragma omp for schedule(dynamic,chunk)
  for (i=0; i<N; i++)
    {
    c[i] = a[i] + b[i];
    printf("Thread %d: c[%d]= %f\n",tid,i,c[i]);
    }

  }  /* end of parallel section */

}

