//. 1. Gather hace algo similar a reduc e, pero lo hace como si fuera 
//	una concatenacion
//	SEND - TAMAÑO DE LO QUE SE ENVIA
//	REC - 	TAMAÑO RECIBIDO POR PROCESOS (NO EL MAXIMO)
#include <stdio.h>
#include <string.h>
// comentario: archivo cabecera de la librer\ia de MPI
#include <mpi.h>
int main(argc,argv)
	int argc;
	char *argv[];
{	
	char *recvbuf = (char *)malloc(20 * sizeof(char));
	char *sendbuf = (char *)malloc(20 * sizeof(char));

	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
		

	sendbuf[0] = '0' + rank / 10;
	sendbuf[1] = '0' + rank % 10;
	MPI_Gather(sendbuf, 2, MPI_CHAR, recvbuf, 2, MPI_CHAR, 0, MPI_COMM_WORLD);
	
	printf("(%d %d) %s\n", rank, size, recvbuf);
	
	MPI_Finalize();

	free(sendbuf);
	free(recvbuf);
	return 0;
}
