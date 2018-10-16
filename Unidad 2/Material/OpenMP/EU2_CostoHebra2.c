Apartado A
La dependencia de datos se ha resulto invirtiendo el orden de calculo.

Código
#pragma omp parallel for
for(int i=0; i < N ; i++){
    y[i] = y[i] + i;
    x[i+1] = y[i] * 2;
}


Apartado B
A continuación se muestran los resultados obtenidos de la ejecución del programa en un equipo con dos núcleos.

Salida
introduce el tamaño del vector 100
Numero de hebras = 1

introduce el tamaño del vector 1000
Numero de hebras = 1

introduce el tamaño del vector 10000
Numero de hebras = 1

introduce el tamaño del vector 100000
Numero de hebras = 2
Jabg9607
j
Apartado C
Para comprobar el numero de hebras que se ejecutan se separa la directiva #pragma omp parallel for en dos, #pragma omp parallel y #pragma omp for.
Las hebras creadas en la primera directiva son las usadas en el for, repartiendo las iteraciones como indique la segunda directiva. Para mostrar un mensaje no repetido con el numero de hebras ejecutadas se usara la directiva #pragma omp single, cuyo código solo sera ejecutado por una de las hebras.

Codigo
#pragma omp parallel if( N > 10000 )
{
  #pragma omp single
  cout<<"Numero de hebras = "<<omp_get_num_threads()<<endl;
 
  #pragma omp for
  for(int i=0; i < N ; i++){
      y[i] = y[i] + i;
      x[i+1] = y[i] * 2;
  }
}