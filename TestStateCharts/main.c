/** \brief Módulo único, prototipo de lo que será la biblioteca para el manejo e implementación
de StateCharts en C.
 *
 *  10/11/2015
 *
 */




#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include "TinyStateCharts.h"



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

