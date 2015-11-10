/** \brief Módulo único, prototipo de lo que será la biblioteca para el manejo e implementación
de StateCharts en C.
 *
 *  10/11/2015
 *
 */




#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>

#define MAX_STATES 1024
#define MAX_NESTED_LEVELS 16
#define MAX_EVENTS 64

enum eEstado;
typedef enum eEstado estadoId_t;

struct estEstado;
typedef struct estEstado estado_t;
typedef struct estEstado *pEstado_t;

enum eEvento;
typedef enum eEvento eventoId_t;

struct estFSM;
typedef struct estFSM FSM_t;


typedef void (*callback_t)(FSM_t * const fsm);
typedef void (*evtHandler_t)(pEstado_t const this, FSM_t* const fsm,void* const param);
typedef void (*callbackAction_t)(FSM_t* const fsm);


enum eEstado
{
    eStateA,
    eStartState = eStateA,
    eStateB,
    eStateC,
    eStateD,

    eNumberOfStates
};

enum eEvento
{
    eEvta,
    eEvtb,
    eEvtc,
    eEvtd,

    eNumberOfEvents
};

struct estEstado
{
    estadoId_t id;
    pEstado_t padre;
    pEstado_t hijoDefault;
    pEstado_t historia;
    uint8_t guardaHistoria;

    evtHandler_t *eventHandlers;
    callback_t entry;
    callback_t exit;
    callback_t process;

};

struct estFSM
{
    pEstado_t actual;
    pEstado_t estados;

};

void A_entry(FSM_t *fsm);
void B_entry(FSM_t *fsm);
void C_entry(FSM_t *fsm);
void D_entry(FSM_t *fsm);

void A_exit(FSM_t *fsm);
void B_exit(FSM_t *fsm);
void C_exit(FSM_t *fsm);
void D_exit(FSM_t *fsm);


void A_b(pEstado_t this, FSM_t* fsm,void* param);
void B_a(pEstado_t this, FSM_t* fsm,void* param);
void C_d(pEstado_t this, FSM_t* fsm,void* param);
void D_c(pEstado_t this, FSM_t* fsm,void* param);

void D_cAction(FSM_t *fsm);

void FSM_exitStates(FSM_t * const fsm, pEstado_t const ancestro);
pEstado_t FSM_enterStates(FSM_t * const fsm, pEstado_t const destino, pEstado_t const ancestro);
pEstado_t FSM_getAncestroComun(pEstado_t const origen, pEstado_t const destino);
void FSM_Transicion(FSM_t * const fsm, pEstado_t const destino,callbackAction_t const accion);
void FSM_DispatchEvent(FSM_t * const fsm, eventoId_t const evento, void* const param);

int main()
{
    estado_t estados[eNumberOfStates] =
    {
        {
            .id = eStateA,
            .padre = estados + eStateD,
            .hijoDefault = NULL,
            .historia = NULL,
            .guardaHistoria = 0,

            .eventHandlers = NULL,
            .entry = A_entry,
            .exit = A_exit,
            .process = NULL
        },
        {
            .id = eStateB,
            .padre = estados + eStateD,
            .hijoDefault = NULL,
            .historia = NULL,
            .guardaHistoria = 0,

            .eventHandlers = NULL,
            .entry = B_entry,
            .exit = B_exit,
            .process = NULL
        },
        {
            .id = eStateC,
            .padre = NULL,
            .hijoDefault = NULL,
            .historia = NULL,
            .guardaHistoria = 0,

            .eventHandlers = NULL,
            .entry = C_entry,
            .exit = C_exit,
            .process = NULL
        },
        {
            .id = eStateD,
            .padre = NULL,
            .hijoDefault = estados+eStateA,
            .historia = estados+eStateA,
            .guardaHistoria = 1,

            .eventHandlers = NULL,
            .entry = D_entry,
            .exit = D_exit,
            .process = NULL
        },

    };

    evtHandler_t eventosA[eNumberOfEvents]= {NULL};
    eventosA[eEvtb]=A_b;
    estados[eStateA].eventHandlers=eventosA;

    evtHandler_t eventosB[eNumberOfEvents]= {NULL};
    eventosB[eEvta]=B_a;
    estados[eStateB].eventHandlers=eventosB;

    evtHandler_t eventosC[eNumberOfEvents]= {NULL};
    eventosC[eEvtd]=C_d;
    estados[eStateC].eventHandlers=eventosC;

    evtHandler_t eventosD[eNumberOfEvents]= {NULL};
    eventosD[eEvtc]=D_c;
    estados[eStateD].eventHandlers=eventosD;

    FSM_t fsm =
    {
        .estados = estados,
        .actual = estados+eStartState,
    };


    FSM_enterStates(&fsm, fsm.actual,NULL);
    puts("---evento b");
    FSM_DispatchEvent(&fsm, eEvtb, NULL);

    puts("---evento b");
    FSM_DispatchEvent(&fsm, eEvtb, NULL);

    puts("---evento a");
    FSM_DispatchEvent(&fsm, eEvta, NULL);

    puts("---evento b");
    FSM_DispatchEvent(&fsm, eEvtb, NULL);

    puts("---evento c");
    FSM_DispatchEvent(&fsm, eEvtc, NULL);

    puts("---evento d");
    FSM_DispatchEvent(&fsm, eEvtd, NULL);


    return 0;
}

