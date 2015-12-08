#ifndef PARSERFSM_H_
#define PARSERFSM_H_

enum evtSignals
{
    NEW_CHAR_SIG,
    TICK_10MS_SIG,

    eNumberOfEvents
};

#define BUFFER_SIZE 32

typedef struct estParserFSM{
    FSM_t super;
    uint16_t tickCount;
    char buffer[BUFFER_SIZE];
    int16_t buffer_index;
}parserFSM_t;

void ParserFSM_init(parserFSM_t *fsm);

#endif // PARSERFSM_H_
