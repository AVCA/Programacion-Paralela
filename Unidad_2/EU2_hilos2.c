#include <omp.h>
#include <stdio.h> 
int main ()
{
int nthreads;
int thread;
int NuevoValor;
omp_set_num_threads(10);
#pragma omp parallel private(nthreads, thread) lastprivate(NuevoValor)//firstprivate(NuevoValor)
  {
  int NuevoValor = 222;
  thread = omp_get_thread_num();
  nthreads = omp_get_num_threads();
  NuevoValor+=1;
  printf("Hola Mundo soy la hebra = %d de %d que somos\n",thread,nthreads);
  printf("Esta es la variable nueva = %d \n", NuevoValor);
  }
  printf("Nuevo valor=%d\n",NuevoValor);
 }