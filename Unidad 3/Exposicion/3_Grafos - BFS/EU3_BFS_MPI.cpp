//============================================================================
// Name        : EU3_BFS_MPI.cpp
// Author      : Ana Victoria Cavazos Argot
// Exposicion  : EU3_BFS_MPI
// Referencia: https://computing.llnl.gov/tutorials/mpi/samples/C/mpi_array.c
//============================================================================

//------------------------------------------------
// Libreria:
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <queue>
#include <iostream>
#include <math.h>
#include "mpi.h"

//------------------------------------------------
// Variables globales:
#define MASTER 0
#define N 5 // MAXIMO en pc: 10000
#define A 1
#define NA 0
#define SEED 45

using namespace std;

unsigned t0, t1;
int n=N, a;//, nodo=1;

int rank, size, c;
MPI_Status estado;
std::queue <int> cola;

int *visitado = NULL;
int *visitado_l = NULL;
int *padres = NULL;
int *padres_l = NULL;
int *cola_n = NULL;
int *cola_nl = NULL;
int *nodos = NULL;
//int matriz[N][N];
int **matriz = NULL;

//------------------------------------------------
// Metodos a utilizar;
void inicializar_malloc();
void cargar_fichero();
void imprimir(queue <int> cola, int visitado[], int padres[]);
void imprimir_matriz();

void master(int rank, int size, MPI_Status estado,int nodo_actual);
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
        inicializar_malloc();
        cargar_fichero();
        imprimir_matriz();
        master(rank, size, estado, NA);
        while(cola.size()>0){
            //printf(" faltan... \n");
            int n_a = cola.front();
            cola.pop();
            master(rank, size, estado,n_a);
        }
        printf("\n Recorrido BFS: [");
        for (int i = 0; i < n; ++i)
        {
            printf("%d | ", padres[i]);
        }
        printf("]\n");
    }
    if(rank > MASTER){
        worker(rank, size, estado);
    }
    for (int i = 0; i < N; ++i){
        free(matriz[i]);
    }
    free(matriz);
    free(visitado);
    free(visitado_l);
    free(padres);
    free(padres_l);
    free(nodos);
    MPI_Finalize();
    // ----------------------------------------------------------
    if(rank == MASTER){
        t1 = clock();
        double time = (double(t1 - t0) / CLOCKS_PER_SEC);
        printf(" \n Tiempo de ejecucion: %f \n", time);
    }
    return 0;
}

void cargar_fichero(){
    // Matriz:
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            matriz[i][j] = 0;
        }
    }
    srand(SEED);
    for(int i = 0; i < N; i++){
        for(int j = 0; j < A; j++){
            int y = rand()%N;
            while(y == i)
                y = rand()%N;
            matriz[i][y] = 1;
            matriz[y][i] = 1;
        }
    }
}

void inicializar_malloc(){
    visitado = (int *)malloc(N * sizeof(int));
    visitado_l = (int *)malloc(N * sizeof(int));
    padres = (int *)malloc(N * sizeof(int));
    padres_l = (int *)malloc(N * sizeof(int));
    cola_n = (int *)malloc(N * sizeof(int));
    nodos = (int *)malloc(N * sizeof(int));
    matriz = (int**)malloc(N*sizeof(int*));
    for (int i = 0; i < N; ++i)
    {
        visitado[i] = 0;
        visitado_l[i] = 0;
        padres[i] = -1;
        padres_l[i] = -1;
        cola_n[i] = -1;
        nodos[i] = -1;
        matriz[i] = (int*)malloc(N*sizeof(int));
        for (int j = 0; j < N; ++j)
        {
            matriz[i][j]=0;
        }
    }
    visitado[NA] = 1;
    padres[NA] = -1;
}

