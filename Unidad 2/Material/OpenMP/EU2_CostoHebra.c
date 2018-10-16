/*
Coste de las hebras

Objetivos
Tomar conocimiento del coste de creación de herbas
Aprender a usar la clausula if()
Aprender a usar #pragma omp single

Explicación
La creación de herbas por parte de OpenMP lleva un coste que para algunos cálculos no es eficiente paralelizar.
Para comparar el coste vamos a realizar el siguiente ejercicio. Tenemos un algoritmo secuencial y se quiere 
hacer de forma paralela. Ten en cuenta que el algoritmo tiene calcula datos dependiendo de otros, 
los cuales imposibilitan que la paralelización sea inmediata.

Código Ejemplo Descarga
A continuación tenemos un código de ejemplo.
*/

#include <omp.h>
#include <iostream>
#include <vector>
using namespace std;
 
int main (){
    unsigned int N;
    cout<<"introduce el tamaño del vector"<<endl;
    cin>>N;
    double timeIni, timeFin;
    vector<int> x(N), y(N);
 
    timeIni = omp_get_wtime();
    //Secuencial
    for(int i=1; i < N ; i++){
    x[i] = y[i-1] * 2;
    y[i] = y[i] + i;
    }
 
    timeFin = omp_get_wtime();
    cout<<"Tiempo tardado secuencial = "<< (timeFin - timeIni)*1000 <<"milisegundos"<<endl;
    timeIni = omp_get_wtime();
    //Paralelo
 
    //Escriba el mismo algoritmo de forma paralela
 
    timeFin = omp_get_wtime();
    cout<<"Tiempo tardado paralelo = "<< (timeFin - timeIni)*1000 <<" milisegundos"<<endl;
}
/*
Ejercicios propuestos
a) Raliza las modificaciones necesarias para que el algoritmo sea paralelizable.
b) Escriba el algoritmo en el espacio del codigo destinado a ello y observa los tiempo de ejecución para tamaño de vector 100,1000,10000,1000000.
c) Añade la clausula if con la condición que creas conveniente.
*/