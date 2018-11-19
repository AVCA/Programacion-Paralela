//============================================================================
// Name        : EU2_lock.c
// Author      : Ana Victoria Cavazos Argot
// Exposicion  : EU2_lock
//============================================================================

#include <stdio.h>  
#include <omp.h>
#define H 2
#define N 2
  
omp_lock_t my_lock;  
  
int main() { 
   // Asignamos la cantidad de hilos
   // que requerimos
   omp_set_num_threads(H);
   // Inicializamos el cerrojo
   omp_init_lock(&my_lock);
   int a = 0; 
   // Comienza la paralelizacion
   #pragma omp parallel
   {  
      // Hilo actual
      int tid = omp_get_thread_num();  
      int i,j;
      for (i = 0; i < N; ++i) {  
         // Comienza el cerrojo
         omp_set_lock(&my_lock);
         // Apartir de aqui todo es secuencial
         printf_s("Hilo %d - starting locked region\n", tid);
         a++;
         printf_s("Hilo %d - ending locked region\n", tid);
         // Se cierra el cerrojo  
         omp_unset_lock(&my_lock);  
         printf("----------\n");
      }
      printf("Hilo %d | a = %d \n",tid,a);
   }  
   printf("a = %d \n",a);
   // Se libera la direccion de memoria que almacena el cerrojo
   omp_destroy_lock(&my_lock);  
}  