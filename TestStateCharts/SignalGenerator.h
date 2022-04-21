#ifndef SIGNALGENERATOR_H_INCLUDED
#define SIGNALGENERATOR_H_INCLUDED

enum SG_events{
    SG_cmd_start,
    SG_cmd_stop,
    SG_cmd_set_freq,
    SG_cmd_set_amp,
    SG_cmd_set_sync,
    SG_cmd_set_async,
    SG_NUM_EVENTS
    };

enum SG_states{
    SG_SIGNAL_GENERATOR,
    SG_OFF,
    SG_ON,
    SG_SYNC,
    SG_ASYNC,
    SG_NUM_STATES
    };

#endif // SIGNALGENERATOR_H_INCLUDED
