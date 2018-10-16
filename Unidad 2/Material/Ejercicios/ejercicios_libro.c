/* threadp.c OMP
Ejemplo de uso de variables threadprivate
RP1: copyin(x) --> toma el valor inicial del master: y/z sin inicializar (0)
RP2: x/y mantienen el valor de la region paralela anterior (threadprivate)
RP3: una rutina: x/z mantienen el valor, pero z no es privada (var. global original)
     tid si es privada porque se pasa como parámetro.  
*/
#include <stdio.h>
#ifdef _OPENMP
 #include <omp.h>
 #define TRUE 1
 #define FALSE 0
#else
 #define omp_get_thread_num() 0
 #define omp_get_num_threads() 1
#endif

int x,y,z,i,tid;
#pragma omp threadprivate(x,y)

void imprimir(int tid){
	printf("Thread %d: x=%d  y=%d  z=%d \n",tid,x,y,z );
}

main(){
	x=y=z=5;
	printf("\n\n Valores antes de la region paralela: x=%d  y=%d  z=%d \n",x,y,z );

	printf("\n Region paralela 1: x/y threadprivate; x copyin; z/tid private \n\n");

	#pragma omp parallel private (z,tid) copyin(x)
	{
	tid=omp_get_thread_num();
	printf("	A: Thread %d: x=%d  y=%d  z=%d \n",tid,x,y,z );

	x=x+tid+1;
	y=tid;
	z=tid+20;
	#pragma omp barrier
	printf("	B: Thread %d: x=%d  y=%d  z=%d \n",tid,x,y,z );
	}

	printf("\n >>>>>>>>>>>>>> Ejecucion de un trozo en serie >>>>>>>>>>>>>>>>\n");

	printf("\n Region paralela 2: x/y threadprivate; z/tid private \n\n");
	#pragma omp parallel private (z,tid) 
	{
	tid=omp_get_thread_num();
	printf("	Thread %d: x=%d  y=%d  z=%d \n",tid,x,y,z );
	}

	printf("\n >>>>>>>>>>>>>> Ejecucion de un trozo en serie >>>>>>>>>>>>>>>>\n");

	printf("\n Region paralela 3: se imprime desde una rutina: tid como parametro \n\n");
	#pragma omp parallel private (z,tid) 
	{
	tid=omp_get_thread_num();
	imprimir(tid);
	}

	printf("\n Main: se imprime desde una rutina: -1 como parametro \n\n");
	imprimir(-1);
	printf("\n Fin del main: x=%d  y=%d  z=%d \n\n",x,y,z);
}


/*
tarea.c
Ejemplo para ver el efecto del reparto en el tiempo de ejecución
Ejecutar modificando el scheduling: static, static 4, dynamic
*/

#include <omp.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

struct timeval t0,t1;
double 	tej;
#define  N 4000;

double calculo(int veces){
	usleep(veces);
	return(1);
}

int i,A[N],nth=-1;
double total;

main(){
	//inicializacion vector de tamaño de tareas
	for(i=0;i<N; i++){
		A[i]=1;
	}
	A[1]=100000;
	A[5]=100000;

	gettimeofday(&t0,0);
	total=0.0;

	#pragma omp parallel for schedule(runtime) reduction(+:total)
	for (i = 0; i < N; i++)
	{
		total+=calculo(A[i]);
	}
	gettimeofday(&t1,0);
	tej=(t1.tv_sec - t0.tv_sec) + (t1.tv_usec - t0.tv_usec) /1e6;

	#ifdef _OPENMP
		nth=omp_get_max_threads();
	#endif

	printf("\n\n Tej con %d hilos (-1=serie) = %1.3f s\n\n Total= %.2f \n\n",nth,tej,total );
}

/* EJERCICIO
MATVEC.C

ESCRIBIR UN PROGRAMA QUE EFECTÚE EL SIG. CALCULO CON MATRICES Y VECTORES:
double A[N*N], B[N], C[N], D[N], X

C[N]=A[N*N]* B[N]
D[N]=A[N*N]* C[N]
X=C[N].D[N]

EL PROGRAMA DEBE PEDIR AL PRINCIPIO EL TAMAÑO DE LOS VECTORES, n (MAX n=1000) Y EL NÚMERO DE THREADS.
*/

/* 
sec.c
Ejemplo de secciones en paralelo (ver orf.c para directivas huérfanas)
Probar con 4 threads, con 3 (lo mismo) y con 2 (el doble de tiempo)
*/

#include <omp.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

#define NA 2000000
#define NB 2000000
#define NC 2000000

struct timeval t0,t1;
double tej;

double func(int N){
	usleep(N);
	return(1);
}

main(){
	int tid=-1,nth;
	double A,B,C,D;

	gettimeofday(%t0,0);
	#pragma omp parallel private (tid,nth)
	{
		#ifdef _OPENMP
		tid=omp_get_thread_num();
		nth=omp_get_num_threads();
		if(tid==0){
			printf("\nEjecucion en paralelo con %d hilos\n", nth);
		}
		#endif

		#pragma omp sections
		{
			#pragma omp section
			{
			A=func(NA);
			}
			#pragma omp section
			{
			B=func(NB);
			}
			#pragma omp section
			{
			C=func(NC);
			}
		}
	}
	D=A+B+C;

	gettimeofday(&t1,0);
	printf("\n Resultados A=%.2f  B=%.2f  C=%.2f  ---> D=%.2f \n",A,B,C,D );

	tej=(t1.tv_sec - t0.tv_sec) + (t1.tv_usec - t0.tv_usec) /1e6;
	printf("\n\n Tiempo de ejecucion = %1.3f ms\n\n",tej*1000 );
}

