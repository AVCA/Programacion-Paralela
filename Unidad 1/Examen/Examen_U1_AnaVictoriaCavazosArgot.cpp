#include <stdio.h>
#include <stdlib.h>
int X=5;
int Y=5;
int PASOS=2;

// Esta estructura se requiere para el calculo de la temperatura
struct Parms { 
  float cx;
  float cy;
} parms = {0.1, 0.1};

void calcular(int inicio, int final, int y, float *u1, float *u2){
   int ix, iy;
   for (ix = inicio; ix <= final; ix++){ 
      for (iy = 1; iy <= y-2; iy++){
        float aux = *(u2+ix*y+iy);
         *(u2+ix*y+iy) = *(u1+ix*y+iy)  + 
                          parms.cx * (*(u1+(ix+1)*y+iy) +
                          *(u1+(ix-1)*y+iy) - 
                          2.0 * *(u1+ix*y+iy)) +
                          parms.cy * (*(u1+ix*y+iy+1) +
                         *(u1+ix*y+iy-1) - 
                          2.0 * *(u1+ix*y+iy));
        printf("(%d,%d): ",ix,iy);
        printf("[%f] -> [%f] = %f + %f * (%f + %f - 2*(%f)) + ",*(u2+ix*y+iy),aux,*(u1+ix*y+iy),parms.cx,*(u1+(ix+1)*y+iy),*(u1+(ix-1)*y+iy),*(u1+ix*y+iy));
        printf("%f * (%f + %f - 2*(%f)) + \n",parms.cy,*(u1+ix*y+iy+1),*(u1+ix*y+iy-1),*(u1+ix*y+iy));
                          
      }
    }

}

void inicializar(int X,int Y,float *m){
	for (int i = 0; i < X-1; ++i)
	{
		for (int j = 0; j < Y-1; ++j)
		{
			*(m+i*Y+j) = (float)(i * (X - i - 1) * j * (Y - j - 1));
		}
	}
}

void imprimir(int nX, int y, float *u1, char *fnam) {
	int iX, iY;
	FILE *fp;

	fp = fopen(fnam, "w");
	for (iY = y-1; iY >= 0; iY--) {
  	for (iX = 0; iX <= nX-1; iX++) {
    	fprintf(fp, "%8.1f", *(u1+iX*y+iY));
    	if (iX != nX-1) 
     	 fprintf(fp, " ");
    	else
     	 fprintf(fp, "\n");
    	}
 	 }	
	fclose(fp);
}


int main(int argc, char *argv[])
{
	float u[2][X][Y];
	// Comenzamos a inicializar las matrices
	inicializar(X,Y,&u[0][0][0]);
	imprimir(X,Y,&u[0][0][0],"inicial.dat");
	// Comenzamos a realizar los calculos de temperatura
	// esto se realizara por cada valor en relacion
	// a sus valores vecinos
	for(int i=0;i<PASOS;i++){
    	for (int ix=0; ix<X; ix++){
      		for (int iy=0; iy<Y; iy++){
        		u[1][ix][iy] = 0.0;
    		}
		}
   		calcular(1,X-2,Y,&u[0][0][0],&u[1][0][0]);
   		for (int ix=0; ix<X; ix++){
      		for (int iy=0; iy<Y; iy++){
        		u[0][ix][iy] = u[1][ix][iy];
      		}
        }
  	}
  	// Una vez que finaliza el calculo de las temperaturas
  	// obtenemos la matriz original con los resultados
  	imprimir(X,Y,&u[1][0][0],"final.dat");

	return 0;
}


