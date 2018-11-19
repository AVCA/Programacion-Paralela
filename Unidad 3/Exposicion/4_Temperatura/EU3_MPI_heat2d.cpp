//============================================================================
// Name        : EU3_MPI_heat2d.cpp
// Author      : Ana Victoria Cavazos Argot
// Exposicion  : EU3_MPI_heat2d
// Referencia: https://computing.llnl.gov/tutorials/parallel_comp/#ExamplesHeat
//============================================================================

//------------------------------------------------
// Libreria:
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>    
#include <ctime>
//------------------------------------------------
// Variables globales:
#define X           5
#define Y           5
#define PASOS       2
#define MASTER      0

unsigned t0, t1;

float matriz[2][X][Y];
int cantidad_trabajadores;
int rango;
int filas;
int inicio;
int extra; 
int izquierda,derecha;  
int start,end;
char initial_d[] = "initial.txt";
char final_d[] = "final.txt";

int rank, size;
MPI_Status estado;

// Esta estructura se requiere para el calculo de la temperatura
struct Parms { 
  float cx;
  float cy;
} parms = {0.1, 0.1};

//------------------------------------------------
// Metodos a utilizar:
void inicializar(int x,int y,float *m);
void imprimir(int x, int y, float *u1, char *fnam);
void calcular(int inicio, int final, int y, float *u1, float *u2);

void master(int rank, int size, MPI_Status estado);
void worker(int rank, int size, MPI_Status estado);

