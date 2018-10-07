#include <omp.h>
#include <stdio.h> 
int main ()
{
int nthreads;
int thread;
int NuevoValor = 222;
omp_set_num_threads(10);
#pragma omp parallel private(nthreads, thread) firstprivate(NuevoValor)
  {
  thread = omp_get_thread_num();
  nthreads = omp_get_num_threads();
  printf("Hola Mundo soy la hebra = %d de %d que somos",threads,nthread);
  printf("Esta es la variable nueva = %d", NuevoValor);
  }
 }