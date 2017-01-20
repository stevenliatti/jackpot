#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "../inc/machine.h"
#include "../inc/control.h"

void* control_thread(void* arg) {
	machine_t* machine = (machine_t*)arg;
	sigset_t mask, maskold;
	sigfillset(&mask);
	pthread_sigmask(SIG_SETMASK, &mask, &maskold);
	int sig;
	do {
		sigwait(&mask, &sig);
		switch (sig) {
			case SIGTSTP :
				machine->cash++;
				puts("Coin inserted.");
				break;
			case SIGINT: puts("Wheel stopped."); break;
			case SIGQUIT: puts("Game terminated."); break;
			default : puts("Invalid signal."); break;
		}
	} while (sig != SIGQUIT);
	return NULL;
}