int main (int argc, char *argv[]){
  t0 = clock();
  // ----------------------------------------------------------
  // Paralelizacion: MPI
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  // ----------------------------------------------------------
  // Master:
  if (rank == MASTER) {
    master(rank, size, estado);
  } 
  //------------------------------------------------
  // Trabajador:
  if (rank != MASTER){
    worker(rank, size, estado);
  }
  // ----------------------------------------------------------
  // Fin de la paralelizacion:
  if(rank == MASTER){
    t1 = clock();
    double time = (double(t1 - t0) / CLOCKS_PER_SEC);
    printf(" \n Tiempo de ejecucion: %f \n", time);
  }
  return 0;
}
void calcular(int inicio, int final, int y, float *u1, float *u2){
   int ix, iy;
   for (ix = inicio; ix <= final; ix++){ 
      for (iy = 1; iy <= y-2; iy++){
        float aux = *(u2+ix*y+iy);
         *(u2+ix*y+iy) = *(u1+ix*y+iy)  + 
                          parms.cx * (*(u1+(ix+1)*y+iy) +
                          *(u1+(ix-1)*y+iy) - 
                          2.0 * *(u1+ix*y+iy)) +
                          parms.cy * (*(u1+ix*y+iy+1) +
                         *(u1+ix*y+iy-1) - 
                          2.0 * *(u1+ix*y+iy));
        //printf("(%d,%d): ",ix,iy);
        //printf("[%f] -> [%f] = %f + %f * (%f + %f - 2*(%f)) + ",*(u2+ix*y+iy),aux,*(u1+ix*y+iy),parms.cx,*(u1+(ix+1)*y+iy),*(u1+(ix-1)*y+iy),*(u1+ix*y+iy));
        //printf("%f * (%f + %f - 2*(%f)) + \n",parms.cy,*(u1+ix*y+iy+1),*(u1+ix*y+iy-1),*(u1+ix*y+iy));
                          
      }
    }
}
void inicializar(int x,int y,float *m){
  for (int i = 0; i < x-1; ++i)
  {
    for (int j = 0; j < y-1; ++j)
    {
      *(m+i*y+j) = (float)(i * (x - i - 1) * j * (y - j - 1));
    }
  }
}
void imprimir(int x, int y, float *u1, char *fnam) {
  int iX, iY;
  FILE *fp;

  fp = fopen(fnam, "w");
  for (iY = y-1; iY >= 0; iY--) {
    for (iX = 0; iX <= x-1; iX++) {
      fprintf(fp, "%8.1f", *(u1+iX*y+iY));
      if (iX != x-1) 
       fprintf(fp, " ");
      else
       fprintf(fp, "\n");
      }
   }  
  fclose(fp);
}
void master(int rank, int size, MPI_Status estado){
  inicializar(X, Y, matriz[0][0]);
  imprimir(X, Y, matriz[0][0], initial_d);
  // ----------------------------------------------------------
  // Paralelizacion: Datos
  cantidad_trabajadores = size-1; // Cantidad de trabajadores displonibles
  rango = X/cantidad_trabajadores;// Cantidad de datos que se repartiran por trabajador
  extra = X%cantidad_trabajadores;// Cantidad de datos extras que faltan por repartit
  inicio = 0; 
  // 1. Comenzamos a repartir los datos a los trabajadores
  for (int i=1; i<=cantidad_trabajadores; i++){
  // 2. Obtenemos la cantidad de filas de la matriz
  // que cada trabajador obtendra
  if(i <= extra)
    filas = rango+1;
  else
    filas = rango; 
    // 3. Ahora dependiendo del hilo actual se detecta
    // cuales hilos seran sus vecinos 
    if (i == 1)
      izquierda = 0;
    else
      izquierda = i - 1;
    if (i == cantidad_trabajadores)
      derecha = 0;
    else
      derecha = i + 1;
    // 4. Mandamos los datos necesarios a cada 
    MPI_Send(&inicio, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
    MPI_Send(&filas, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
    MPI_Send(&izquierda, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
    MPI_Send(&derecha, 1, MPI_INT, i, 1, MPI_COMM_WORLD); 
    MPI_Send(&matriz[0][inicio][0], filas*Y, MPI_FLOAT, i, 1,MPI_COMM_WORLD);
    // 5. Actualizamos el rango donde inicia el reparto de los datos
    inicio = inicio + filas;
  }
  // 6. Ahora tenemos que recibir la matriz modificada por cada trabajado
  // Para esto necesitamos el inicio y las cantidades de fila por cada trabajador
  for (int i=1; i<=cantidad_trabajadores; i++){
    MPI_Recv(&inicio, 1, MPI_INT, i, 4, MPI_COMM_WORLD,&estado);
    MPI_Recv(&filas, 1, MPI_INT, i, 4, MPI_COMM_WORLD, &estado);
    MPI_Recv(&matriz[0][inicio][0], filas*Y, MPI_FLOAT, i,4, MPI_COMM_WORLD, &estado);
  }
  // 7. Ahora que tenemos la matriz lista solo queda imprimir la matriz resultante
  imprimir(X, Y, &matriz[0][0][0], final_d);
  MPI_Finalize();
}
void worker(int rank, int size, MPI_Status estado){
  // ----------------------------------------------------------
  // 1. Inicializar las matrices
  for (int z=0; z<2; z++)
    for (int x=0; x<X; x++) 
      for (int y=0; y<Y; y++) 
        matriz[z][x][y] = 0.0;
  // ----------------------------------------------------------
  // 2. Recibo los datos que el MASTER envia para realizar los calculos
  MPI_Recv(&inicio, 1, MPI_INT, MASTER, 1, MPI_COMM_WORLD, &estado);
  MPI_Recv(&filas, 1, MPI_INT, MASTER, 1, MPI_COMM_WORLD, &estado);
  MPI_Recv(&izquierda, 1, MPI_INT, MASTER, 1, MPI_COMM_WORLD, &estado);
  MPI_Recv(&derecha, 1, MPI_INT, MASTER, 1, MPI_COMM_WORLD, &estado);
  MPI_Recv(&matriz[0][inicio][0], filas*Y, MPI_FLOAT, MASTER, 1,MPI_COMM_WORLD, &estado);
  // ----------------------------------------------------------
  // 3. Calculamos los rangos de las filas obtenidas:
  start=inicio;
  end=inicio+filas-1;
  if (inicio==0) 
    start=1;
  if ((inicio+filas)==X) 
    end--;
  // ----------------------------------------------------------
  // 4. Compartimos los datos entre los vecinos y calculamos el nuevo valor
  int iz = 0; // Indice de la matriz 
  for (int i = 1; i <= PASOS; i++){
    // 5. Intercambio de valores entre hilos vecinos
    if (izquierda != 0){
      MPI_Send(&matriz[iz][inicio][0], Y, MPI_FLOAT, izquierda,3, MPI_COMM_WORLD);
      MPI_Recv(&matriz[iz][inicio-1][0], Y, MPI_FLOAT, izquierda,2, MPI_COMM_WORLD, &estado);
    }
    if (derecha != 0){
      MPI_Send(&matriz[iz][inicio+filas-1][0], Y, MPI_FLOAT, derecha,2, MPI_COMM_WORLD);
      MPI_Recv(&matriz[iz][inicio+filas][0], Y, MPI_FLOAT, derecha, 3,MPI_COMM_WORLD, &estado);
    }
    // 6. Calculamos la nueva matriz
    calcular(start,end,Y,&matriz[iz][0][0],&matriz[1-iz][0][0]);
    // 7. Actualizar el indice de la matriz 
    // este se modifica segun la cantidad de los pasos
    // para que las matrices se actualicen entre las dos
    iz = 1 - iz;
  }
  // ----------------------------------------------------------
  // 8. Por ultimo enviamos la ultima matriz modificada a MASTER
  MPI_Send(&inicio, 1, MPI_INT, MASTER, 4, MPI_COMM_WORLD);
  MPI_Send(&filas, 1, MPI_INT, MASTER, 4, MPI_COMM_WORLD);
  MPI_Send(&matriz[iz][inicio][0], filas*Y, MPI_FLOAT, MASTER, 4,MPI_COMM_WORLD);
  MPI_Finalize();
}