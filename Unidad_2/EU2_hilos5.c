#include <omp.h>
#include <iostream>

main ()
{
printf("\nIntroduce el numero de threads ---> ");
scanf("%d",&nthr);
omp_set_num_threads(nthr);
#pragma omp parallel private(tid)
{
tid = omp_get_thread_num();
printf ("Thread %d en marcha \n", tid);
A[tid] = tid + 10;
printf ("El thread %d ha terminado \n", tid);
}
for (i=0; i<N; i++) printf ("A(%d) = %d \n", i, A[i]);
}