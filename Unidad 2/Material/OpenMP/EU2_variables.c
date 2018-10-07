#include <stdio.h>
#include <omp.h>
int tid, x=-1, y=-1, z=-1;
main ()
{
x = 2;
y = 1;
printf("\n\n ANTES --> x = %d\t y = %d\t z = %d\n", x, y, z);
#pragma omp parallel shared(y) private(x,z) // probar con x firstprivate
{
tid = omp_get_thread_num();
if (tid==0) printf("\n\n DENTRO(1) --> x = %d\t y = %d\t z = %d\n", x, y, z);
z = x*x + 3;
x = y*3 + z;
if (tid==0) y = x;
if (tid==0) printf("\n\n DENTRO(2) --> x = %d\t y = %d\t z = %d\n", x, y,
z);
}
printf("\n\n FUERA --> x = %d\t y = %d\t z = %d\n\n", x, y, z);
}