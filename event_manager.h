#ifndef EVENT_MANAGER
#define EVENT_MANAGER

#include <stdio.h>
#include "elev.h"
#include "state_machine.h"
#include "timer.h"


#define N_FLOORS 4
#define N_BUTTONS 3

void em_eventMachine();
void em_checkForNewOrder();
void em_checkStopButton();
void em_checkTimerForNewOrder();
void em_checkForNewFloor();


#endif