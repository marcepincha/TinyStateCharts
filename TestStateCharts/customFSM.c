#include "TinyStateCharts.h"
#include <stdio.h>



void A_entry(FSM_t *fsm);
void B_entry(FSM_t *fsm);
void C_entry(FSM_t *fsm);
void D_entry(FSM_t *fsm);

void A_exit(FSM_t *fsm);
void B_exit(FSM_t *fsm);
void C_exit(FSM_t *fsm);
void D_exit(FSM_t *fsm);

void A_process(FSM_t *fsm);
void B_process(FSM_t *fsm);
void C_process(FSM_t *fsm);
void D_process(FSM_t *fsm);


void A_b(pEstado_t this, FSM_t* fsm,void* param);
void B_a(pEstado_t this, FSM_t* fsm,void* param);
void C_d(pEstado_t this, FSM_t* fsm,void* param);
void D_c(pEstado_t this, FSM_t* fsm,void* param);

void C_Tick10ms(pEstado_t this, FSM_t* fsm,void* param);

void D_cAction(FSM_t *fsm);

pEstado_t estadosHistoria[eNumberOfStates] = {0};

evtHandler_t eventosA[eNumberOfEvents]= {0};
evtHandler_t eventosB[eNumberOfEvents]= {0};
evtHandler_t eventosC[eNumberOfEvents]= {0};
evtHandler_t eventosD[eNumberOfEvents]= {0};




    const estado_t estados[eNumberOfStates] =
    {
        {
//            .id = eStateA,
            .padre = (pEstado_t)estados + eStateD,
            .hijoDefault = NULL,
            .historia = &estadosHistoria[eStateA],
            .guardaHistoria = 0,

            .eventHandlers = eventosA,
            .entry = A_entry,
            .exit = A_exit,
            .process = A_process
        },
        {
//            .id = eStateB,
            .padre = (pEstado_t)estados + eStateD,
            .hijoDefault = NULL,
            .historia = &estadosHistoria[eStateB],
            .guardaHistoria = 0,

            .eventHandlers = eventosB,
            .entry = B_entry,
            .exit = B_exit,
            .process = B_process
        },
        {
//            .id = eStateC,
            .padre = NULL,
            .hijoDefault = NULL,
            .historia = &estadosHistoria[eStateC],
            .guardaHistoria = 0,

            .eventHandlers = eventosC,
            .entry = C_entry,
            .exit = C_exit,
            .process = C_process
        },
        {
//            .id = eStateD,
            .padre = NULL,
            .hijoDefault = (pEstado_t)estados+eStateA,
            .historia = &estadosHistoria[eStateA],
            .guardaHistoria = 1,

            .eventHandlers = eventosD,
            .entry = D_entry,
            .exit = D_exit,
            .process = D_process
        },

    };



    FSM_t customFSM =
    {
        .estados = (pEstado_t)estados,
        .actual = (pEstado_t)estados+eStartState,
    };

    void customFSM_iniciar()
    {
        eventosA[eEvtb]=A_b;
        eventosB[eEvta]=B_a;
        eventosC[eEvtd]=C_d;
        eventosD[eEvtc]=D_c;

        eventosC[eEvtTick10ms]=C_Tick10ms;


        FSM_init(&customFSM);
    }



void A_entry(FSM_t *fsm)
{
    puts("ESTADO A: Entry()");
}

void B_entry(FSM_t *fsm)
{
    puts("ESTADO B: Entry()");
}


uint16_t C_ticks10ms = 0;
void C_entry(FSM_t *fsm)
{
    C_ticks10ms = 0;
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

void C_Tick10ms(pEstado_t this, FSM_t* fsm,void* param)
{
    C_ticks10ms++;
    //timeout a los 5 seg. vuelvo directo a StateA
    if(C_ticks10ms > 500)
    {
        FSM_Transicion(fsm,(fsm->estados)+eStateA,NULL);
    }
}

void D_cAction(FSM_t *fsm)
{
    puts("\tAccion de transición D->C por evento c");
}

void A_process(FSM_t *fsm)
{
    printf("A");
}
void B_process(FSM_t *fsm)
{
    printf("B");
}
void C_process(FSM_t *fsm)
{
    printf("C");
}
void D_process(FSM_t *fsm)
{
    printf("D");
}
