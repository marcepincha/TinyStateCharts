#include "TinyStateCharts.h"

void FSM_exitStates(FSM_t * const fsm, pEstado_t const ancestro);
pEstado_t FSM_enterStates(FSM_t * const fsm, pEstado_t const destino, pEstado_t const ancestro);
pEstado_t FSM_getAncestroComun(pEstado_t const origen, pEstado_t const destino);

void FSM_exitStates(FSM_t * const fsm, pEstado_t const ancestro)
{

    //salgo del estado actual desde el nivel m�s anidado al m�s externo. Freno en el ancestro com�n.



    while((*fsm).actual != NULL && (*fsm).actual != ancestro)
    {
        if( (*(*fsm).actual).exit )
            (*(*fsm).actual).exit(fsm);
        (*fsm).actual = (*(*fsm).actual).padre;
    }
}

void FSM_init(FSM_t * const fsm)
{
    FSM_enterStates(fsm, (*fsm).actual,NULL);
}

pEstado_t FSM_enterStates(FSM_t * const fsm,pEstado_t const destino, pEstado_t const ancestro)
{
    pEstado_t pila[MAX_NESTED_LEVELS]= {0};
    uint8_t index = 0;

    /// Considero que al entrar a un estado entro al hijo por default o al estado hist�rico.
    /// Eso ocurre hasta el �ltimo nivel de anidamiento
    /// Dicho hijo ser� el retorno de esta funci�n
    pEstado_t aux = destino;
    while((*aux).hijoDefault != NULL || ((*aux).guardaHistoria && (*aux).historia != NULL) )
    {
        if((*aux).guardaHistoria)
        {
            aux = *((*aux).historia);
        }
        else
        {
            aux = (*aux).hijoDefault;
        }
    }
    pEstado_t hijo = aux;

    /// "pusheo" en una pila los estados desde el nivel de anidamiento m�s bajo hasta
    /// el ancestro com�n
    while(aux != NULL && aux != ancestro)
    {
        //push
        pila[index]=aux;
        index++;

        pEstado_t padre = (*aux).padre;
        if(padre != NULL)
        {
            pEstado_t * historia = (pEstado_t*) (*padre).historia;
            *historia = aux;
        }
        aux = padre;
    }

    /// Ejecuto los entry() de los estados del nivel m�s externo al m�s interno
    while(index)
    {
        //Pop
        index--;
        if( (*pila[index]).entry)
            (*pila[index]).entry(fsm);
        (*fsm).actual = pila[index];


    }
    return hijo;
}

pEstado_t FSM_getAncestroComun(pEstado_t const origen, pEstado_t const destino)
{
    pEstado_t ancestrosOrigen[MAX_NESTED_LEVELS]= {0};
    uint8_t indexO = 0, indexD = 0;

    pEstado_t aux = origen;
    while(aux != NULL)
    {
        ancestrosOrigen[indexO]=aux;
        indexO++;
        aux = (*aux).padre;
    }

    aux = destino;
    while(aux != NULL)
    {
        for(indexD=0; indexD<indexO ; indexD++)
        {
            if(aux == ancestrosOrigen[indexD])
                return aux;
        }

        aux = (*aux).padre;
    }

    return NULL;

}

void FSM_Transicion(FSM_t * const fsm, pEstado_t const destino,callbackAction_t const accion)
{

    /// TODO: ver si combiene hacer camino de entry, sino hay que recorrer dos veces
    pEstado_t ancestro = FSM_getAncestroComun((*fsm).actual,destino);

    FSM_exitStates(fsm, ancestro);

    ///patron template
    if(accion != NULL)
        accion(fsm);

    FSM_enterStates(fsm,destino,ancestro);

    //(*fsm).actual = hijo;

}

void FSM_DispatchEvent(FSM_t *const fsm, const evento_t evento, void* const param)
{
    evtHandler_t *handlersVector;
    pEstado_t aux;

    aux = (*fsm).actual;

    while(aux != NULL)
    {
        handlersVector = (evtHandler_t*)((*aux).eventHandlers);
        if(handlersVector != NULL)
        {
            evtHandler_t handler = handlersVector[evento];
            if(handler != NULL)
            {
                handler((*fsm).actual,fsm,param);
                return;
            }
        }

        aux = (*aux).padre;
    }


}

void FSM_During(FSM_t * const fsm)
{
    pEstado_t aux;

    aux = (*fsm).actual;

    while(aux != NULL)
    {

        if((*aux).process != NULL)
        {
            (*aux).process(fsm);
        }

        aux = (*aux).padre;

    }
}
