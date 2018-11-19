//1. Usar BCAST PARA enviar y recibir
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// comentario: archivo cabecera de la librer\ia de MPI
#include <mpi.h>
int main(int argc, char *argv[])
{
	char *buf = (char *)malloc(20 * sizeof(char));
	int cnt = 20;
	MPI_Status status;
	int res;

	int rank, size;
	MPI_Init(&argc, &argv);// Inicializa el entorno de ejecucion
	// MPI_COMM_WORLD -> comunicador por defecto
	// Rank -> Identificador del proceso
	// Size -> Cantidad de procesos
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Determina id. proceso en el comunicador
	MPI_Comm_size(MPI_COMM_WORLD, &size); // Determina nunmero de procesos del comunicador
	// Si el proceso es el proceso MASTER
	if( rank == 0 ){
		// Imprimo mensaje en el arreglo char
		sprintf(buf, "Hola 1 y 2");
		// Envio el mensaje a todos los hilos trabajadores
		res = MPI_Bcast(buf, cnt, MPI_CHAR, 0, MPI_COMM_WORLD);
		// Respuesta del envio
		printf("res: %d\n", res);
		// Constantes del estado del mensaje
		printf("MPI_SUCCESS: %d\n", MPI_SUCCESS);
		printf("MPI_ERR_COMM: %d\n", MPI_ERR_COMM);
		printf("MPI_ERR_COUNT: %d\n", MPI_ERR_COUNT);
		printf("MPI_ERR_TYPE %d\n", MPI_ERR_TYPE);
		printf("MPI_ERR_BUFFER %d\n", MPI_ERR_BUFFER);
		printf("MPI_ERR_ROOT %d\n", MPI_ERR_ROOT);
	}else{
		// El hilo trabajor recibe el mensaje
		res = MPI_Bcast(buf, cnt, MPI_CHAR, 0, MPI_COMM_WORLD);
		// Se imprime el mensaje recibido
		printf("(%d %d) %s\n", rank, size, buf);
	}
	MPI_Finalize();
	// Se libera el espacio de memoria
	free(buf);
	return 0;
}
