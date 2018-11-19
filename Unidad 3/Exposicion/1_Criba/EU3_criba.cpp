//============================================================================
// Name        : EU3_criba.cpp
// Author      : Ana Victoria Cavazos Argot
// Exposicion  : EU3_criba
//============================================================================

#include <stdio.h> 
#include <stdlib.h>
#include <ctime>

#define N 10000//7925 //1000 numeros primos

// Variables globales:
unsigned t0, t1;

int main(int argc, char *argv[]){
   t0=clock();
   // ----------------------------------------------------------
   // Variables locales:
   int v[N],primos[N];
   int aux=0,c=0;
   // ----------------------------------------------------------
   //Inicializamos el arreglo binario
   for (int i = 0; i < N; ++i){
      v[i]=1;
      primos[i]=1;
   }
   // ----------------------------------------------------------
   // Comenzamos a obtener los numeros primos mediante la criba
   printf(" 1000 primeros numeros primos: \n");
   // Comenzamos desde 2 ya que ni 0 ni 1 es primo
   for(int i=2;i<N;i++){
      // Si el valor actual no fue "eliminado",
      // es decir, el arreglo en su posicion es igual a 1
      // Significa que es PRIMO
      if(v[i]==1){
         // Imprimimos el numero primo
         printf(" %d, ",i);
         // Aumentamos el contador de numeros primos
         c++;
         // Procedemos a eliminar los multiplos del numero primo
         for (int j=i;j<N;j++)
         {
            // Si es multiplo su valor en el arreglo se iguala a 0
            if(j%i==0)
               v[j]=0;
         }
      }
   }
   printf("\n\n Cantidad de primos obtenidos: %d\n",c);
   // ----------------------------------------------------------
   t1 = clock();
   double time = (double(t1-t0)/CLOCKS_PER_SEC);
   printf(" \n Tiempo de ejecucion: %f \n",time);
   return 0;
}