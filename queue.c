#include "queue.h"

void queue_save_new_order(queue_button_type BUTTON_TYPE, int FLOOR){
	queue_matrix[BUTTON_TYPE][FLOOR] = SETT;
}
int queue_remove_order(queue_button_type BUTTON_TYPE, int FLOOR){   
        if(queue_matrix[BUTTON_TYPE][FLOOR] == 1){
            queue_matrix[BUTTON_TYPE][FLOOR] = UNSET;
            return 1;
        }
        else return -1;
}
void queue_remove_all_orders(void){
	for(int n_button = 0; n_button<N_BUTTONS; n_button++){
    	for(int n_floor = FIRST; n_floor < N_FLOORS; n_floor++){
	        if (queue_matrix[n_button][n_floor] != -1){
	            queue_matrix[n_button][n_floor] = UNSET;
	        }
	    }
	}
}
int queue_determ_next_floor(int last_floor, int state_dir){
	int DIRN_UP = 1;
	int DIRN_DOWN = -1;
    if(state_dir == DIRN_UP){
        for(int floor = last_floor; floor < N_FLOORS; floor++){
            if(queue_matrix[QUEUE_BUTTON_CALL_UP][floor] == BUTTON_PUSHED){
                return floor;
            }
            else if(queue_matrix[QUEUE_BUTTON_COMMAND][floor] == BUTTON_PUSHED){
                return floor;
            }
        }
        for(int floor =N_FLOORS-1; floor >= FIRST; floor--){
            if(queue_matrix[QUEUE_BUTTON_CALL_DOWN][floor] == BUTTON_PUSHED || (queue_matrix[QUEUE_BUTTON_COMMAND][floor] == BUTTON_PUSHED && floor < last_floor)){
                return floor;
            }
        }
        for(int floor = FIRST; floor < last_floor; floor++){
            if(queue_matrix[QUEUE_BUTTON_CALL_UP][floor] == BUTTON_PUSHED){
                return floor;
            }
        }
    }
    else if(state_dir == DIRN_DOWN){
        for(int floor = last_floor; floor >= FIRST; floor--){
            if(queue_matrix[QUEUE_BUTTON_CALL_DOWN][floor] == BUTTON_PUSHED){
                return floor;
            }
            else if(queue_matrix[QUEUE_BUTTON_COMMAND][floor] == BUTTON_PUSHED){
                return floor;
            }
        }
        for(int floor = FIRST; floor < N_FLOORS; floor++){
            if(queue_matrix[QUEUE_BUTTON_CALL_UP][floor] == BUTTON_PUSHED || (queue_matrix[QUEUE_BUTTON_COMMAND][floor] == BUTTON_PUSHED && floor > last_floor)){
                return floor;
            }
        }
        for(int floor = last_floor+1; floor < N_FLOORS; floor++){
            if(queue_matrix[QUEUE_BUTTON_CALL_DOWN][floor] == BUTTON_PUSHED){
                return floor;
            }
        }
    }
    return -1;
}