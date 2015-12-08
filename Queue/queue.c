#include "queue.h"
#include <string.h>
#include <stdio.h>

#define DEBUG_QUEUE 0

void queue_print(queue_t*);

void queue_init(queue_t* this, QUEUE_INDEX_TYPE queue_size, uint8_t data_size, void* buffer)
{
    (*this).count = 0;
    (*this).lastIn = 0;
    (*this).firstOut = 0;
    (*this).size = queue_size;
	(*this).sizeData = data_size;
	(*this).buffer = buffer;
}

/////////////////////////////////////////////////////////////////////////
void* queue_write_FIFO(queue_t* this, const void* in_data)
{
    #if DEBUG_QUEUE
    queue_print(this);
    #endif // DEBUG

    ///si esta lleno retorno NULL
    if((*this).count == (*this).size)
        return NULL;

    /*
    if (((*this).lastIn + 1) % (*this).size  == (*this).firstOut)
        return NULL;
    */

    /// si no esta lleno pongo el dato en la cola
    void * destino = (*this).buffer + (*this).sizeData * (*this).lastIn;
    memcpy(destino, in_data, (*this).sizeData);
    (*this).lastIn = ((*this).lastIn + 1) % (*this).size;
    (*this).count ++;

    return destino;
}

//////////////////////////////////////////////////////////////////////////
void* queue_write_LIFO(queue_t* this, const void* in_data)
{
    #if DEBUG_QUEUE
    queue_print(this);
    #endif // DEBUG

    ///si esta lleno retorno NULL
    if((*this).count == (*this).size)
        return NULL;

    /*
    if (((*this).lastIn + 1) % (*this).size  == (*this).firstOut)
        return NULL;
    */


    /// si no esta lleno pongo el dato en la cabeza
    if( (*this).firstOut > 0)
        (*this).firstOut --;
    else
        (*this).firstOut = (*this).size - 1;

    void * destino = (*this).buffer + (*this).firstOut * (*this).sizeData;
    memcpy(destino, in_data, (*this).sizeData);
    (*this).count ++;

    return destino;
}

///////////////////////////////////////////////////////////////////////
void*  queue_read(queue_t* this, void* out_data)
{
    ///si esta vacio retorno NULL
    if((*this).count == 0)
        return NULL;

    /*
    if( (*this).lastIn == (*this).firstOut )
        return NULL;
    */

    /// si no esta vacio saco el dato de la cabeza
    void * origen = (*this).buffer + (*this).firstOut * (*this).sizeData;
    memcpy(out_data, origen, (*this).sizeData);

    (*this).firstOut = ((*this).firstOut + 1) % (*this).size;
    (*this).count --;

    return out_data;

}

///////////////////////////////////////////////////////////////////
void*  queue_peek(queue_t* this, void* out_data)
{
    ///si esta vacio retorno NULL
    if((*this).count == 0)
        return NULL;

    /*
    if( (*this).lastIn == (*this).firstOut )
        return NULL;
    */

    /// si no esta vacio copio el dato de la cabeza
    void * origen = (*this).buffer + (*this).firstOut * (*this).sizeData;
    memcpy(out_data, origen,(*this).sizeData);

    return out_data;
}

void queue_print(queue_t* this)
{
    printf("\n-------------------");
    printf("\nlastIn = %d",(*this).lastIn);
    printf("\nfirstOut = %d",(*this).firstOut);
    printf("\nsize = %d",(*this).size);
    printf("\ncount = %d",(*this).count);
    printf("\n-------------------\n");
}
