/**********************************************************************************
sec.c
Ejemplo de secciones en paralelo (ver orf.c para directivas huerfanas)
**********************************************************************************/
//// Probar con 4 threads, con 3 (lo mismo) y con 2 (el doble de tiempo)
#include <omp.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#define NA 2000000
#define NB 2000000
#define NC 2000000
struct timeval t0, t1;
double tej;

double func (int N)
{
	usleep(N);
	return(1);
}

main ()
{
	int tid=-1, nth;
	double A, B, C, D;;
	gettimeofday(&t0, 0);
	#pragma omp parallel private(tid, nth)
	{
		tid = omp_get_thread_num();
		nth = omp_get_num_threads();
		if (tid==0) printf("\n Ejecucion en paralelo con %d hilos\n", nth);
		#pragma omp sections
		{
			#pragma omp section
			{
				A = func(NA);
			}
			#pragma omp section
			{
				B = func(NB);
			}
			#pragma omp section
			{
				C = func(NC);
			}
		}
	}
	D = A + B + C;
	gettimeofday(&t1, 0);
	printf ("\n Resultados A = %.2f B = %.2f C = %.2f ---> D = %.2f", A,B,C,D);
	tej = (t1.tv_sec - t0.tv_sec) + (t1.tv_usec - t0.tv_usec) / 1e6;
	printf("\n\n T. de ejec. = %1.3f ms\n\n", tej*1000);
}