#include "state_machine.h"

elev_motor_direction_t state_dir = DIRN_DOWN; //initial state_dir DIRN_DOWN for evaluating next floor for the first time
int state = IDLE; //initial state IDLE
int last_floor; //no initial state, becouse of undefineable
int next_floor;
int last_order[2] = {-1,-1};

void fsm_eventHandler(type_event event, elev_button_type_t BUTTON_TYPE, type_floor FLOOR){ 
	switch (state) {
		case IDLE:
			switch (event) {
			case ORDER:
				queue_save_new_order(BUTTON_TYPE, FLOOR);
				fsm_setOrderLights(BUTTON_TYPE, FLOOR);
				break;
			case REACHEDFLOOR:
			    for(int f=FIRST; f<N_FLOORS; f++){
    			    if(elev_get_floor_sensor_signal()==f){
    			        last_floor = f;
    			        elev_set_floor_indicator(f);
     		    }
    		}

			case TIMEROUT:
				next_floor = queue_determ_next_floor(last_floor,state_dir); 
				if (next_floor == -1){break;}
				if (next_floor < last_floor){
					elev_set_motor_direction(DIRN_DOWN);
					state = RUN;
					state_dir = DIRN_DOWN;
				}
				else if (next_floor > last_floor){
					elev_set_motor_direction(DIRN_UP);
					state = RUN;
					state_dir = DIRN_UP;
				}
				else if (next_floor == last_floor){
						if(elev_get_floor_sensor_signal() == -1){
							if(state_dir == DIRN_UP){
								elev_set_motor_direction(DIRN_DOWN);
				                state = RUN;
				                state_dir = DIRN_DOWN;
								last_floor += 1;
							}
							else if(state_dir == DIRN_DOWN){
								elev_set_motor_direction(DIRN_UP);
				                state = RUN;
				                state_dir = DIRN_UP;
								last_floor -= 1;
							}
						}
						else{
							state = RUN;
						}

				}
				break;
			case STOPPUSHED:
				elev_set_stop_lamp(SETT);
       			elev_set_motor_direction(UNSET);
        		queue_remove_all_orders();
        		for(int n_button = 0; n_button<N_BUTTONS; n_button++){
                	for(int n_floor = FIRST; n_floor < N_FLOORS; n_floor++){
                		fsm_unsetOrderLights(n_button,n_floor);
                	}
                }
        		if(elev_get_floor_sensor_signal() != -1){
            		elev_set_door_open_lamp(SETT);
        		}
        		state = STOP;
				break;
			};
			break;
		case RUN:
			switch (event) {
			case ORDER:
				queue_save_new_order(BUTTON_TYPE, FLOOR);
				fsm_setOrderLights(BUTTON_TYPE, FLOOR);
				break;
			case REACHEDFLOOR:
				    for(int f=FIRST; f<N_FLOORS; f++){
						if(elev_get_floor_sensor_signal()==f){
						    last_floor = f;
						    elev_set_floor_indicator(f);
						    if(queue_determ_next_floor(last_floor, state_dir) == last_floor){
								elev_set_motor_direction(0);
	       				        elev_set_door_open_lamp(1);
	            				state = ATFLOOR;
	            				timer_start();
		    					queue_remove_order(BUTTON_COMMAND,last_floor);
		    					fsm_unsetOrderLights(BUTTON_COMMAND,last_floor);
		    					if(state_dir == DIRN_UP){
		    						if(queue_remove_order(BUTTON_CALL_UP,last_floor) == 1){
		    							last_order[0] = BUTTON_CALL_UP;
		    							last_order[1] = last_floor;
		    							fsm_unsetOrderLights(BUTTON_CALL_UP,last_floor);

		    						}
		    						else{
		    	  						queue_remove_order(BUTTON_CALL_DOWN,last_floor);
		    	  						fsm_unsetOrderLights(BUTTON_CALL_DOWN,last_floor);
		    	  						last_order[0] = BUTTON_CALL_DOWN;
		    							last_order[1] = last_floor;
		    						}
		    					}
		    					else if(state_dir == DIRN_DOWN){
	    							if(queue_remove_order(BUTTON_CALL_DOWN,last_floor) == 1){
	    								fsm_unsetOrderLights(BUTTON_CALL_DOWN,last_floor);
	    								last_order[0] = BUTTON_CALL_DOWN;
		    							last_order[1] = last_floor;
	    							}
	    							else{
		    		 					queue_remove_order(BUTTON_CALL_UP,last_floor);
		    		 					fsm_unsetOrderLights(BUTTON_CALL_UP,last_floor);
		    		 					last_order[0] = BUTTON_CALL_UP;
		    							last_order[1] = last_floor;
									}
	 						   }
	            			}
						}
					}
				break;
			case STOPPUSHED:
				elev_set_stop_lamp(SETT);
       			elev_set_motor_direction(UNSET);
        		queue_remove_all_orders();
        		for(int n_button = 0; n_button<N_BUTTONS; n_button++){
                	for(int n_floor = FIRST; n_floor < N_FLOORS; n_floor++){
                		fsm_unsetOrderLights(n_button,n_floor);
                	}
                 }
        		if(elev_get_floor_sensor_signal() != -1){
            		elev_set_door_open_lamp(SETT);
        		}
        		state = STOP;
				break;
			};
			break;
		case ATFLOOR:
			switch (event) {
			case ORDER:
				if((last_order[0] == BUTTON_TYPE || last_order[0] != BUTTON_COMMAND) && last_order[1] == FLOOR && timer_checker() == 0){
					break;
				}
				else{
				queue_save_new_order(BUTTON_TYPE, FLOOR);
				fsm_setOrderLights(BUTTON_TYPE, FLOOR);
				}
				break;
			case TIMEROUT:
		        elev_set_door_open_lamp(0);
				state = IDLE;
				break;
			case STOPPUSHED:
				elev_set_stop_lamp(SETT);
       			elev_set_motor_direction(UNSET);
        		queue_remove_all_orders();
        		for(int n_button = 0; n_button<N_BUTTONS; n_button++){
                	for(int n_floor = FIRST; n_floor < N_FLOORS; n_floor++){
                		fsm_unsetOrderLights(n_button,n_floor);
                	}
                }
        		if(elev_get_floor_sensor_signal() != -1){
            		elev_set_door_open_lamp(SETT);
        		}
        		state = STOP;
				break;
			};
			break;
		case STOP:
			switch (event) {
			case STOPPUSHED:
				elev_set_stop_lamp(SETT);
       			elev_set_motor_direction(UNSET);
        		queue_remove_all_orders();
        		for(int n_button = 0; n_button<N_BUTTONS; n_button++){
                	for(int n_floor = FIRST; n_floor < N_FLOORS; n_floor++){
                		fsm_unsetOrderLights(n_button,n_floor);
                		}
                }
		        if(elev_get_floor_sensor_signal() != -1){
		        	elev_set_door_open_lamp(SETT);
        		}
        		state = STOP;
				break;
			case STOPRELSD:
			    elev_set_stop_lamp(UNSET);
        		elev_set_door_open_lamp(UNSET);
        		state = IDLE;
				break;
			};
			break;
	};
}

void fsm_setOrderLights(elev_button_type_t BUTTON_TYPE, type_floor FLOOR){
	elev_set_button_lamp(BUTTON_TYPE,FLOOR, SETT);
}

void fsm_unsetOrderLights(elev_button_type_t BUTTON_TYPE, type_floor FLOOR){
	elev_set_button_lamp(BUTTON_TYPE,FLOOR, UNSET);
}
