//1. Usar BCAST PARA enviar y recibir
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
	int cnt = 1;
	MPI_Status status;
int res;

	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if( rank == 0 ){	
		res = MPI_Reduce(sendbuf, recvbuf, cnt, MPI_CHAR, MPI_SUM, 0, MPI_COMM_WORLD);
		printf("(%d %d) %d\n", rank, size, recvbuf[0]);
	}else{
		sendbuf[0] = rank;
		printf("(%d %d) %d\n", rank, size, sendbuf[0]);
		res = MPI_Reduce(sendbuf, recvbuf, cnt, MPI_CHAR, MPI_SUM, 0, MPI_COMM_WORLD);
		
	}
	MPI_Finalize();

	free(sendbuf);
	free(recvbuf);
	return 0;
}
