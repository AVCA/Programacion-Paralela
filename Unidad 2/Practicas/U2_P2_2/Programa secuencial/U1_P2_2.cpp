//============================================================================
// Name        : main.cpp
// Author      : Ana Victoria Cavazos Argot
// Practica    : U1_P2_2
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctime> 
#define SEED 20
#define N 5000

unsigned t0, t1;

float f_suma(long int numeros[], int num){
	float suma=0;
	for (int i = 0; i < num; ++i)
	{
		suma+=numeros[i];
	}
	return suma;
}

float f_media(long int suma, int num){
	return ((float)suma/num);
}

int f_moda(long int numeros[], int num){
	int aux=numeros[0];
	int max=numeros[num-1]+1;
	int repetidos[max];
	for(int i=0;i<max;i++){
		repetidos[i]=0;
	}
	for (int i=0; i < num; i++){
		if(numeros[i]==aux)repetidos[numeros[i]]++;
		else{aux=numeros[i];repetidos[numeros[i]]++;}
	}
	aux=0;
	int moda=0;
	int m = 0;
	for(int i=0;i<max;i++){
		//printf("%d ",repetidos[i]);
		if(repetidos[i]>moda){
			m = i;
			moda=repetidos[i];
		}
	}
	printf("\n");
	return m;
}

int f_par(long int numeros[], int num){
	int par=0;
	for (int i = 0; i < num; ++i)
	{
		if(numeros[i]%2==0)par++;
	}
	return par;
}

int f_impar(long int numeros[], int num){
	int impar=0;
	for (int i = 0; i < num; ++i)
	{
		if(numeros[i]%2!=0)impar++;
	}
	return impar;
}

float f_var(long int numeros[], int num, float media){
	float aux=0;
	for(int i=0;i<num;i++){
		aux+= pow(fabs(numeros[i]-media),2);
	}
	return aux/num;
}

float f_desv(long int numeros[], int num, float media){
	float aux=0;
	for(int i=0;i<num;i++){
		aux+= pow(fabs(numeros[i]-media),2);
	}
	float var =aux/num;
	return sqrt(var);
}

bool f_primo(int n){
	//Si MOD 2 == 0 no es primo
	//Si Mod 2 == 1 puede ser primo
	if (n <= 1)
			return false;
		if (n <= 3)
			return true;
		if (n % 2 == 0 || n % 3 == 0)
			return false;
		int i = 5;
		while ((i * i) <= n) {
			if (n % i == 0 || n % (i + 2) == 0)
				return false;
			i += 6;
		}
		return true;
}

bool f_amigo(int a,int b){
	int suma_a=0, suma_b=0;
	// 1+2+4+5+10+11+20+22+44+55+110 = 284 = a
	for(int i=1;i<a;i++){
        if(a % i ==0)suma_a=suma_a+i;
    }
    // 1+2+4+71+142 = 220 = b
    for(int i=1;i<b;i++){
        if(b % i ==0)suma_b=suma_b+i;
    }
    if((suma_a==b)&&(suma_b==a))
    	return true;
    else
    	return false;
}

int main(void) {
	t0=clock();
	int num=N, i=0, j=0, aux;
	long int numeros[num];
	int primos[num],amigos[num][num];

	puts("!!!Hello Students!!!");
	puts("METODO DE LA BURBUJA");
	//puts("Cuantos Numeros quieres:");
	//scanf("%d",&num);
	// LLENAR ARREGLO CON VALORES ALEATORIOS
	srand(SEED);
	for (i=0; i < num; i++){
		numeros[i]= rand();
	    printf("%d, ",numeros[i]);
	}
	printf ("\n");
	//ORDENAR
	for (i=0; i<num;i++){
		for (j=i+1;j<num;j++)
		{
			if (numeros[j] < numeros[i])
				{aux = numeros[i];
				 numeros[i] = numeros[j];
				 numeros[j]=aux;}
		}
	}
	for (i=0; i < num; i++){
		printf("%d, ",numeros[i]);
	}
	printf ("\n");
	// ----------------------------------------------------------
	// Suma:
	float suma = f_suma(numeros,num);
	// Media:
	float media = f_media(suma,num);
	// Moda:
	int moda = f_moda(numeros,num);
	// Par:
	int par = f_par(numeros,num);
	// Impar:
	int impar = f_impar(numeros,num);
	// Varianza
	float var = f_var(numeros,num,media);
	// Desviacion estandar
	float desv = f_desv(numeros,num,media);
	// Primos
	for(int i=0;i<num;i++){
		primos[i]=0;
		if(f_primo(numeros[i])) primos[i]=numeros[i];
	}
	// ----------------------------------------------------------
	printf ("\n");
	printf ("El elemento mas pequeño es: %d \n", numeros[0]);
	printf (" El elemento mas grande es: %d \n", numeros[num-1]);
	printf("-------\n");
	printf("Suma: %f\n",suma);
	printf("Media: %f\n",media);
	printf("Moda: %d\n",moda);
	printf("-------\n");
	printf("Desviacion estandar: %f\n",desv);
	printf("Varianza: %f\n",var);
	printf("-------\n");
	printf("Pares: %d\n",par);
	printf("Impares: %d\n",impar);
	printf("-------\n");
	printf("Primos: ");
	for(int i=0;i<num;i++){
		if(primos[i]!=0)
			printf("%d,",primos[i]);
	}
	printf ("\n");
	// ----------------------------------------------------------
	// Amigos
	printf("Amigos:\n");
	for(int i=0;i<num;i++){
		for(int j=0;j<num;j++){
			if(numeros[i]!=numeros[j])
				if(f_amigo(numeros[i],numeros[j]))
					printf("%d y %d \n",numeros[i],numeros[j]);;
		}
	}
	t1 = clock();
	double time = (double(t1-t0)/CLOCKS_PER_SEC);
	printf("Tiempo de ejecucion: %f \n",time);
return EXIT_SUCCESS;
}