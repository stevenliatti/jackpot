#include "../inc/machine.h"
#include "../inc/control.h"

void* wheel(void* arg) {
	wheel_t* wheel = (wheel_t*)arg;
	sigset_t mask, maskold;
	sigfillset(&mask);
	pthread_sigmask(SIG_SETMASK, &mask, &maskold);
	int sig;
	do {
		sigwait(&mask, &sig);
		printf("coucou\n");
	} while (sig != SIGQUIT);
	return NULL;
}
