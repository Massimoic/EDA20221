#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int cantidadHilos;
void* Hello(void* rank);

int main(){
    long hilo;
    cantidadHilos = 4;

    pthread_t* handles;
    handles = (pthread_t*)malloc(cantidadHilos*sizeof(pthread_t));

    for(hilo=0; hilo<cantidadHilos; hilo++){
        pthread_create(&handles[hilo], NULL, Hello, (void*)hilo);
    }

    printf("Lanzamos a todos :^)");
    for(hilo=0; hilo<cantidadHilos ; hilo++){
        pthread_join(handles[hilo], NULL);
    }

    free(handles);
    return 0;
}

void* Hello(void* hilo){
    long local = (long) hilo;
    printf("Hola hilo %ld, jaja\n", local);

    return NULL;
}