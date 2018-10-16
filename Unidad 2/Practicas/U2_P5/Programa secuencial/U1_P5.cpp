//============================================================================
// Name        : main.cpp
// Author      : Ana Victoria Cavazos Argot
// Practica    : U1_P2_1
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctime>
#define SEED 45
#define N 500000

unsigned t0, t1;

float f_suma(int numeros[], int num){
	float suma=0;
	for (int i = 0; i < num; ++i)
	{
		suma+=numeros[i];
	}
	printf("suma = %f\n",suma);
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
	/*for(int i=0;i<max;i++){
		if(repetidos!=0)
			printf("%d ",i);
	}
	printf("\n");*/
	int moda=0;
	int m = 0;
	for(int i=0;i<max;i++){
		//printf("%d ",repetidos[i]);
		if(repetidos[i]>moda){
			m = i;
			moda=repetidos[i];
		}
	}
	//printf("\n");
	return m;
}

float f_var(int numeros[], int num, float media){
	float aux=0;
	for(int i=0;i<num;i++){
		aux+= pow(fabs(numeros[i]-media),2);
	}
	return aux/num;
}

float f_desv(int numeros[], int num, float media){
	float aux=0;
	for(int i=0;i<num;i++){
		aux+= pow(fabs(numeros[i]-media),2);
	}
	float var =aux/num;
	return sqrt(var);
}

int main() {
	t0=clock();
	// ------------------------------------
	// Arreglo que contendra los numeros:
	int numeros[N];
	printf("Secuencial: \n");
	// Generamos los numeros 
	srand(SEED);
	for(int i=0;i<N;i++){
		numeros[i] = rand()%10+1;
		printf("%d ",numeros[i]);
	}
	printf("\n");
	// -----------------------------------
	// Ordenar los numeros:
	int aux=0;
	for (int i=0; i<N;i++){
		for (int j=i+1;j<N;j++)
		{
			if (numeros[j] < numeros[i])
			{
				aux = numeros[i];
				numeros[i] = numeros[j];
				numeros[j]=aux;
			}
		}
	}
	/*printf("-----------------------------\n");
	for(int i=0;i<N;i++){
		printf("%d ",numeros[i]);
	}
	printf("\n");*/
	// ----------------------------------
	// Suma:
	float suma = f_suma(numeros,N);
	// Media: 
	float media = f_media(suma,N);
	// Moda:
	int moda = f_moda(numeros,N);
	// Varianza
	float var = f_var(numeros,N,media);
	// Desviacion estandar
	float desv = f_desv(numeros,N,media);


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
