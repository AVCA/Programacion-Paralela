/*
>> envio1 Modifica el programa envio para que devuelva a P0 la suma de los elementos del
vector recibido y éste último imprima el resultado.
*/

#include <mpi.h>
#include <stdio.h>

#define N 10

int main (int argc, char **argv)
{
    int pid, npr, origen, destino, ndat, tag;
    int VA[N], i,resultado;

    MPI_Status info;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &npr);

    for (i = 0; i < N; i++) VA[i] = 0;

    if (pid == 0)
    {
        for (i = 0; i < N; i++) VA[i] = i;
        MPI_Send(&VA[0], N, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&resultado, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &info);
        printf("Suma de los elementos desde P%d: %d\n",pid,resultado);
    }
    else if (pid == 1)
    {
        printf("\n valor de VA en P1 antes de recibir datos\n\n");
        for (i = 0; i < N; i++) printf("%4d", VA[i]);
        printf("\n\n");

        MPI_Recv(&VA[0], N, MPI_INT, 0, 0, MPI_COMM_WORLD, &info);
        MPI_Get_count (&info, MPI_INT, &ndat);

        printf(" Pr 1 recibe VA de Pr %d: tag %d, ndat %d \n\n",info.MPI_SOURCE, info.MPI_TAG, ndat);
        for (i = 0; i < ndat; i++) printf("%4d", VA[i]);
        printf("\n\n");
        for (int i = 0; i < N; ++i)
        {
            resultado+=VA[i];
        }
        MPI_Send(&resultado,1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}