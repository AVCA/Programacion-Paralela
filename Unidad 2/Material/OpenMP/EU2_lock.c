#include <stdio.h>  
#include <omp.h>  
#define H 4

// Variables:  
omp_lock_t my_lock;  
  
int main() {  
   /*
      // --------------------------
      // EXCLUSION MUTUA: CERROJOS
      // --------------------------
      // Funciones:
      // void omp_init_lock( omp_lock_t * cerrojo );
      // void omp_destroy_lock( omp_lock_t * cerrojo );
      // void omp_set_lock( omp_lock_t * cerrojo );
      // void omp_unset_lock( omp_lock_t * cerrojo );
      // 
      // omp_test_lock( omp_lock_t * cerrojo );
      // omp_get_nested( void );
      // --------------------------
      // Cerrojos simples:
      // 1) Creamos el cerrojo, se requiere de
      // una direccion de memoria o apuntador
   */
   omp_init_lock(&my_lock);
   #pragma omp parallel num_threads(H)  
   {  
      int tid = omp_get_thread_num();  
      for (int i = 0; i < H; i++) {
         // 2) Se asigna el cerrojo creado
         omp_set_lock(&my_lock);
         /*
            // Apartir de aqui los demas hilos tendran que 
            // esperar a que este termine ya que no pueden
            // modificar las variables que este ocupe
         */
         printf_s("Thread %d - starting locked region\n", tid);  
         printf_s("Thread %d - ending locked region\n", tid); 
         // 3) Quitamos el cerrojo 
         omp_unset_lock(&my_lock); 
         printf("------------------------------------------\n");
      }  
   }  
   // 4) Eliminamos el cerrojo liberando memoria.
   omp_destroy_lock(&my_lock);  
}  