#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

#define TAM_UTIL 4
#define TAM TAM_UTIL+1

void imprimir_arreglo(float* a, int t);

int main()
{
    float arreglo[TAM];
    queue_t cola;

    queue_init(&cola,TAM,sizeof(float),arreglo);
    imprimir_arreglo(arreglo,TAM);

    float dato=1;
    if(queue_write_FIFO(&cola,&dato))
        imprimir_arreglo(arreglo,TAM);
    else
        printf("COLA LLENA\n");

     dato=2;
    if(queue_write_FIFO(&cola,&dato))
        imprimir_arreglo(arreglo,TAM);
    else
        printf("COLA LLENA\n");

     dato=3;
    if(queue_write_FIFO(&cola,&dato))
        imprimir_arreglo(arreglo,TAM);
    else
        printf("COLA LLENA\n");

    dato=5;
    if(queue_write_FIFO(&cola,&dato))
        imprimir_arreglo(arreglo,TAM);
    else
        printf("COLA LLENA\n");

    dato=7;
    if(queue_write_FIFO(&cola,&dato))
        imprimir_arreglo(arreglo,TAM);
    else
        printf("COLA LLENA\n");

    dato=9;
    if(queue_write_FIFO(&cola,&dato))
        imprimir_arreglo(arreglo,TAM);
    else
        printf("COLA LLENA\n");

    if(queue_read(&cola,&dato))
        printf("dato leido: %f",dato);
    else
        printf("COLA VACIA\n");

    dato=11;
    if(queue_write_FIFO(&cola,&dato))
        imprimir_arreglo(arreglo,TAM);
    else
        printf("COLA LLENA\n");


    if(queue_read(&cola,&dato))
        printf("dato leido: %f\n",dato);
    else
        printf("COLA VACIA\n");

    if(queue_read(&cola,&dato))
        printf("dato leido: %f\n",dato);
    else
        printf("COLA VACIA\n");

    if(queue_read(&cola,&dato))
        printf("dato leido: %f\n",dato);
    else
        printf("COLA VACIA\n");

    if(queue_read(&cola,&dato))
        printf("dato leido: %f\n",dato);
    else
        printf("COLA VACIA\n");

    if(queue_read(&cola,&dato))
        printf("dato leido: %f\n",dato);
    else
        printf("COLA VACIA\n");

    if(queue_read(&cola,&dato))
        printf("dato leido: %f\n",dato);
    else
        printf("COLA VACIA\n");


    printf("Hello world!\n");
    return 0;
}

void imprimir_arreglo(float* a, int t)
{
    int i;
    for(i=0;i<t;i++)
        printf("%f ",a[i]);
    printf("\n");
}
