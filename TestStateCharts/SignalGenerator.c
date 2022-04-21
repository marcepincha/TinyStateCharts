#include "TinyStateCharts.h"
#include "SignalGenerator.h"

#define GUARDA_HISTORIA 1
#define NO_GUARDA_HISTORIA 0

void evth_PrenderSignalGenerator    (FSM_t* const fsm,  void* const param);
void evth_ApagarSignalGenerator     (FSM_t* const fsm,  void* const param);
void evth_SetearClockInterno     (FSM_t* const fsm,  void* const param);
void evth_SetearClockExterno     (FSM_t* const fsm,  void* const param);

void SG_entryCommon       (FSM_t* const fsm);




/***
    Tabla de manejadores de eventos
***/
const evtHandler_t SG_evtTable[SG_NUM_STATES][SG_NUM_EVENTS] ={
    {NULL,},
    {evth_PrenderSignalGenerator,  NULL,},
    {NULL,               evth_ApagarSignalGenerator,},
    {NULL,               NULL,      NULL,   NULL,   NULL, evth_SetearClockInterno},
    {NULL,               NULL,      NULL,   NULL,   evth_SetearClockExterno, NULL}
};
/*SG_cmd_set_freq,
    SG_cmd_set_amp,
    SG_cmd_set_sync,
    SG_cmd_set_async,
*/
/***
    Arreglo con definición de estados
***/
const estado_t SG_stateTable[SG_NUM_STATES] ={
    [SG_SIGNAL_GENERATOR]={
        .padre          = NULL,
        .hijoDefault    = SG_stateTable+SG_OFF,
        .historia       = NULL,
        .guardaHistoria = NO_GUARDA_HISTORIA,

        .eventHandlers  = SG_evtTable[SG_SIGNAL_GENERATOR],
        .entry          = SG_entryCommon,
        .exit           = NULL,
        .process        = NULL,

        .nombre = "SIGNAL_GENERATOR",
    },
    [SG_OFF]={
        .padre          = SG_stateTable+SG_SIGNAL_GENERATOR,
        .hijoDefault    = NULL,
        .historia       = NULL,
        .guardaHistoria = NO_GUARDA_HISTORIA,

        .eventHandlers  = SG_evtTable[SG_OFF],
        .entry          = SG_entryCommon,
        .exit           = NULL,
        .process        = NULL,

        .nombre = "OFF",
    },
    [SG_ON]={
        .padre          = SG_stateTable+SG_SIGNAL_GENERATOR,
        .hijoDefault    = SG_stateTable+SG_ASYNC,
        .historia       = NULL,
        .guardaHistoria = NO_GUARDA_HISTORIA,

        .eventHandlers  = SG_evtTable[SG_ON],
        .entry          = SG_entryCommon,
        .exit           = NULL,
        .process        = NULL,

        .nombre = "ON",
    },
    [SG_ASYNC]={
        .padre          = SG_stateTable+SG_ON,
        .hijoDefault    = NULL,
        .historia       = NULL,
        .guardaHistoria = NO_GUARDA_HISTORIA,

        .eventHandlers  = SG_evtTable[SG_ASYNC],
        .entry          = SG_entryCommon,
        .exit           = NULL,
        .process        = NULL,

        .nombre = "ON (ASYNC)",
    },
    [SG_SYNC]={
        .padre          = SG_stateTable+SG_ON,
        .hijoDefault    = NULL,
        .historia       = NULL,
        .guardaHistoria = NO_GUARDA_HISTORIA,

        .eventHandlers  = SG_evtTable[SG_SYNC],
        .entry          = SG_entryCommon,
        .exit           = NULL,
        .process        = NULL,

        .nombre = "ON (SYNC)",
    },
};

FSM_t SG_fsm = {.estados=SG_stateTable, .actual=&SG_stateTable[SG_SIGNAL_GENERATOR]};

void evth_PrenderSignalGenerator    (FSM_t* const fsm,  void* const param)
{
    FSM_Transicion(fsm,&SG_stateTable[SG_ON],NULL);
}
void evth_ApagarSignalGenerator     (FSM_t* const fsm,  void* const param)
{
    FSM_Transicion(fsm,&SG_stateTable[SG_OFF],NULL);
}

void evth_SetearClockInterno     (FSM_t* const fsm,  void* const param){
    FSM_Transicion(fsm,&SG_stateTable[SG_ASYNC],NULL);
}
void evth_SetearClockExterno     (FSM_t* const fsm,  void* const param){
    //FSM_Transicion(fsm,&SG_stateTable[SG_SYNC],NULL);
    FSM_Transicion_indirecta(fsm,&SG_stateTable[SG_SYNC],&SG_stateTable[SG_SIGNAL_GENERATOR],NULL);

}



void SG_entryCommon       (FSM_t* const fsm)
{
    printf("Se entró al estado: %s\n",fsm->actual->nombre);
}
