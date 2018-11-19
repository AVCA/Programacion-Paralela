//============================================================================
// Name        : EU3_criba.cpp
// Author      : Ana Victoria Cavazos Argot
// Exposicion  : EU3_criba
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <array>
#include <ctime>

#define N 1000 //numeros primos
#define B 4 //BUCKET

// Variables globales:
using namespace std;
unsigned t0, t1;

// Metodos a utilizar;
void ordenamiento(int numeros[]);
void imprimir_arreglo(int v[]);
void imprimir_matriz(int m[B][N]);
void sort_bucket(int v[],int m[B][N]);
void concatenar(int v[],int m[B][N]);

int main(int argc, char *argv[])
{
   t0 = clock();
   // ----------------------------------------------------------
   // Variables locales:
   int v[N];
   int bucket[B][N];
   // ----------------------------------------------------------
   // Inicializamos el arreglo con los valores
   for (int i = 0; i < N; ++i){
      v[i] = rand() % N + 1;
   }
   imprimir_arreglo(v);
   // Inicializamos las buckets
   for (int i = 0; i < B; ++i){
      for (int j = 0; j < N; ++j)
      {
         bucket[i][j] = 0;
      }
   }
   // ----------------------------------------------------------
   sort_bucket(v,bucket);
   concatenar(v,bucket);
   imprimir_arreglo(v);
   // ----------------------------------------------------------
   t1 = clock();
   double time = (double(t1 - t0) / CLOCKS_PER_SEC);
   printf(" \n Tiempo de ejecucion: %f \n", time);
   return 0;
}

void concatenar(int v[], int bucket[B][N]){
   // Concatenamos las cubetas
   int k = 0;
   for(int i = 0; i < B; i++){
        for(int j = 0; j < N; j++){
            if(bucket[i][j] != 0){
                v[k] = bucket[i][j];
                k++;
            }
        }
   }
}

void sort_bucket(int v[], int bucket[B][N]){
   int inc = N / B;
   int min = 0, max = inc;
   // Comenzamos con el ordenamiento BUCKET
   // 1) Llenamos cada cubeta:
   for(int i = 0; i < B; i++){
      // 2) Recorremos todos los numeros para llenar la cubeta actual
      //printf("Bucket %d:\n",i);
      int k = 0;
      for(int j = 0; j < N; j++){
         // 3) Si el valor se encuentra en el
         // rango actual de la cubeta
         // min < v[j] < max
         // printf("%d < %d < %d :",min,v[j],max);
         if(v[j] >= min && v[j] <= max){
            bucket[i][k] = v[j];
            k++;
         }
      }
      //
      min = max + 1;
      max = min + inc;
      // Ordenamos la cubeta
      ordenamiento(bucket[i]);
   }
   imprimir_matriz(bucket);
}

void imprimir_arreglo(int v[]){
   printf("\n[ ");
   for (int i = 0; i < N; ++i){
      printf("%d ", v[i]);
   }
   printf("] \n");
}

void imprimir_matriz(int m[B][N]){
   printf("\nBuckets: \n");
   for (int i = 0; i < B; i++)
   {
      printf("[%d] -> [ ", i);
      for (int j = 0; j < N; j++)
      {
         if(m[i][j] != 0)
         printf("%d ", m[i][j]);
      }
      printf("] \n");
   }
}

void ordenamiento(int numeros[]){
    int aux = 0;
    for (int i = 0; i < N; i++)
    {
        for (int j = i + 1; j < N; j++)
        {
            if (numeros[j] < numeros[i])
            {
                aux = numeros[i];
                numeros[i] = numeros[j];
                numeros[j] = aux;
            }
        }
    }
}