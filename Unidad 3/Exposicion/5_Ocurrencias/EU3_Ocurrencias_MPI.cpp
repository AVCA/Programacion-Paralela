//============================================================================
// Name        : EU3_Ocurrencias_MPI.cpp
// Author      : Ana Victoria Cavazos Argot
// Exposicion  : EU3_Ocurrencias_MPI
//============================================================================

//------------------------------------------------
// Libreria:
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <math.h>
#include "mpi.h"

//------------------------------------------------
// Variables globales:
#define DIM 3
#define x 8
#define MASTER 0

using namespace std;
unsigned t0, t1;

int rank, size,count=0;
MPI_Status estado;
double A[DIM][DIM];

// Metodos a utilizar:
void leer();
void imprimir();

void master(int rank, int size, MPI_Status estado);
void worker(int rank, int size, MPI_Status estado);

int main(int argc, char *argv[]){
  t0 = clock();
  // ----------------------------------------------------------
  // Paralelizacion: MPI
  MPI_Init(&argc, &argv); // Inicializamos los procesos
  MPI_Comm_size(MPI_COMM_WORLD, &size); // Obtenemos el numero total de procesos
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtenemos el valor de nuestro identificador
  // ----------------------------------------------------------
  if(rank == MASTER){
    master(rank, size, estado);
    
  }
  if(rank != MASTER){
    worker(rank, size, estado);
  } 
  // ----------------------------------------------------------
  if(rank == MASTER){
    t1 = clock();
    double time = (double(t1 - t0) / CLOCKS_PER_SEC);
    printf(" \n Tiempo de ejecucion: %f \n", time);
  }
  return 0;
}
void leer(){
  for (int i = 0; i < DIM; ++i)
  {
    for (int j = 0; j < DIM; ++j)
    {
      A[i][j]=rand()%9+1;
    }
  }
}
void imprimir(){
  if(DIM<10){
    for (int i = 0; i < DIM; ++i)
    {
      for (int j = 0; j < DIM; ++j)
      {
          printf(" %.0f |",A[i][j]);
      }
      printf("\n");
    }
  }
}
void master(int rank, int size, MPI_Status estado){
  // 1. Inicializamos la matriz y el conteo
  leer();
  imprimir();
  count=0;
  // 2. Repartimos la matriz entre los trabajadores
  float cr = (float) DIM / (size - 1);
  int c = ceil(cr);
  int f=0,aux=0;
  int filas[c];
  for (int i = 1; i < size; ++i){
    for (int j = 0; j < c; ++j){
      //printf("i = %d | f = %d \n",i,f);
      filas[j] = f;
      f++;
    }
    // 3 Enviamos los arreglos necesarios para el calculo
    MPI_Send(&A[0][0],DIM*DIM, MPI_DOUBLE,i,0,MPI_COMM_WORLD);
    MPI_Send(&c, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
    MPI_Send(&filas, c, MPI_INT, i, 0, MPI_COMM_WORLD);
    MPI_Recv(&aux, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &estado);
    count+=aux;
  }
  printf(" %d ocurrencias \n",count);
}
void worker(int rank, int size, MPI_Status estado){
  int c=0,count=0;
  MPI_Recv(&A[0][0],DIM*DIM, MPI_DOUBLE, MASTER, 0, MPI_COMM_WORLD, &estado);
  MPI_Recv(&c, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD, &estado);
  int filas[c];
  MPI_Recv(&filas, c, MPI_INT, MASTER, 0, MPI_COMM_WORLD, &estado);
  // --------------------------------------
  for (int i = 0; i < c; ++i){
    int k = filas[i];
    //printf("rank = %d | f = %d \n",rank,k);
    for (int j = 0; j < DIM; ++j){
      if(A[k][j]==x){
        count++;
      }
    }
  }
  //printf("%d \n",count);
  //
  MPI_Send(&count, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD);
}