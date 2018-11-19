//1. Send + Recv
//2. sprintf
//3. Intentar con qsub
//4. Usar envio con -np 2, y -np 3, observar que se queda estatico
//5. crear script .sh
//5.1	mpirun -np 2 ./salida
//6. qsub script.sh
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// comentario: archivo cabecera de la librer\ia de MPI
#include <mpi.h>
//int argc;
//char *argv[];
int main(int argc, char *argv[])
{
	char *buf = (char *)malloc(20 * sizeof(char));
	int cnt = 20;
	MPI_Status status;

	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if( rank == 0 ){
		sprintf(buf, "Hola 1");
		MPI_Send(buf, cnt, MPI_CHAR, 1, 1, MPI_COMM_WORLD);
		sprintf(buf, "Hola 2");
		MPI_Send(buf, cnt, MPI_CHAR, 2, 1, MPI_COMM_WORLD);	
		sprintf(buf, "Hola 3");
		MPI_Send(buf, cnt, MPI_CHAR, 3, 1, MPI_COMM_WORLD);
	}else{		
		MPI_Recv(buf, cnt, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status);
		printf("(%d %d) %s\n", rank, size, buf);
	}
	MPI_Finalize();

	free(buf);
	return 0;
}