void A_entry(FSM_t *fsm)
{
    puts("ESTADO A: Entry()");
}

void B_entry(FSM_t *fsm)
{
    puts("ESTADO B: Entry()");
}

void C_entry(FSM_t *fsm)
{
    puts("ESTADO C: Entry()");
}

void D_entry(FSM_t *fsm)
{
    puts("ESTADO D: Entry()");
}

void A_exit(FSM_t *fsm)
{
    puts("ESTADO A: Exit()");
}

void B_exit(FSM_t *fsm)
{
    puts("ESTADO B: Exit()");
}

void C_exit(FSM_t *fsm)
{
    puts("ESTADO C: Exit()");
}

void D_exit(FSM_t *fsm)
{
    puts("ESTADO D: Exit()");
}

void FSM_exitStates(FSM_t * const fsm, pEstado_t const ancestro)
{

    //salgo del estado actual desde el nivel más anidado al más externo. Freno en el ancestro común.



    while((*fsm).actual != NULL && (*fsm).actual != ancestro)
    {
        (*(*fsm).actual).exit(fsm);
        (*fsm).actual = (*(*fsm).actual).padre;
    }
}

pEstado_t FSM_enterStates(FSM_t * const fsm,pEstado_t const destino, pEstado_t const ancestro)
{
    pEstado_t pila[MAX_NESTED_LEVELS]= {0};
    uint8_t index = 0;

    /// Considero que al entrar a un estado entro al hijo por default o al estado histórico.
    /// Eso ocurre hasta el último nivel de anidamiento
    /// Dicho hijo será el retorno de esta función
    pEstado_t aux = destino;
    while((*aux).hijoDefault != NULL || ((*aux).guardaHistoria && (*aux).historia != NULL) )
    {
        if((*aux).guardaHistoria)
        {
            aux = (*aux).historia;
        }
        else
        {
            aux = (*aux).hijoDefault;
        }
    }
    pEstado_t hijo = aux;

    /// "pusheo" en una pila los estados desde el nivel de anidamiento más bajo hasta
    /// el ancestro común
    while(aux != NULL && aux != ancestro)
    {
        //push
        pila[index]=aux;
        index++;

        pEstado_t padre = (*aux).padre;
        if(padre != NULL)
        {
            (*padre).historia = aux;
        }
        aux = padre;
    }

    /// Ejecuto los entry() de los estados del nivel más externo al más interno
    while(index)
    {
        //Pop
        index--;
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


    pEstado_t ancestro = FSM_getAncestroComun((*fsm).actual,destino);

    FSM_exitStates(fsm, ancestro);

    ///patron template
    if(accion != NULL)
        accion(fsm);

    FSM_enterStates(fsm,destino,ancestro);

    //(*fsm).actual = hijo;

}

void FSM_DispatchEvent(FSM_t *const fsm, const eventoId_t evento, void* const param)
{
    evtHandler_t *handlersVector;
    pEstado_t aux;

    aux = (*fsm).actual;

    while(aux != NULL)
    {
        handlersVector = (*aux).eventHandlers;
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

void A_b(pEstado_t this, FSM_t* fsm,void* param)
{
    //GUARDAS
    FSM_Transicion(fsm,(fsm->estados)+eStateB,NULL);
}

void B_a(pEstado_t this, FSM_t* fsm,void* param)
{
    //GUARDAS
    FSM_Transicion(fsm,(fsm->estados)+eStateA,NULL);
}
void C_d(pEstado_t this, FSM_t* fsm,void* param)
{
    //GUARDAS
    FSM_Transicion(fsm,(fsm->estados)+eStateD,NULL);
}
void D_c(pEstado_t this, FSM_t* fsm,void* param)
{
    //GUARDAS
    FSM_Transicion(fsm,(fsm->estados)+eStateC,D_cAction);
}

void D_cAction(FSM_t *fsm)
{
    puts("\tAccion de transición D->C por evento c");
}

