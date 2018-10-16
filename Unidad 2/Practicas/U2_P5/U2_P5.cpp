//============================================================================
// Name        : U1_P5.cpp
// Author      : Ana Victoria Cavazos Argot
// Practica    : U1_P5
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctime>
#include <omp.h>
#define SEED 45
#define H 1000
#define N 500000

unsigned t0, t1;

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

float f_desv(long int numeros[], int num, float media){
	float aux=0;
	#pragma omp parallel for reduction(+:aux)
	for(int i=0;i<num;i++){
		aux+= pow(fabs(numeros[i]-media),2);
	}
	float var =aux/num;
	return sqrt(var);
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

int main() {
	t0=clock();
	// ------------------------------------
	// Variables locales:
	printf("Paralelo:\n");
	long int numeros[N];
	float suma=0,media=0,var=0,desv=0;
	int moda=0,par=0,impar=0,num=N;
	// Generamos los numeros 
	srand(SEED);
	for(int i=0;i<N;i++){
		numeros[i] = rand()%10+1;
		printf("%d ",numeros[i]);
	}
	printf("\n");
	// -----------------------------------
	// Ordenamiento de Burbuja
	ordenamiento(numeros,num);
	// ----------------------------------------------------------
	// Paralelismo:
	// Asignacion de hilos:
	omp_set_num_threads(H);
	// ----------------------------------------------------------
	// Paralelismo de datos:
	#pragma omp parallel shared(suma)
	{
		// Dividimos el conjunto de datos
		int tid = omp_get_thread_num();
		int nth = omp_get_num_threads();
		int inicio = tid * N / nth;
		int fin = (tid+1) * N / nth;
		// Suma:
		suma+= f_suma(numeros,num,inicio,fin);
	}
	//Paralelismo de funciones:
	#pragma omp parallel shared(suma,media,moda,var,desv)
	{
		#pragma omp single
		{
			#pragma omp taskwait
			{
				// Moda:
				#pragma omp task
				moda = f_moda(numeros,num);

				// Media:
				#pragma omp task
				media = f_media(suma,num);

				#pragma omp taskwait
				{
					// Varianza:
					#pragma omp task
					var = f_var(numeros,num,media);

					// Desviacion estandar:
					#pragma omp task
					desv = f_desv(numeros,num,media);
				}
			}
		}
	}
	// ----------------------------------------------------------

	printf("-----------------------------\n");
	printf("Media: %f\n",media);
	printf("Moda: %d\n",moda);
	printf("Desviacion estandar: %f\n",desv);
	printf("Varianza: %f\n",var);


	t1 = clock();
	double time = (double(t1-t0)/CLOCKS_PER_SEC);
	printf("Tiempo de ejecucion: %f \n",time);
	return 0;
}
