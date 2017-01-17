#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main (int argc, char** argv) {
	sigset_t mask, maskold;
	sigemptyset(&mask);
	sigfillset(&mask);
	pthread_sigmask(SIG_SETMASK, &mask, &maskold);
	puts("All signals blocked.");
	puts("Use SIGTSTP to insert a coin, SIGINT to stop a wheel and SIGQUIT to quit.");
	puts("reminders :");
	puts("SIGSTP : CTRL + Z");
	puts("SIGINT : CTRL + C");
	puts("SIGINT : CTRL + \\");
	puts("kill -s <SIGNAL> <PID>");
	int sig;
	do {
		sigwait(&mask, &sig);
		switch (sig) {
			case SIGTSTP : puts("Coin inserted."); break;
			case SIGINT: puts("Wheel stopped."); break;
			default : puts("Invalid signal."); break;
		}
	} while (sig != SIGQUIT);
	puts("Program terminated.");
   return EXIT_SUCCESS;
}
