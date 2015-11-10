#ifndef TINYSTATECHARTS_H_
#define TINYSTATECHARTS_H_

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
    pEstado_t const estados;

};

void FSM_exitStates(FSM_t * const fsm, pEstado_t const ancestro);
pEstado_t FSM_enterStates(FSM_t * const fsm, pEstado_t const destino, pEstado_t const ancestro);
pEstado_t FSM_getAncestroComun(pEstado_t const origen, pEstado_t const destino);
void FSM_Transicion(FSM_t * const fsm, pEstado_t const destino,callbackAction_t const accion);
void FSM_DispatchEvent(FSM_t * const fsm, eventoId_t const evento, void* const param);


#endif // TINYSTATECHARTS_H_
