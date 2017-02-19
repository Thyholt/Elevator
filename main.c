#include "event_manager.h"


int main() {
	if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }
	em_eventMachine();
    	return 0;

}
