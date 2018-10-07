//============================================================================
// Name        : main.cpp
// Author      : Ana Victoria Cavazos Argot
// Practica    : U1_P2_1
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <ctime> 
#define SEED 90

unsigned t0, t1;
int dimension=550;
int max_rutas=dimension+1;
int par=0,impar=0;
int **matriz=NULL;
bool completo=true,convexo=false;

void crear_fichero(char archivo[]){
	FILE *fichero;
	fichero=fopen(archivo,"w");
	if( fichero==NULL )
		printf("Error al abrir el fichero\n");
	else
	{
		//Escritura:
		//printf("\n%d",dimension);
		fprintf(fichero,"%d\n",dimension);
		srand(SEED);
		// Numero de rutas:
		int rutas = rand()%max_rutas+1;
		// Generamos las rutas
		// Se generan rutas*dimensiones para aumentar
		// la posibilidad de rutas distintas
		for(int i=0;i<rutas*(dimension);i++){
			// Nodos aleatorios
			int x = rand()%dimension+1;
			int y = rand()%dimension+1;
			while(y==x)
				y = rand()%dimension+1;
			//printf("\n%d %d",x,y); 
			fprintf(fichero,"%d %d\n",x ,y);
		}
	}
	fclose(fichero);
}

void cargar_fichero(char archivo[]) {
	FILE *fichero;
	fichero=fopen(archivo,"r");
	if( fichero==NULL )
		printf("Error al abrir el fichero\n");
	else
	{
		fscanf(fichero,"%d",&dimension);
		printf("%d\n",dimension);
		
		//Filas
		matriz = (int**)malloc(dimension*sizeof(int*));
		// Columnas
		for(int i=0;i<dimension;i++){
			matriz[i] = (int*)malloc(dimension*sizeof(int));
			// Inicializar la matriz en ceros
			for(int j=0;j<dimension;j++){
				matriz[i][j]=0;
			}
		}
		
		//
		int x,y;
		while( !feof(fichero) ){
			fscanf(fichero,"%d %d",&x,&y);
			//printf("%d %d\n",x,y);
			matriz[x-1][y-1]=1;
			matriz[y-1][x-1]=1;
		}
		//
		for(int i=0;i<dimension;i++){
			for(int j=0;j<dimension;j++){
				printf("%d|",matriz[i][j]);
			}
			printf("%\n");
		}
	}
	fclose(fichero);
}

void g_completo(){
	// ------------------------------------------------
	// Variables:
	// int aux: Contador de rutas por nodo.
	// ------------------------------------------------
	// Se necesita que cada nodo posea n-1 rutas para
	// considerar el grafo completo.
	// ------------------------------------------------
	int aux=0;
	// Comenzamos a contar las rutas de todos los nodos.
	for(int i=0;i<dimension;i++){
		for(int j=0;j<dimension;j++){
			// Si el nodo esta conectado a otro
			// sus valores seran igual a 1
			if(matriz[i][j]==1)
				aux+=1;
		}
		// Si el total de rutas del nodo actual
		// es menor que el (total de nodos - 1)
		// significa que el grafo NO ES COMPLETO.
		if(aux<dimension-1){
			printf("Incompleto\n");
			completo=false;
			break;
		}
		// Si el nodo actual tiene n-1 rutas 
		// se continua con el sigiente nodo, 
		// y se reinicia el contador.
		aux=0;
	}
	if(completo){
		printf("Completo\n");
	}
}

