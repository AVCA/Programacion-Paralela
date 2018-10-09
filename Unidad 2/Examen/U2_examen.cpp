//============================================================================
// Name        : U2_examen.cpp
// Author      : Ana Victoria Cavazos Argot
// Practica    : U2_examen
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <ctime>
#include <omp.h>
#define SEED 45
#define N 100000000 // 10,000,000
#define H 4

unsigned t0, t1;

float Montecarlo(){
	double x=0,y=0,aux=0;
	int count=0,i=0;
	// ----------------------------------------------------------
	// Paralelismo de proceso:
	#pragma omp parallel for shared(count) private(x,y,aux)
	for(i=0;i<N;i++){
		x = ((double)rand())/RAND_MAX;
		y = ((double)rand())/RAND_MAX;
		aux=pow(x,2)+pow(y,2);
		//printf("x = %f | y = %f | (x^1+y^2) = %f\n",x,y,aux);
		if(aux<=1)count++;
	}
	//printf("%d\n",count);
	// count = Conteo dentro del circulo (Area del circulo)
	// N = Conteo dentro del cuadrado (Area dentro del cuadrado) 
	return ((double)count/(double)(N))*4.0;
}
float Leibniz(){
	float suma=0;
	// ----------------------------------------------------------
	// Paralelismo de proceso:
	#pragma omp parallel for reduction(+:suma)
	for (int i = 0; i < N; ++i)
	{
		suma+=(pow(-1,i)/(2.0*i+1));
	}
	return suma*4.0;
}
float Nilakanta(){
	float suma=3;
	// ----------------------------------------------------------
	// Paralelismo de proceso:
	#pragma omp parallel for shared(suma)
	for (int i = 2; i < N;i++)
	{
		if(i%2==0)
			suma -= 4.0/(i*(i+1)*(i+2));
		else
			suma += 4.0/(i*(i+1)*(i+2));
		i++;
	}
	return suma;
}

//============================================================================
// 									M A I N
//============================================================================

int main() {
	t0=clock();
	// ----------------------------------------------------------
	// Paralelismo:
	// Asignacion de hilos:
	omp_set_num_threads(H);
	srand(SEED);
	printf("Montecarlo: %f\n",Montecarlo());
	printf("Leibniz: %f\n",Leibniz());
	printf("Nilakanta: %f\n",Nilakanta());
	// ----------------------------------------------------------
	t1 = clock();
	double time = (double(t1-t0)/CLOCKS_PER_SEC);
	printf("Tiempo de ejecucion: %f \n",time);
	return 0;
}
