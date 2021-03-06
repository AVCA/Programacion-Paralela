#include <stdio.h>
#include <omp.h>
#define N 4
int tid, nth;
int comienzo, fin, x = 0, z = 0, zpar, A[N][N], i, j;
int main ()
{
//inicializar la matriz
for (i=0; i<N; i++)
    for (j=0; j<N; j++) 
            A[i][j] = A[i][j]+i;
        

#pragma omp parallel private(tid,nth,comienzo,fin,i,j) shared(z)
{
    tid = omp_get_thread_num();
    nth = omp_get_num_threads();
    comienzo = tid * N / nth;
    fin = (tid+1) * N / nth;
    printf("\n Proceso %d/%d; comienzo = %d, fin = %d \n", tid, nth, comienzo, fin);
    zpar = 0;
    for (i=comienzo; i<fin; i++)
        for (j=0; j<N; j++)
        {
        #pragma omp critical
        {    
        x = A[i][j] * A[i][j];
        A[i][j] = A[i][j] + x;
        zpar = zpar + x;
        }
        }
    z = z + zpar;
}
    printf("\n --> Matriz A[i][j]\n");
    for(i=0; i<N; i++)
    {
    printf("\n");
    for(j=0; j<N; j++) printf(" %8d ", A[i][j]);
    printf("\n");
    }
    printf("\n\n --> x, z, zpar = %d, %d, %d \n\n", x, z, zpar);
}