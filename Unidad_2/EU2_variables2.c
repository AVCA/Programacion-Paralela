#include <stdio.h>
#include <omp.h>
#define N 3
#define H 3
int tid, nth;
int comienzo, fin, x = 0, z = 0, zpar, A[N][N], i, j;
main ()
{
	printf("Matriz orignal: \n");
	for (i=0; i<N; i++){
		for (j=0; j<N; j++)
		{
			A[i][j] = i+j;//A[i][j]+i;//1;//i+j;
			printf(" %8d ",A[i][j]);
		}
		printf("\n\n");
	}
	omp_set_num_threads(H);
	#pragma omp parallel private(tid,nth,comienzo,fin,i,j) shared(z) //default(none)
	{
		tid = omp_get_thread_num();
		nth = omp_get_num_threads();
		comienzo = tid * N / nth;
		fin = (tid+1) * N / nth;
		printf("\nProceso %d/%d; comienzo = %d, fin = %d \n", tid, nth, comienzo, fin);
		zpar = 0;
		for (i=comienzo; i<fin; i++){
			printf("%d\n",z);
			for (j=0; j<N; j++){
				printf("//////////////////////////////////\n");
				printf("%d\n",z);
				printf("x = A[%d][%d] *A [%d][%d]\n",i,j,i,j);
				printf("%d = [%d] * [%d] \n",x,A[i][j],A[i][j]);
				x = A[i][j] * A[i][j];
				printf("x = %d\n",x);
				printf("--------\n");
				printf("A[%d][%d] = A[%d][%d] + x\n",i,j,i,j);
				printf("%d = %d + %d\n",A[i][j],A[i][j],x);
				A[i][j] = A[i][j] + x;
				printf("A[%d][%d]=%d\n",i,j,A[i][j]);
				printf("--------\n");
				printf("zpar = zpar + x\n");
				printf("zpar = %d + %d\n",zpar,x);
				zpar = zpar + x;
				printf("zpar = %d\n",zpar);
				printf("%d\n",z);
			}
			printf("z= %d\n",z);
			printf("z= %d + %d\n",z,zpar);
			z = z + zpar;
			printf("z= %d\n",z);
			printf("-----------------\n");
		}
	}
	printf("\n --> Matriz A[i][j]\n");
	for(i=0; i<N; i++){
		printf("\n");
		for(j=0; j<N; j++) printf(" %8d ", A[i][j]);
		printf("\n");
	}
	printf("\n\n --> x, z, zpar = %d, %d, %d \n\n", x, z, zpar);
}