#include <stdio.h>
#include <string.h>
#include "../TestStateCharts/TinyStateCharts.h"
#include "../Queue/queue.h"
#include "command.h"

///Enumeraci�n de estados
enum eEstado
{
    ESTADO_IDLE,
    eStartState = ESTADO_IDLE,
    ESTADO_PARSING,

    eNumberOfStates
};

///Enumeracion de las se�ales usadas como eventos
enum evtSignals
{
    NEW_CHAR_SIG,
    TICK_10MS_SIG,

    eNumberOfEvents
};


/// cabeceras de los manejadores de eventos
void Idle_NewChar(pEstado_t const this, FSM_t* const fsm, void * param);
void Idle_Tick10ms(pEstado_t const this, FSM_t* const fsm, void * param);
void Parsing_Tick10ms(pEstado_t const this, FSM_t* const fsm, void * param);
void Parsing_Entry(FSM_t * const fsm);

/// tabla de asignaci�n de manejadores de eventos
const evtHandler_t eventos [eNumberOfStates][eNumberOfEvents] =
{
    ///NEW_CHAR_SIG     TICK_10MS_SIG
    { Idle_NewChar,     Idle_Tick10ms,            }, /// ESTADO_IDLE
    { NULL,             Parsing_Tick10ms,           }, /// ESTADO_PARSING
};

/// vector de estados DECLARAR EN EL MISMO ORDEN QUE LA ENUMERACION eEstado
const estado_t estados[eNumberOfStates] =
{
    {
//       ESTADO_IDLE
        .padre = NULL,
        .hijoDefault = NULL,
        .historia = NULL,
        .guardaHistoria = 0,

        .eventHandlers = eventos[ESTADO_IDLE],
        .entry = NULL,
        .exit = NULL,
        .process = NULL
    },
    {
//       ESTADO_PARSING
        .padre = NULL,
        .hijoDefault = NULL,
        .historia = NULL,
        .guardaHistoria = 0,

        .eventHandlers = eventos[ESTADO_PARSING],
        .entry = Parsing_Entry,
        .exit = NULL,
        .process = NULL
    },

};


/// m�quina de estados tipo heredado
FSM_t customFSM =
{
    .estados = (pEstado_t)estados,
    .actual = (pEstado_t)estados+eStartState,
};


////////////////////////////////////////////////////////////////
static const uint16_t TICK_COUNT_TIMEOUT = 1000;
//static const int16_t BUFFER_SIZE = 32;
static const uint8_t END_INPUT_CHAR_CR = '\r';
static const uint8_t END_INPUT_CHAR_LF = '\n';
#define BUFFER_SIZE 32

typedef struct estParserFSM
{
    FSM_t super;
    uint16_t tickCount;
    char buffer[BUFFER_SIZE];
    int16_t buffer_index;
    responseHandler_t responseHandler;
} parserFSM_t;


void buffer_flush(parserFSM_t *fsm)
{
    (*fsm).buffer[0]=0;
    (*fsm).buffer_index = -1;
}


void ParserFSM_init(parserFSM_t *fsm, responseHandler_t responseHandler)
{
    (*fsm).tickCount = 0;
    (*fsm).responseHandler = responseHandler;
    buffer_flush(fsm);
    responseHandler(">");
    (*(FSM_t*)fsm).estados = (pEstado_t)estados;
    (*(FSM_t*)fsm).actual = (pEstado_t)estados+eStartState;

    FSM_init((FSM_t*)fsm);
}

void Idle_NewChar(pEstado_t const this, FSM_t* const fsm, void * param)
{
    char character = *((char*)param);
    (*(parserFSM_t*)fsm).buffer_index ++;
    (*(parserFSM_t*)fsm).tickCount = 0;

    if((character == END_INPUT_CHAR_CR || character == END_INPUT_CHAR_LF) && (*(parserFSM_t*)fsm).buffer_index < BUFFER_SIZE)
    {
        (*(parserFSM_t*)fsm).buffer[(*(parserFSM_t*)fsm).buffer_index]='\0';
        FSM_Transicion(fsm,(pEstado_t)&estados[ESTADO_PARSING],NULL);
        return;
    }
    else if((*(parserFSM_t*)fsm).buffer_index < BUFFER_SIZE-1)
    {
        (*(parserFSM_t*)fsm).buffer[(*(parserFSM_t*)fsm).buffer_index]=character;
        char response[2]={character, 0};
        (*(parserFSM_t*)fsm).responseHandler(response);

        return;
    }
    else
    {
        buffer_flush((parserFSM_t*)fsm);
        //puts("\nNumero maximo de caracteres de un comando excedido\n");
        (*(parserFSM_t*)fsm).responseHandler("\n>");
        return;
    }

}
void Idle_Tick10ms(pEstado_t const this, FSM_t* const fsm, void * param)
{
    (*(parserFSM_t*)fsm).tickCount ++;
    if ( (*(parserFSM_t*)fsm).tickCount >= TICK_COUNT_TIMEOUT)
    {
        (*(parserFSM_t*)fsm).tickCount = 0;
        buffer_flush((parserFSM_t*)fsm);
        //puts("\ntiempo maximo para un comando excedido");
        (*(parserFSM_t*)fsm).responseHandler("\r                                         \r>");
    }
}
void Parsing_Tick10ms(pEstado_t const this, FSM_t* const fsm, void * param)
{
    buffer_flush((parserFSM_t*)fsm);
    (*(parserFSM_t*)fsm).responseHandler("\n>");
    FSM_Transicion(fsm,(pEstado_t)&estados[ESTADO_IDLE],NULL);

}


extern queue_t queue_Commands;
void Parsing_Entry(FSM_t * const fsm)
{

    command_req_t request =
    {
        .responseHandler = ((parserFSM_t*)fsm)->responseHandler,
    };

    if(NULL != CommandRequest_init(&request,
                                   (*(parserFSM_t*)fsm).buffer,
                                   (command_t*)cmdTable,
                                   cmdTableSize))
    {
        queue_write_FIFO(&queue_Commands,&request);
    }
    else
    {
        (*(parserFSM_t*)fsm).responseHandler("\nComando no soportado\n");
    }


}


