//============================================================================
// Name        : U2_P3.cpp
// Author      : Ana Victoria Cavazos Argot
// Practica    : U2_P3
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#define SEED 45
#define N 5000000000 // 10,000,000
#define H 20

unsigned t0, t1;

float Montecarlo(){
	double x=0,y=0,aux=0;
	int count=0,i=0;
	// ----------------------------------------------------------
	// Paralelismo de proceso:
	//#pragma omp parallel for reduction(+:count)
	for(i=0;i<N;i++){
		x = ((double)rand())/RAND_MAX;
		y = ((double)rand())/RAND_MAX;
		aux=pow(x,2)+pow(y,2);
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
	int signo=1, j=2;
	#pragma om parallel for
	for (int i = 2; i < N;i++)
	{
		int a=i,b=a+1,c=a+2;
		//printf("------\n");
		//printf("suma =(%d) %f\n",signo,suma);
		//printf("a = %d | b = %d | c =%d\n",a,b,c);
		suma += signo*(4.0/(a*b*c));
		i++;
		if(signo==1)signo=-1;
		else signo=1;
	}
	//printf("suma = %f\n",suma);
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
	/*#pragma omp single
	{
		#pragma omp tasks
		{
			#pragma omp task
				printf("Montecarlo: %f\n",Montecarlo());
			#pragma omp task
				printf("Leibniz: %f\n",Leibniz());
			#pragma omp task
				printf("Nilakanta: %f\n",Nilakanta());
		}
	}*/
	// ----------------------------------------------------------
	t1 = clock();
	double time = (double(t1-t0)/CLOCKS_PER_SEC);
	printf("Tiempo de ejecucion: %f \n",time);
	return 0;
}
