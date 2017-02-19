#ifndef STATE_MACHINE
#define STATE_MACHINE

#include <stdio.h>
#include "queue.h"
#include "elev.h"
#include "timer.h"


typedef enum { IDLE, RUN, ATFLOOR, STOP } type_state;
typedef enum { ORDER, REACHEDFLOOR, TIMEROUT, STOPPUSHED, STOPRELSD } type_event;


void fsm_eventHandler(type_event event, elev_button_type_t BUTTON_TYPE, type_floor FLOOR);
void fsm_setOrderLights(elev_button_type_t BUTTON_TYPE, type_floor FLOOR);
void fsm_unsetOrderLights(elev_button_type_t BUTTON_TYPE, type_floor FLOOR);



#endif
