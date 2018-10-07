#include <studio.h>
#include <omp.h>

float area=0.0,pi,x;
int i,n=100;

int main(){
	#pragma omp parallel for
	{
		for (int i = 0; i < n; ++i)
		{
			x=(i+0.5)/n;
			area+=4.0/(1.0+(x*x));
		}
	}
	pi=area/n;
	printf("pi=%f  \n",pi);
	return 0;
}