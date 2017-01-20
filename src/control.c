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
	printf("All signals blocked.\n"
	"Use SIGTSTP to insert a coin, SIGINT to stop a wheel and SIGQUIT to quit.\n"
	"reminders :\n"
	"SIGTSTP : CTRL + Z\n"
	"SIGINT : CTRL + C\n"
	"SIGQUIT : CTRL + \\\n"
	"kill -s <SIGNAL> <PID>\n");
	int sig;
	do {
		sigwait(&mask, &sig);
		switch (sig) {
			case SIGTSTP : puts("Coin inserted."); break;
			case SIGINT: puts("Wheel stopped."); break;
			default : puts("Invalid signal."); break;
		}
	} while (sig != SIGQUIT);
	printf("Program terminated.\n");
	return arg;
}
