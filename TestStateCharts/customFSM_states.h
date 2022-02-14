#ifndef CUSTOMFSM_STATES_H_INCLUDED
#define CUSTOMFSM_STATES_H_INCLUDED

#include "TinyStateCharts.h"

enum eEstado
{
    eStateA,
    eStartState = eStateA,
    eStateB,
    eStateC,
    eStateD,

    eNumberOfStates
};

extern const estado_t estados[];

#endif // CUSTOMFSM_STATES_H_INCLUDED