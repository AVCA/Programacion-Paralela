//EJERCICIO
//>> Completa el programa intepar0, que calcula la integral de la funcion f = 1/(x+1) + 1/(x2+1)
//mediante sumas de areas de trapecios bajo la curva que representa la función.
/**********************************************************************************
intepar0.c
Integral de una funcion mediante sumas de areas de trapecios
-- para completar
**********************************************************************************/
#include <mpi.h>
#include <stdio.h>

double t0, t1;

void Leer_datos(double *a_ptr, double *b_ptr, int *n_ptr, int pid, int npr);
double Integrar(double a_loc, double b_loc, int n_loc, double w);
double f(double x);

int main(int argc, char **argv){
    int pid, npr; // Identificador y numero de proc.
    double a, b, w, a_loc, b_loc;
    int n, n_loc, resto, kont;
    double resul, resul_loc; // Resultado de la integral

    MPI_Status status;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &npr);

    // Lectura y distribucion de parametros a todos los procesadores
    Leer_datos(&a, &b, &n, pid, npr); 

    w = (b - a) / n; // cada uno calcula el trozo que tiene que sumar
    n_loc = n / npr;
    resto = n % npr;

    if (pid < resto) n_loc = n_loc + 1;
    a_loc = a + pid * n_loc * w;

    if (pid >= resto) a_loc = a_loc + resto * w;
    b_loc = a_loc + n_loc * w;

    // Calculo de la integral local
    resul_loc = Integrar(a_loc, b_loc, n_loc, w);

    // Sumar resultados parciales
    MPI_Reduce(&resul_loc, &resul, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // Impresion de resultados
    if (pid == 0)
    {
        t1 = MPI_Wtime();
        printf("\n Integral (= ln x+1 + atan x), de %1.1f a %1.1f, %d trap. = %1.12f\n", a, b, n, resul);
        printf(" Tiempo ejecucion (%d pr.) = %1.3f ms \n\n", npr, (t1 - t0) * 1000);
    }
    MPI_Finalize();
    return 0;
} /* main */

// FUNCION Leer_datos
void Leer_datos(double *a_ptr, double *b_ptr, int *n_ptr, int pid, int npr){
    float aux_a, aux_b,buf[3];
    MPI_Status status;
    if (pid == 0)
    {
        printf("\n Introduce a, b (limites) y n (num. de trap.<= 10e8) \n");
        scanf("%f %f %d", &aux_a, &aux_b, n_ptr);
        //(*a_ptr) = (double)aux_a; // a_ptr, b_ptr, n_ptr: punteros a a, b y n
        //(*b_ptr) = (double)aux_b;
        // distribuir datos a todos los procesos
        buf[0] = aux_a;
        buf[1] = aux_b;
        buf[2] = (float) * n_ptr;
    }
    // Distribuyo los valores dentro del arreglo
    MPI_Bcast(&buf, 3, MPI_FLOAT, 0, MPI_COMM_WORLD);
    // Ya que los recibo actualizo las variables mediante
    // su puntero
    (*a_ptr) = (double)(buf[0]);
    (*b_ptr) = (double)(buf[1]);
    (*n_ptr) = (double)(buf[2]);
} /* Leer_datos */

// FUNCION Integrar: calculo local de la integral
double Integrar(double a_loc, double b_loc, int n_loc, double w){
    double resul_loc, x;
    int i;
    // calculo de la integral
    resul_loc = (f(a_loc) + f(b_loc)) / 2.0;
    x = a_loc;
    for (i = 1; i < n_loc; i++)
    {
        x = x + w;
        resul_loc = resul_loc + f(x);
    }
    resul_loc = resul_loc * w;
    return resul_loc;
} /* Integrar */

// FUNCION f: funcion a integrar
double f(double x){
    double y;
    y = 1.0 / (x + 1.0) + 1.0 / (x * x + 1.0) ;
    return y;
}