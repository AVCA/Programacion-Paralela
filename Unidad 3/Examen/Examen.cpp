//============================================================================
// Name        : examen.cpp
// Author      : Ana Victoria Cavazos Argot
// Exposicion  : examen
// Referencia: 
//============================================================================

//------------------------------------------------
// Libreria:
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

//------------------------------------------------
// Variables globales:
#define MASTER 0
#define SEED 45

unsigned t0, t1;

int rank, size, c;
MPI_Status estado;

//------------------------------------------------
// Metodos a utilizar;
void master(int rank, int size, MPI_Status estado);
void worker(int rank, int size, MPI_Status estado);

int main(int argc, char *argv[]){
    t0 = clock();
    // ----------------------------------------------------------
    // Paralelizacion: MPI
    int rank, size;
    MPI_Status estado;
    MPI_Init(&argc, &argv); // Inicializamos los procesos
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Obtenemos el numero total de procesos
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtenemos el valor de nuestro identificador
    // ----------------------------------------------------------
    // Master:
    if(rank == MASTER){
        master(rank, size, estado, NA);
    }
    if(rank > MASTER){
        worker(rank, size, estado);
    }
    MPI_Finalize();
    // ----------------------------------------------------------
    if(rank == MASTER){
        t1 = clock();
        double time = (double(t1 - t0) / CLOCKS_PER_SEC);
        printf(" \n Tiempo de ejecucion: %f \n", time);
    }
    return 0;
}

void master(int rank, int size, MPI_Status estado, int nodo_actual){
    
}

void worker(int rank, int size, MPI_Status estado){
    
}
