//. 1. SENCOUNT INDICA EL TAMAÑO A ENVIAR POR PROCESO NO EL TAMAÑO DEL BUFFER QUE TIENEN LA INFORMACION
// 2. RECVOUNT INDICA LA CAPACIDAD A RECIBIR
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
		
strcpy(sendbuf,"Hola Como Estas");
	MPI_Scatter(sendbuf, 1, MPI_CHAR, recvbuf, 2, MPI_CHAR, 0, MPI_COMM_WORLD);
	
	printf("(%d %d) %c\n", rank, size, recvbuf[0]);
	
	MPI_Finalize();

	free(sendbuf);
	free(recvbuf);
	return 0;
}
