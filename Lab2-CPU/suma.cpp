#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    // float h;
    float a;
    float b;
    int n,i;
    float h;
    int cantidadHilos = 4;
    float resultado = 0.0;

    a = 1;
    b = 5;
    n = 10000000;
    h = (b - a) / n;

#   pragma omp parallel for num_threads(cantidadHilos) reduction(+:resultado)
    for (i = 1;i<n-1;++i){
        resultado += fun(a + i*h);
    }
    resultado = resultado*h;
    
    printf("La integral de 1/x es igual a %f\n", resultado);
    return 0;
}