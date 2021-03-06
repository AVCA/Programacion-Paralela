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
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if( rank == 0 ){
		sprintf(buf, "Hola 1 y 2");
		res = MPI_Bcast(buf, cnt, MPI_CHAR, 0, MPI_COMM_WORLD);

		printf("res -) %d\n", res);

		printf("MPI_SUCCESS -) %d\n", MPI_SUCCESS);
		printf("MPI_ERR_COMM -) %d\n", MPI_ERR_COMM);
		printf("MPI_ERR_COUNT -) %d\n", MPI_ERR_COUNT);
		printf("MPI_ERR_TYPE -) %d\n", MPI_ERR_TYPE);
		printf("MPI_ERR_BUFFER -) %d\n", MPI_ERR_BUFFER);
		printf("MPI_ERR_ROOT -) %d\n", MPI_ERR_ROOT);
	}else{
		
		res = MPI_Bcast(buf, cnt, MPI_CHAR, 0, MPI_COMM_WORLD);
		printf("(%d %d) %s\n", rank, size, buf);
	}
	MPI_Finalize();

	free(buf);
	return 0;
}
