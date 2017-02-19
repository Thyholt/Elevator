#include "timer.h"

static time_t start_time;
double door_opens_seconds = 3.0;

int timer_checker(void) {
	time_t current_time;
	current_time = time(NULL);
	double difference_time = difftime(current_time, start_time);
	if (difference_time < door_opens_seconds) {
		return 0;
	}
	else {
		return 1;
	}
}

void timer_start(void) {
	start_time = time(NULL);
}
