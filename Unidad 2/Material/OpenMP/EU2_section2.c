int main ()
{
    int thread;
    double timeIni, timeFin;
    timeIni = omp_get_wtime();
    omp_set_num_threads(2);
 
    #pragma omp parallel private(thread)
    {
 
    thread = omp_get_thread_num();
 
    #pragma omp sections
    {
    #pragma omp section
      {
          cout<<"Hebra "<<thread<<" Ejecutando tarea 1"<<endl;
          tarea_uno();
      }
 
    #pragma omp section
      {
          cout<<"Hebra "<<thread<<" Ejecutando tarea 2"<<endl;
          tarea_dos();
      }
 
    }//Fin Sections
 
    }//Fin Parallel
    timeFin = omp_get_wtime();
 
    cout<<"Tiempo tardado = "<< timeFin - timeIni <<" segundos"<<endl;
 
}
----------------------------------------------------------------------------------------------------------------
Si observamos el tiempo de ejecuci�n del programa ha sido igual a la tarea que mas tarda.

Salida
> g++ -fopenmp 04.cpp -o sections
> ./sections 
Hebra 1 Ejecutando tarea 2 
Hebra 0 Ejecutando tarea 1 
Tiempo tardado = 4.00065 segundos

-----------------------------------------------------------------------------------------------------------------
Apartado B
A�adimos la siguiente salida para saber en que instante de tiempo abandonan las hebras el bloque sections.

C�digo
}//Fin Sections
    cout<<"Hebra "<<thread<<" salio del sections en el instante = "<<omp_get_wtime() -
timeIni <<endl;
}//Fin Parallel
Como vemos en la ejecuci�n del c�digo a pesar de que las tareas tienen 2 segundos de diferencia las dos hebras salen de la regi�n sections a la vez. 
OpenMP realiza una sincronizaci�n de las hebras y su variables compartidas.

Salida
Hebra 1 Ejecutando tarea 2
Hebra 0 Ejecutando tarea 1
Hebra 1 salio del sections en el instante = 4.00052
Hebra 0 salio del sections en el instante = 4.00371
Tiempo tardado = 4.00374 segundos

---------------------------------------------------------------------------------------------------------------
Para evitar esa sincronizaci�n a la salida de algunas directivas se ha de a�adir la clausula nowait.

C�digo
#pragma omp sections nowait
Como se observa en la salida, las hebras no se sincronizan al final del sections.

Salida
Hebra 1 Ejecutando tarea 2
Hebra 0 Ejecutando tarea 1
Hebra 0 salio del sections en el instante = 2.00067
Hebra 1 salio del sections en el instante = 4.00065
Tiempo tardado = 4.00092 segundos

-------------------------------------------------------------------------------------------------------------------
Apartado C
Cuando el numero de hebras es superior al numero de section simplemente las hebras sin trabajo se quedan ociosas, 
o si esta presente nowait siguen ejecutando c�digo despu�s del sections.

Salida
Hebra 1 Ejecutando tarea 2
Hebra 0 Ejecutando tarea 1
Hebra 2 salio del sections en el instante = 0.000500483
Hebra 3 salio del sections en el instante = 0.000605175
Hebra 0 salio del sections en el instante = 2.00062
Hebra 1 salio del sections en el instante = 4.00066
Tiempo tardado = 4.00077 segundos

---------------------------------------------------------------------------------------------------------------------------
Apartado D
Cuando el numero de section es superior al numero de hebras, se reparten las section entre las hebras disponibles, 
cuando una de ellas termine de ejecutar una de ellas seguir� por otra, as� hasta que todos los section sean ejecutados por una hebra.

Salida
Hebra 0 Ejecutando tarea 1
Hebra 0 Ejecutando tarea 2
Hebra 0 salio del sections en el instante = 6.00074
Tiempo tardado = 6.00087 segundos