#ifndef CUSTOMFSM_EVENTS_H_INCLUDED
#define CUSTOMFSM_EVENTS_H_INCLUDED

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
    eEvtTick10ms,

    eNumberOfEvents
};

#endif // CUSTOMFSM_EVENTS_H_INCLUDED
