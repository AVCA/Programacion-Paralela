//============================================================================
// Name        : EU3_MPI_criba.cpp
// Author      : Ana Victoria Cavazos Argot
// Exposicion  : EU3_MPI_criba
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"
#include <ctime>

#define N 100//7925 //1000 numeros primos

// Variables globales:
unsigned t0, t1;
int v[N];

int main(int argc, char *argv[])
{
    t0 = clock();
    // ----------------------------------------------------------
    // Variables locales:
    int rank, size;
    int v[N], primos[N];
    int aux = 0, c = 0;
    // ----------------------------------------------------------
    //Inicializamos el arreglo binario
    for (int i = 2; i < N; ++i){
        v[i] = 1;
        primos[i] = 1;
    }
    // Comenzamos a obtener los numeros primos mediante la criba
    //printf(" 1000 primeros numeros primos: \n");
    // ----------------------------------------------------------
    // Paralelizacion: MPI
    MPI_Init(&argc, &argv); // Inicializamos los procesos
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Obtenemos el numero total de procesos
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtenemos el valor de nuestro identificador
    // Comenzamos desde 2 ya que ni 0 ni 1 es primo
    //printf("Primos: ");
    //printf("i = %f \n",floor(2 + rank * (N / size)));
    //printf(" < %f \n",(rank + 1) * (sqrt(N)));
    for(int i = floor(2 + rank * (N / size)); i < (rank + 1) * (sqrt(N)); i++){
       // printf(" \n",rank,size );
        printf("rank = %d | size = %d | %f - %f \n",rank,size,floor(2 + rank * (N / size)),(rank + 1) * (sqrt(N)) );
        // Si el valor actual no fue "eliminado",
        // es decir, el arreglo en su posicion es igual a 1
        // Significa que es PRIMO
        if(v[i] == 1)
        {
            //printf("%d ",i);
            // Procedemos a eliminar los multiplos del numero primo
            for(int h = 2; i*h < N; ++h)
                v[i*h] = 0;
        }
    }
    MPI_Allreduce(v,// Vector donde se eliminaron los multiplos
                  primos, // Vector donde se juntaran los vectores modificados por proceso
                  N, // Tamaño del vector primos
                  MPI_INT, // Tipo de datos
                  MPI_PROD, // Operacion a realizar. En este caso multiplica todos los elementos
                  MPI_COMM_WORLD); // Comunicador
    //Se reducen todos los m locales a resultado, que es a variable global
    MPI_Finalize();
    if(rank == 0){
        for(int i = 2; i < N; i++)
        {
            if(primos[i] == 1)
            {
                c++;
                printf(" %d, ",i);
            }
        }
        printf("\n Cantidad de primos obtenidos: %d\n", c);
        // ----------------------------------------------------------
        t1 = clock();
        double time = (double(t1 - t0) / CLOCKS_PER_SEC);
        printf(" \n Tiempo de ejecucion: %f \n", time);
    }
    return 0;
}