/*
orf.c OPEN MP
Cálculo de un producto escalar
reparto de taeas tipo orphan
Ejecutar la versión serie (orf_ser) antes para ver lo que tiene que dar
*/

#include <omp.h>
#include <stdio.h>

#define N 100
float A[N], B[N],pe;

void AporB(){
	int j;
	#pragma omp for reduction (+:pe)
	for(j=0;j<N;j++){
		pe+=A[j]*B[j];
	}
}

main(){
	int i;

	for (i = 0; i < N; i++)
	{
		A[i]=i;
		B[i]=N-i;
	}
	pe=0.0;

	#pragma omp parallel
	{
		AporB();
	}

	printf("\n\n 	>> PE=%10.0f \n\n",pe);
}

/*
nested.c
Ejemplo de regiones paralelas anidadas
*/

#ifdef _OPENMP
 #include <omp.h>
 #define TRUE 1
 #define FALSE 0
#else
 #define omp_get_thread_num() 0
 #define omp_get_num_threads() 1
 #define omp_get_nested() 0
 #define omp_get_level() 0
#endif

int main(){
	int tid=-1;

	#ifdef _OPENMP
		(void) omp_set_num_threads(4);
		
		(void) omp_set_nested(TRUE);
		if(! omp_get_nested()){
			printf("Cuidado!!! No se ha activado el paralelismo anidado \n");
		}
	#endif

		printf("Paralelismo anidado esta %s \n",omp_get_nested() ? "activado":"no activado" );

	#pragma omp parallel private(tid)
	{
		tid=omp_get_thread_num();
		printf("TID (%d) ejecuta la region externa (nivel: %d)\n",tid,omp_get_level() );

		#pragma omp parallel num_threads(3)
		{
			printf("  >>>TID (%d.%d):  El thread %d ejecuta la region interna (nivel:%d) \n",tid,omp_get_thread_num(),omp_get_thread_num(),omp_get_level() );
		}

	}
	return(0);
}

/*
EJERCICIO: histo.c

Paralelizar un programa que efectúa unas cuantas operaciones sobre una matriz (una "imagen"):

1) calcular el histograma de la imagen (datos iniciales: de 0 a NG).
2) calcular el valor minimo del histograma ("nivel de gris que menos veces aparece").
3) a partir de ahí, calcula:
	B: vector que contiene la suma de los elementos de cada fila hasta (sin incluir) encontrar el valor minimo
	C: vector que contiene la posicion del minimo en cada fila
	SPM: suma de las posiciones del minimo

En una primera versión usar una imagen de tamaño 10x10 y un valor de NG de 10
*/

/* histo ser.c */
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define N 10 /* luego 6000*/
#define NG 10 /* luego 256*/

main(){
	struct timeval t0,t1;
	double tej;

	int IMA[N][N],histo[NG],B[N],C[N];
	int i,j,tid,hmin,imin,spm=0,x;

	//inicializacion de variables (aleatorio)
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			IMA[i][j]=rand()%NG;
		}
	}

	//se imprimen 10 elementos de 10 filas de IMA
	printf("\n Matriz IMA\n");
	for(i=0;i<10;i++){
		printf("\n");
		for(j=0;j<10;j++){
			printf(" %3d", IMA[i][j]);
			printf("\n");
		}
	}

	for(i=0;i<NG;i++){
		histo[i]=0;
	}

	//toma de tiempos
	gettimeofday(&t0,0);

	//1. calculo del histograma de IMA
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			histo[IMA[i][j]]++;
		}
	}

	//2. Busqueda del minimo del histograma
	hmin=N*N+1;
	for(i=0;i<N;i++){
		if(hmin>histo[i]){
			hmin=histo[i];
			imin=i;
		}
	}

	//calculo de B,C Y SPM
	for(i=0;i<N;i++){
		j=0;
		x=0;
		while((IMA[i][j]!=imin) && (j<N)){
			x=x+IMA[i][j];
			j++;
		}
		B[i]=x;
		C[i]=j;
		spm=spm+j;
	}

	//toma de tiempos
	gettimeofday(&t1,0);

	//imprimir resultados
	printf("\nHistograma\n");
	for(i=0;i<10;i++){
		printf("%5d",i );
	}
	printf("\n");
	for(i=0;i<10;i++){
		printf("%5d",histo[i] );
	}

	printf("\n hmin=%d  imin=%d \n",hmin,imin );

	printf("\nVector B \n");
	for(i=0;i<10;i++){
		printf("%3d",B[i] );
	}
	printf("\nVector C \n");
	for(i=0;i<10;i++){
		printf("%3d",C[i] );
	}

	printf("\n SPM=%d\n\n", spm );

	//tej=(t1.tv_sec - t0.tv_sec) + (t1.tv_usec - t0.tv_usec) /1e6;
	//printf("\n\n Tiempo de ejecucion (serie) = %1.3f ms\n\n",tej*1000 );

}