void imprimir(queue <int> cola, int visitado[], int padres[]){
    printf("\n Visitados: [");
    for (int i = 0; i < n; ++i)
    {
        printf("%d | ", visitado[i]);
    }
    printf("]");
    printf("\n Padres: [");
    for (int i = 0; i < n; ++i)
    {
        printf("%d | ", padres[i]);
    }
    printf("]");
    queue <int> sub = cola;

    printf("\n Cola: [");
    if(sub.size() > 0)
    {
        for (int i = 0; i < sub.size() + 1; ++i)
        {
            printf("%d | ", sub.front());
            sub.pop();
        }
    }
    printf("]\n");
}

void imprimir_matriz(){
    if(N<30){
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                printf(" %d |",matriz[i][j]);
            }
            printf("\n");
        }
    }
}

void master(int rank, int size, MPI_Status estado, int nodo_actual){
    for(int i = 0; i < n; i++)
    {
        if(matriz[nodo_actual][i] == 1 && visitado[i] != 1)
        {
            cola.push(i);
            visitado[i] = 1;
            padres[i] = nodo_actual;
        }
    }
    int completo=1;
    for (int i = 0; i < N; ++i)
    {
        if(visitado[i]==0)
            completo=0;
    }
    // ---------------------------------------
    // 3.Repartimos los nodos obtenidos entre los trabajadores
    if(completo==0){
        float cr = (float) cola.size() / (size - 1);
        c = ceil(cr);
        for (int i = 1; i < size; ++i)
        {
            for (int j = 0; j < c; ++j)
            {
                nodos[j] = -1;
                if(cola.size() > 0)
                {
                    nodos[j] = cola.front();
                    cola.pop();
                }
            }
            // 3.2 Enviamos los arreglos necesarios para el calculo
            MPI_Send(&matriz[0][0],N*N, MPI_INT,i,0,MPI_COMM_WORLD);
            MPI_Send(&c, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(nodos, N, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(visitado, N, MPI_INT, i, 0, MPI_COMM_WORLD);

            MPI_Recv(visitado_l, n, MPI_INT, i, 1, MPI_COMM_WORLD, &estado);
            MPI_Recv(padres_l, n, MPI_INT, i, 1, MPI_COMM_WORLD, &estado);
            MPI_Recv(cola_n, n, MPI_INT, i, 1, MPI_COMM_WORLD, &estado);

            for (int j = 0; j < n; ++j)
            {
                if(visitado[j] == 0)
                    visitado[j] = visitado_l[j];
                if(padres[j] == -1)
                    padres[j] = padres_l[j];
                if(cola_n[j] != -1)
                    cola.push(cola_n[j]);
            }
        }
    }
}

void worker(int rank, int size, MPI_Status estado){
    int c=0,nodo_actual=0;
    inicializar_malloc();

    MPI_Recv(&matriz[0][0],N*N, MPI_INT, MASTER, 0, MPI_COMM_WORLD, &estado);
    printf("---------\n");
    imprimir_matriz();
    MPI_Recv(&c, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD, &estado);
    MPI_Recv(nodos, N, MPI_INT, MASTER, 0, MPI_COMM_WORLD, &estado); 
    MPI_Recv(visitado, N, MPI_INT, MASTER, 0, MPI_COMM_WORLD, &estado);

    for (int j = 0; j < c; ++j)
    {
        nodo_actual = nodos[j];
        if(nodo_actual != -1)
        {
            for(int i = 0; i < N; i++)
            {
                if(matriz[nodo_actual][i] == 1 && visitado[i] != 1)
                {
                    visitado[i] = 1;
                    padres[i] = nodo_actual;
                    cola.push(i);
                    cola_n[i]=i;
                }
            }
        }

    }
    MPI_Send(visitado, N, MPI_INT, MASTER, 1, MPI_COMM_WORLD);
    MPI_Send(padres, N, MPI_INT, MASTER, 1, MPI_COMM_WORLD);
    MPI_Send(cola_n, N, MPI_INT, MASTER, 1, MPI_COMM_WORLD);
}
