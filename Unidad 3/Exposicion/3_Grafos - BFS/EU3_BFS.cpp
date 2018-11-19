//============================================================================
// Name        : EU3_BFS.cpp
// Author      : Ana Victoria Cavazos Argot
// Exposicion  : EU3_BFS
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <queue>
#include <iostream>

#define N 1000000
#define A 6
#define N_A 0
#define SEED 45

// Variables globales:
unsigned t0, t1;
int n, a, nodo_actual;
int *visitado=NULL;
int *padres=NULL;
int **matriz=NULL;
char archivo[] = "grafo_6_-1_0.txt";
using namespace std; 

//
void BFS();
void inicializar_malloc();
void crear_fichero(char archivo[]);
void cargar_fichero(char archivo[]);

int main(int argc, char *argv[]){
    t0 = clock();
    // ----------------------------------------------------------
    cargar_fichero(archivo);
    BFS();
    // ----------------------------------------------------------
    t1 = clock();
    double time = (double(t1 - t0) / CLOCKS_PER_SEC);
    printf(" \n Tiempo de ejecucion: %f \n", time);
    return 0;
}

void inicializar_malloc(int n){
  // ---------------------------------------------
  // Matriz:
  matriz = (int**)malloc(n*sizeof(int*));
  for(int i=0;i<n;i++){
    matriz[i] = (int*)malloc(n*sizeof(int));
    for(int j=0;j<n;j++){
      matriz[i][j]=0;
    }
  }
  // ------------------------------------
  // Arreglos:
  visitado = (int*)malloc(n*sizeof(int));
  padres = (int*)malloc(n*sizeof(int));
}

void cargar_fichero(char archivo[]) {
   FILE *fichero;
   fichero=fopen(archivo,"r");
   if( fichero==NULL )
      printf("Error al abrir el fichero\n");
   else
   { 
      fscanf(fichero,"%d",&n);
      fscanf(fichero,"%d",&a);
      int x,y;
      inicializar_malloc(n);
      for(int i = 0; i < a; i++)
      {
         fscanf(fichero,"%d",&x);
         fscanf(fichero,"%d",&y);
         matriz[x][y] = 1;
         matriz[y][x] = 1;
      }
      fscanf(fichero,"%d",&nodo_actual);
   }
   fclose(fichero);
}

void BFS(){
   // BFS
   printf(" n = %d",n );
   printf("\n a = %d",a );
   printf("\n");
   
   if(N<=10){
      for(int x=0;x<N;x++){
         for(int y=0;y<N;y++){
            printf(" [%d][%d] = %d |",x,y,matriz[x][y]);
         }
         printf("\n");
      }
   }
   
   queue <int> cola;
   cola.push(nodo_actual);
   visitado[nodo_actual] = 1;
   padres[nodo_actual] = -1;

   while(!cola.empty())
   {
      nodo_actual = cola.front();
      cola.pop();
      //printf("\n nodo_actual: %d",nodo_actual);
      for(int i = 0; i < n; i++){
         if(matriz[nodo_actual][i] == 1 && visitado[i] != 1)
         {
            visitado[i] = 1;
            padres[i] = nodo_actual;
            //printf("\n padre[%d]: %d",i,nodo_actual);
            cola.push(i);
         }
      }
      //printf("\n");
   }
   printf("\n [H][P]");
    for(int i = 0; i < n; i++)
    {
        printf("| [%d][%d] |",i,padres[i]);
    }
    free(matriz);
    free(visitado);
    free(padres);
}