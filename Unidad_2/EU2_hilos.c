#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
 
int main (){
int nthreads;
int thread;
#pragma omp parallel private(nthreads, thread)
  {
  thread = omp_get_thread_num();
  nthreads = omp_get_num_threads();
  printf("Hola Mundo soy la hebra = %d, de %d que somos\n",nthreads, thread );
  }
 return 0;
}
