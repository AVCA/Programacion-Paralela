//============================================================================
// Name        : U1_P8.cpp
// Author      : Ana Victoria Cavazos Argot
// Practica    : U1_P8_1
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctime> 
#define SEED 45
#define n 1000000 // 1,000,000 

// Variables globales:
unsigned t0, t1;

float f_suma(int numeros[], int num){
	float suma=0;
	for (int i = 0; i < num; ++i)
	{
		suma+=numeros[i];
	}
	return suma;
}

float f_media(int suma, int num){
	return ((float)suma/num);
}

int f_moda(int numeros[], int num){
	int aux=numeros[0];
	int max=numeros[num-1]+1;
	int repetidos[max];
	for(int i=0;i<max;i++){
		repetidos[i]=0;
	}
	for (int i=0; i < num; i++){
		if(numeros[i]==aux)repetidos[numeros[i]]++;
		else{aux=numeros[i];repetidos[numeros[i]]++;}
	}
	aux=0;
	int moda=0;
	int m = 0;
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

float f_var(int numeros[], int num, float media){
	float aux=0;
	for(int i=0;i<num;i++){
		aux+= pow(fabs(numeros[i]-media),2);
	}
	return aux/num;
}

void ordenamiento(int numeros[], int num){
	int aux=0;
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
	int *v = (int*)malloc(n*sizeof(int));
	// ------------------------------------
	// Generamos los numeros
	srand(SEED);
	// ------------------------------------
	// Inicializamos el vector
	for (int i=0; i < n; i++){
		v[i]= rand()%500+1;
	    printf("%d ",v[i]);
	}
	printf("\n");
	// ----------------------------------------------------------
	// Elemento mas pequeño y grande:
	ordenamiento(v,n);
	// Suma:
	float suma = f_suma(v,n);
	// Media:
	float media = f_media(suma,n);
	// Moda:
	int moda = f_moda(v,n);
	// Varianza:
	float var = f_var(v,n,media);
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
	printf("\n");
	free (v);
	// ----------------------------------------------------------
	t1 = clock();
	double time = (double(t1-t0)/CLOCKS_PER_SEC);
	printf(" \nTiempo de ejecucion: %f \n",time);
	return 0;
}