void g_convexo(int i,int j){
	// ------------------------------------------------
	// Variables:
	// int i: Nodo Inicial.
	// int j: Nodo Final.
	// ------------------------------------------------
	// Se necesita comprobar que desde cualquier nodo i
	// se puede llegar al nodo j.
	// ------------------------------------------------
	// Si matriz[i][j] = 1 significa que 
	// existe una ruta directa del nodo i
	// y el nodo j, por lo tanto puede ser convexo.
	// ------------------------------------------------
	if(matriz[i][j]==0){
		convexo=false;
	// ------------------------------------------------
	// Si matriz[i][j] = 0 significa que 
	// NO existe una ruta directa del nodo i
	// y el nodo j, por lo que hay que comprobar si existe
	// alguna ruta alternativa que permita llegar 
	// al nodo j recorriendo los demas nodos 
	// disponibles sin repetirlos.
	// ------------------------------------------------
	// EJEMPLO: ¿1 -> 2?
	// matriz[1][2] = 0		aux: 1	Visitado: 		
	// matriz[1][3] = 1		aux: 3	Visitado: 1		¿3 = 2? No
	// matriz[3][2] = 1		aux: 2	Visitado: 1,3	¿2 = 2? Si -> Terminamos
	// Ruta: 1 -> 3 -> 2
	// ------------------------------------------------
	// Variables:
	// int aux: Nodo siguente a recorrer en la ruta.
	// int visitados[]: Arreglo que almacena los nodos visitados.
	// ------------------------------------------------
	// Inicializamos la variable aux con el nodo inicial.
	int aux=i; 
	// Inicializamos el arreglo visitados en ceros.
	int visitados[dimension];
	for(int m=0;m<dimension;m++){
		visitados[m]=0;
	}
	// ------------------------------------------------
	// Inicializamos el arreglo visitados en la posicion
	// del nodo i con 1, ya que al ser el inicial es 
	// visitado desde el inicio.
	// ------------------------------------------------
	visitados[i]=1;
	// ------------------------------------------------	
	// A continuacion comprobamos que es posible iniciar
	// una ruta con el nodo i.
	for(int y=0;y<dimension;y++){
		// Si existe almenos una ruta es posible crear una ruta.
			if(matriz[i][y]==1){
				// Por lo tanto es posible que sea CONVEXO.
				convexo=true;
				break;
			}
	}
	// ------------------------------------------------
	// Ya que estamos seguros que es posible encontrar una
	// ruta entre i y j comenzamos a explorar todas las 
	// rutas disponibles.
	// ------------------------------------------------
	// Mientras que matriz[aux][j] = 0 y las rutas ya exploradas
	// permitan avanzar hacia otro nodo (aun es posible que sea 
	// convexo) hay que explorar las rutas disponibles entre los nodos.
	// ------------------------------------------------
	while(matriz[aux][j]==0 && convexo){
		// Comenzamos a recorrer todos los nodos
		for(int x=0;x<dimension;x++){
			for(int y=0;y<dimension;y++){
				// Si la ruta actual es igual a 1
				// y el nodo y no ha sido visitado
				// podemos utilizar esa ruta.
				if(matriz[x][y]==1 && visitados[y]==0){
						// Marcamos el nodo y como visitado.
						visitados[y]=1;
						// La variable aux toma el valor
						// del siguiente nodo a recorrer.
						aux=y;
						// Si el nodo actual y es igual al nodo j
						// significa que llegamos al nodo final esperado.
						if(y==j)
						{
							// x lo igualamos a dimension para que el
							// el ciclo exterior termine.
							x=dimension;
							break;
						}
						break;
					}
					// Si llegamos a la ultima ruta disponible del nodo actual
					// significa que no existe alguna ruta que nos permita 
					// llegar al nodo final j, por lo tanto debemos de terminar
					// el ciclo y ademas el grafo NO ES CONVEXO.
					if(y==dimension-1){convexo=false;break;
					}
				}
				// Si el nodo que permite avanzar al siguiente nodo (aux)
				// permite llegar al nodo final, no es necesario continuar 
				// buscando otro nodo, por lo que finalizamos el ciclo
				// y el grafo es CONVEXO.
				if(matriz[aux][j]==1){
					convexo=true;break;
				}
				// Si llegamos al ultimo nodo sin encontrar una ruta disponible
				// significa que no es posible llegar al ultimo nodo, por lo tanto
				// NO ES CONVEXO.
				if(x==dimension-1){convexo=false;break;}
			}
			
		}
	}
	else
	{
		convexo=true;
	}
}

void g_euleriano(){
	// ------------------------------------------------
	// Variables:
	// int aux: Contador de rutas por nodo
	// ------------------------------------------------
	int aux=0;
	for(int i=0;i<dimension;i++){
		for(int j=0;j<dimension;j++){
			// Contamos las rutas de cada nodo
			if(matriz[i][j]==1){
				aux++;
			}
		}
		// Contamos los nodos con rutas par o impar
		if(aux%2==0)par++;
		else impar++;
		aux=0;
	}
	// ------------------------------------------------
	// Un grafo posee un ciclo Euleriano:
	// Todos sus nodos tienen rutas par
	// Todos sus nodos tienen rutas par a excepcion de 2
	// ------------------------------------------------
	//printf("Par: %d\n",par);
	//printf("Impar: %d\n",impar);
	if(impar>2)
		printf("No Euleriano\n" );
	else
		printf("Euleriano\n" );
}

void propiedades(){
	printf("----------\n");
	g_completo();
	if(completo)
		convexo=true;
	else{
		for(int i=0;i<dimension;i++){
		for(int j=0;j<dimension;j++){
			if(j!=i){
				g_convexo(i,j);
				//printf("%d %d [%d]\n",i,j,convexo);
				if(!convexo)break;
				}
				
			}
			if(!convexo)break;
		}
	}
		if(convexo)
			printf("Convexo\n");	
		else
			printf("No Convexo\n");
	g_euleriano();
}
//============================================================================
// 									M A I N
//============================================================================

int main() {
	t0=clock();
	crear_fichero("Grafo.txt");
	cargar_fichero("Grafo.txt");
	propiedades();
	t1 = clock();
	double time = (double(t1-t0)/CLOCKS_PER_SEC);
	printf("Tiempo de ejecucion: %f \n",time);
	printf("%d\n",par);
	printf("%d\n",impar );
	return 0;
}
