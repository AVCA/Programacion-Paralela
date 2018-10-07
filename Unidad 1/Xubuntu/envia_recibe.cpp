/*
 ============================================================================
 Name        : send_receive2.cpp
 Author      : Sergio RodrÃ¬guez Lumley & Daniel Guerrero MartÃ¬nez
 Version     :
 Copyright   : GNU Open Source and Free license
 Description : Tutorial 2. Paso de mensajes.
	Este codigo muestra como formar una cadena a traves de paso de mensajes
	entre procesos. Comienza la proceso 0 enviando el mensaje, el resto de
	procesos, en orden, reciben el mensaje del proceso anterior y la envian
	a la siguiente, a excepcion de la ultima, que no envia ninguno.
 ============================================================================
 */
#include <mpi.h>
#include <iostream>
using namespace std;
 
int main(int argc, char *argv[]) 
{ 
    int rank, size, contador;
    MPI_Status estado;

    MPI_Init(&argc, &argv); // Inicializamos la comunicacion de los procesos
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Obtenemos el numero total de procesos
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtenemos el valor de nuestro identificador
 
    if(rank == 0){

    	MPI_Send(&rank //referencia al vector de elementos a enviar
    			,1 // tamaÃ±o del vector a enviar
    			,MPI_INT // Tipo de dato que envias
    			,rank+1 // pid del proceso destino
    			,0 //etiqueta
    			,MPI_COMM_WORLD); //Comunicador por el que se manda

    }else{

    	MPI_Recv(&contador // Referencia al vector donde se almacenara lo recibido
    			,1 // tamaÃ±o del vector a recibir
    			,MPI_INT // Tipo de dato que recibe
    			,rank-1 // pid del proceso origen de la que se recibe
    			,0 // etiqueta
    			,MPI_COMM_WORLD // Comunicador por el que se recibe
    			,&estado); // estructura informativa del estado

    	cout<<"Soy el proceso "<<rank<<" y he recibido "<<contador<<endl;
    	contador++;
    	if(rank != size-1)
    		MPI_Send(&contador, 1 ,MPI_INT ,rank+1 , 0 ,MPI_COMM_WORLD);

    }

	// Terminamos la ejecucion de los procesos, despues de esto solo existira
	// el proceso 0
	// Â¡Ojo! Esto no significa que los demas procesos no ejecuten el resto
	// de codigo despues de "Finalize", es conveniente asegurarnos con una
	// condicion si vamos a ejecutar mas codigo (Por ejemplo, con "if(rank==0)".
    MPI_Finalize();
    return 0;
} 
