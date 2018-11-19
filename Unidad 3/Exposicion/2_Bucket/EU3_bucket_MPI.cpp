//============================================================================
// Name        : EU3_criba.cpp
// Author      : Ana Victoria Cavazos Argot
// Exposicion  : EU3_criba
// Fuente: https://sites.google.com/a/g.clemson.edu/cpsc362-fa2013/home/bucketsort-v1
//============================================================================

//------------------------------------------------
// Libreria:
#include <stdio.h>
#include <stdlib.h>
#include <array>
#include <ctime>
#include "mpi.h"

//------------------------------------------------
// Variables globales:
#define N 1000 //1000 // numeros
using namespace std;
unsigned t0, t1;
int B = 0; // Variable auxiliar que almacenara el total del procesos
int inc = 0; // Incremento entre los rangos de las cubetas
int k=0; // Conteo de elementos por cubeta
int *v = NULL; // Vector que almacenara los numeros desordenados
int *r = NULL; // Vector que almacenara los numeros ordenados
int *bucket = NULL; // Vector que almacenara los valores correspondientes al rango de la cubeta
int *proc_count=NULL; // Vector que almacenara el conteo de los elementos por cubeta
int *disp=NULL; // Vector que almacena las posiciones para concatemar cada bucket ordenada en el arreglo final (r)

// Metodos a utilizar;
void ordenamiento(int numeros[],int n);
void imprimir_arreglo(int v[],int n, int rank, int min, int max);
void sort_bucket(int rank);

int main(int argc, char *argv[]){
  t0 = clock();
  // ----------------------------------------------------------
  // Inicializamos los arreglos v,r y bucket
  v = (int *)malloc(N * sizeof(int));
  r = (int *)malloc(N * sizeof(int));
  bucket = (int *)malloc(N * sizeof(int));
  for (int i = 0; i < N; ++i){
    v[i] = rand() % N + 1;
    bucket[i] = 0;
    r[i]=0;
  }
  // ----------------------------------------------------------
  // Paralelizacion: MPI
  int rank, size;
  MPI_Init(&argc, &argv); // Inicializamos los procesos
  MPI_Comm_size(MPI_COMM_WORLD, &size); // Obtenemos el numero total de procesos
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtenemos el valor de nuestro identificador
  // ----------------------------------------------------------
  // Variables locales:
  B = size; 
  inc = N / B;
  k=0;
  // ------------------------------------------------------
  if(rank == 0){
    printf(" =============================\n");
    printf(" Vector original:\n");
    imprimir_arreglo(v,N, rank, 0, 0);
    // Inicializamos los vectores necesarios 
    // para la concatenacion del vector ordenado
    proc_count = (int *)malloc(size * sizeof(int));
    disp = (int *)malloc(size * sizeof(int));
  }
  // Iniciamos el ordenamiento por cubetas: BUCKET SORT
  sort_bucket(rank);
  // Al vector proc_count le daremos el conteo de elementos en
  // la cubeta actual
  MPI_Gather(&k,1,MPI_INT,proc_count,1,MPI_INT,0,MPI_COMM_WORLD);
  // Para este punto el arreglo proc_count tiene todos los conteos
  // de todas las cubetas
  // Ya que tenemos los conteos en un arreglo, ahora necesitamos
  // las posiciones en donde insertaremos las cubetas
  // -------------------------------------
  // Ejemplo:
  // proc_count = [1,3,5,1] <- Conteo de elementos por cubeta
  // disp = [0,1,4,9] <- Posiciones desde donde se insertaran los elementos
  //  0   1 2 3   4 5 6 7 8   9
  // [2 | 3 4 4 | 6 6 7 7 8 | 10]
  if (rank == 0){
    // Disp[0] es 0 ya que la cubeta[0] siempre 
    // iniciara al comienzo del arreglo a ordenar
    disp[0] = 0;
    //printf("%d |\n",disp[0]);
    // Comenzamos a calcular las demas posiciones:
    for (int i = 0; i < size-1; i++){
      //printf("%d ,\n",proc_count[i]);
      // Se suma la posicion actual mas el conteo actual
      // Para asi obtener el inicio de la siguiente cubeta
      disp[i+1] = disp[i] + proc_count[i];
      //printf("%d |\n",disp[i+1]);
    }
  }
  // Ahora que tenemos las posiciones desde donde tenemos que concatenar
  // utilizaremos MPI_Gatherv para concatenar la cubeta actual en su
  // posicion correspondiente sin esperar a los demas procesos
  // MPI_Gatherv(array_local,tamaño,tipo,array_final,tamaños,posiciones,tipo,etiqueta o raiz, comunicador)
  MPI_Gatherv(bucket,k,MPI_INT,r,proc_count,disp,MPI_INT,0,MPI_COMM_WORLD);
  MPI_Finalize();
  if(rank == 0)
  {
    printf(" =============================\n");
    printf(" Vector ordenado: \n");
    imprimir_arreglo(r,N, rank, 0, 0);
    // ----------------------------------------------------------
    t1 = clock();
    double time = (double(t1 - t0) / CLOCKS_PER_SEC);
    printf(" \n Tiempo de ejecucion: %f \n", time);
    free(v);
    free(r);
    free(bucket);
    free(proc_count);
    free(disp);
  }
  return 0;
}

void sort_bucket(int rank){
  // Creamos un arreglo auxiliar que almacenara los
  // valores de la cubeta de manera temporal
  int *v_aux = (int *)malloc(N * sizeof(int));
  int B = rank;
  // Calculamos el rango de la cubeta
  int min = B + (inc * B), max = min + inc;
  // Buscamos los elementos dentro de la cubeta
  for(int j = 0; j < N; j++)
  {
    v_aux[j] = 0;
    if(v[j] >= min && v[j] <= max)
    {
      v_aux[k] = v[j];
      k++;
    }
  }
  // Ya que tenemos los elementos los vaciamos
  // en la cubeta con un tamaño mas exacto
  bucket = (int *)malloc(k * sizeof(int));
  for(int i=0;i<k;i++){
    bucket[i]=v_aux[i];
  }
  int aux = 0;
  // Ordenamos la cubeta con un metodo independiente
  ordenamiento(bucket,k);
  free(v_aux);
  //imprimir_arreglo(bucket,k, rank, min, max);
}

void imprimir_arreglo(int v[],int n, int rank, int min, int max){
    printf("\n[%d] [%d-%d]->[ ", rank, min, max);
    for (int i = 0; i < n; ++i)
    {
        printf("%d ", v[i]);
    }
    printf("] \n");
}

void ordenamiento(int numeros[],int n){
    int aux = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if(numeros[i] != -0 && numeros[j] != 0)
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
}