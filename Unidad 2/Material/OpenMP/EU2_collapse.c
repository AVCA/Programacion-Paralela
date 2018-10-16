#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main()
{
	int j, k, jlast, klast;
	#pragma omp parallel
	{
		// collapse toma los valores
		// del nivel "n" del for
		#pragma omp for collapse(3) //lastprivate(jlast, klast)
		for (k=0; k<=1; k++)
			for (j=1; j<=4; j++)
			{
				for (j=1; j<=4; j++)
			{
				jlast=j;
				klast=k;
			}
			}
		printf("%d %d\n", klast, jlast);
		#pragma omp single
		printf("Final: %d %d\n", klast, jlast);
	}
	return 0;
}