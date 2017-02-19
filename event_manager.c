#include "event_manager.h"

void em_eventMachine(){
    elev_set_motor_direction(DIRN_DOWN);
    while (elev_get_floor_sensor_signal() == -1){}
    elev_set_motor_direction(DIRN_STOP);
    printf("Initialization successful! Elevator is in state IDLE in defined floor\n");

	while(1){
		em_checkForNewOrder();
		em_checkStopButton();
		em_checkTimerForNewOrder();
        em_checkForNewFloor();
	}
}

//Check for order elevator and floor buttons pushed
void em_checkForNewOrder() {
	static char order_type_buttons[N_BUTTONS] =
    {BUTTON_CALL_UP, BUTTON_CALL_DOWN, BUTTON_COMMAND};

        for(int r = 0; r<N_BUTTONS; r++)
        {
            for(int c = FIRST; c < N_FLOORS; c++)
            {
                if(elev_get_button_signal(order_type_buttons[r],N_FLOORS-1-c)){
            		fsm_eventHandler(ORDER,order_type_buttons[r],N_FLOORS-1-c);
                }
            }
        }
}

void em_checkStopButton() {
	static int stop_pushed = 0;
    if(elev_get_stop_signal()== 1 && stop_pushed == 0){
        stop_pushed = 1;
        fsm_eventHandler(STOPPUSHED,0,0);}
    else if (elev_get_stop_signal() == 0 && stop_pushed == 1){
        stop_pushed = 0;
        fsm_eventHandler(STOPRELSD,0,0);
    }
	//kaller fsm_eventHandler(STOP)

}
void em_checkTimerForNewOrder() {
	static int timer_status = 0;
    if (timer_checker()==1 && timer_status == 1){
        timer_status = 0;
        fsm_eventHandler(TIMEROUT,0,0);
    }
    else if (timer_checker() == 0 && timer_status == 0){
        timer_status = 1;
	}
    else timer_status = 1;
}

void em_checkForNewFloor() {
    for(int f=FIRST; f<N_FLOORS; f++){
        if(elev_get_floor_sensor_signal()==f){
            fsm_eventHandler(REACHEDFLOOR,0,0);
        }
    }
}