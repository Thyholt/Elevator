#ifndef QUEUE
#define QUEUE

#include <stdio.h>

#define N_FLOORS 4
#define N_BUTTONS 3
#define UNSET 0
#define SETT 1
#define BUTTON_PUSHED 1

typedef enum tag_type_floor {
  FIRST,
  SECOND,
  THIRD,
  FOURTH 
} type_floor;

typedef enum tag_queue_button_type{
    QUEUE_BUTTON_CALL_UP = 0,
    QUEUE_BUTTON_CALL_DOWN = 1,
    QUEUE_BUTTON_COMMAND = 2
} queue_button_type;

static int queue_matrix[N_BUTTONS][N_FLOORS] =
/*FIRST SECOND THIRD FOURTH*/
{ { 0,     0,      0,     -1 }, //|UP|
{ -1,     0,      0,      0 }, //|DOWN|
{ 0,     0,      0,      0 } }; //|CMD|


void queue_save_new_order(queue_button_type BUTTON_TYPE, int FLOOR);
int queue_remove_order(queue_button_type BUTTON_TYPE, int FLOOR);
void queue_remove_all_orders(void);

//last_floor elevators last floor, state_dir elevators last direction 
int queue_determ_next_floor(int last_floor, int state_dir); 

#endif
