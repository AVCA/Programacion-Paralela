// 1. Probar con 5 procesos
// 2. Probar con 20 prcoesos
// 3. Probar con 100 procesos
// 4. Probar con 600 procesos, observar el tiempo que tarda, cuando 
//	son concurrentes todos, deberia consumir 5 segundos unicamente
#include <stdio.h>
// comentario: archivo cabecera de la librer\ia de MPI
#include <mpi.h>
int main(argc, argv)
	int argc;
	char *argv[];
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	sleep(5);
	printf("Hello world from process %d of %d\n", rank, size);
	MPI_Finalize();
	return 0;
}
