//============================================================================
// Name        : U1_P7.cpp
// Author      : Ana Victoria Cavazos Argot
// Practica    : U1_P7_1
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctime>
#include <omp.h>
#define SEED 45
#define H 1
#define x 100 // Cantidad de filas
#define y 100 // Cantidad de columnas y tamaño del vector

// Variables globales:
unsigned t0, t1;
long int *v;
long int *r;
long int **m = NULL;

float f_suma(long int numeros[], int num, int inicio, int fin){
	float suma=0;
	#pragma omp parallel for reduction(+:suma)
	for (int i = inicio; i < fin; ++i)
	{
		suma+=numeros[i];
	}
	return suma;
}

float f_media(long int suma, int num){
	return ((float)suma/num);
}

int f_moda(long int numeros[], int num){
	int aux=numeros[0];
	int max=numeros[num-1]+1;
	int repetidos[max];
	#pragma omp parallel for
	for(int i=0;i<max;i++){
		repetidos[i]=0;
	}
	#pragma omp parallel for shared(repetidos)
	for (int i=0; i < num; i++){
		if(numeros[i]==aux)repetidos[numeros[i]]++;
		else{aux=numeros[i];repetidos[numeros[i]]++;}
	}
	aux=0;
	int moda=0;
	int m = 0;
	#pragma omp parallel for shared(m,moda)
	for(int i=0;i<max;i++){
		//printf("%d ",repetidos[i]);
		if(repetidos[i]>moda){
			m = i;
			moda=repetidos[i];
		}
	}
	printf("\n");
	return m;
}

float f_var(long int numeros[], int num, float media){
	float aux=0;
	#pragma omp parallel for reduction(+:aux)
	for(int i=0;i<num;i++){
		aux+= pow(fabs(numeros[i]-media),2);
	}
	return aux/num;
}

void ordenamiento(long int numeros[], int num){
	int aux=0;
	#pragma omp parallel for
	for (int i=0; i<num;i++){
		for (int j=i+1;j<num;j++)
		{
			if (numeros[j] < numeros[i])
				{aux = numeros[i];
				 numeros[i] = numeros[j];
				 numeros[j]=aux;}
		}
	}
}

void multiplicacion(){
	int i=0,j=0;
	#pragma omp parallel for shared(r,i)
	for (i = 0; i < x; ++i)
	{
		r[i]= 0;
		for (j = 0; j < y; ++j)
		{
			r[i]+=m[i][j]*v[j];
		}
	}
}

int main() {
	t0=clock();
	// ------------------------------------
	// Variables locales:
	float suma=0,media=0,var=0;
	int moda=0,par=0,impar=0;
	//
	int n=y;
	v = (long int*)malloc(n*sizeof(int));
	r = (long int*)malloc(x*sizeof(int));
	//Filas
	m = (long int**)malloc(x*sizeof(int*));
	// Columnas
	for(int i=0;i<x;i++){
		m[i] = (long int*)malloc(y*sizeof(int));
		// Inicializar la matriz en ceros
		for(int j=0;j<y;j++){
			m[i][j]=0;
		}
	}
	//long int v[n];
	//long int r[x];
	//long int m[x][y];
	// ------------------------------------
	// Generamos los numeros
	srand(SEED);
	// ------------------------------------
	// Inicializamos el vector
	printf(" Vector: \n");
	printf(" [ ");
	for (int i=0; i < n; i++){
		v[i]= rand()%10+1;
	    printf("%d ",v[i]);
	}
	printf("]\n");
	printf ("\n");
	// ------------------------------------
	// Inicializamos la matriz
	printf(" Matriz: \n");
	for (int i=0; i < x; i++){
		printf(" | ");
		for (int j = 0; j < y; j++)
		{
			m[i][j] = rand()%10+1;
	    	printf("%d ",m[i][j]);
		}
		printf("|\n");
	}
	// ----------------------------------------------------------
	// Paralelismo:
	// Asignacion de hilos:
	omp_set_num_threads(H);
	// ----------------------------------------------------------
	// Multiplicacion:
	multiplicacion();
	// ----------------------------------------------------------
	// Elemento mas pequeño y grande:
	ordenamiento(v,n);
	// Paralelismo de datos:
	#pragma omp parallel shared(suma)
	{
		// Dividimos el conjunto de datos
		int tid = omp_get_thread_num();
		int nth = omp_get_num_threads();
		int inicio = tid * n / nth;
		int fin = (tid+1) * n / nth;
		// Suma:
		suma+= f_suma(v,n,inicio,fin);
	}
	//Paralelismo de funciones:
	#pragma omp parallel shared(suma,media,moda,var)
	{
		#pragma omp single
		{
			#pragma omp taskwait
			{
				// Moda:
				#pragma omp task
				moda = f_moda(v,n);

				// Media:
				#pragma omp task
				media = f_media(suma,n);

				#pragma omp taskwait
				{
					// Varianza:
					#pragma omp task
					var = f_var(v,n,media);
				}
			}
		}
	}
	// ----------------------------------------------------------
	// Multiplicacion:
	//multiplicacion(v,m,r);
	// ----------------------------------------------------------
	printf(" -------\n");
	printf(" Suma: %f\n",suma);
	printf(" Media: %f\n",media);
	printf(" Moda: %d\n",moda);
	printf(" Varianza: %f\n",var);
	printf(" -------\n");
	printf (" El elemento mas pequeño es: %d \n", v[0]);
	printf (" El elemento mas grande es: %d \n", v[n-1]);
	printf(" -------\n");
	printf(" Multiplicacion: \n");
	printf(" [ ");
	printf("%d ",r[0]);
	printf(", %d ",r[x-1]);
	printf("]");
	printf("\n");
	free(v);
	free(r);
	free(m);
	// ---------------------------------------------------------
	t1 = clock();
	double time = (double(t1-t0)/CLOCKS_PER_SEC);
	printf(" \nTiempo de ejecucion: %f \n",time);
	return 0;
}