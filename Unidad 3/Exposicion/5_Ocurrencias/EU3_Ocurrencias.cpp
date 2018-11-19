//============================================================================
// Name        : EU3_Ocurrencias.cpp
// Author      : Ana Victoria Cavazos Argot
// Exposicion  : EU3_Ocurrencias
//============================================================================

//------------------------------------------------
// Libreria:
#include <stdio.h>
#include <stdlib.h>
#include <ctime>

//------------------------------------------------
// Variables globales:
#define DIM 3
#define x 8

using namespace std;
unsigned t0, t1;

// Metodos a utilizar:
void leer(double A[DIM][DIM]);

int main(int argc, char *argv[]){
  t0 = clock();
  // ----------------------------------------------------------
  double A[DIM][DIM];
  int i,j,count;

  leer(A);
  count=0;

  for (int i = 0; i < DIM; ++i)
  {
    for (int j = 0; j < DIM; ++j)
    {
      if(A[i][j]==x){
        count++;
      }
    }
  }
  printf(" %d ocurrencias de %d\n",count,x);
  // ----------------------------------------------------------
  t1 = clock();
  double time = (double(t1 - t0) / CLOCKS_PER_SEC);
  printf(" \n Tiempo de ejecucion: %f \n", time);
  return 0;
}

void leer(double A[DIM][DIM]){
  for (int i = 0; i < DIM; ++i)
  {
    for (int j = 0; j < DIM; ++j)
    {
      A[i][j]=rand()%9+1;
      if(DIM<10)
        printf(" %.0f |",A[i][j]);
    }
    if (DIM<10)
    {
      printf("\n");
    }
  }
}