#include "../inc/machine.h"
#include "../inc/control.h"

void insert_coin(machine_t* machine) {
	int size = machine->wheels_nb;
	if (!(machine->started)) {
		machine->started = true;
		for (int i = 0; i < size; i++) {
			machine->wheel[i]->rolling = true;
		}
		machine->cash++;
		printf("Coin inserted.\n");
	}
}

void stop_wheel(machine_t* machine) {
	int size = machine->wheels_nb;
	if (machine->started) {
		int cnt = 0;
		while(!(machine->wheel[cnt]->rolling) && cnt < size) cnt++;
		if (cnt != size) {
			pthread_mutex_lock(&(machine->mutex));
			machine->wheel[cnt]->rolling = false;
			printf("Wheel stopped.\n");
			if (cnt == size - 1) machine->started = false;
			pthread_mutex_unlock(&(machine->mutex));
		}
	}
}

void* control_thread(void* arg) {
	machine_t* machine = (machine_t*)arg;
	sigset_t mask, maskold;
	sigfillset(&mask);
	pthread_sigmask(SIG_SETMASK, &mask, &maskold);
	printf("Use SIGTSTP to insert a coin, SIGINT to stop a wheel "
	"and SIGQUIT to quit.\nreminders :\n"
	"SIGTSTP : CTRL + Z\nSIGINT : CTRL + C\nSIGQUIT : CTRL + \\\n"
	"kill -s <SIGNAL> %d\n", getpid());
	int sig;
	do {
		sigwait(&mask, &sig);
		switch (sig) {
			case SIGTSTP : insert_coin(machine); break;
			case SIGINT : stop_wheel(machine); break;
			case SIGQUIT : printf("Game terminated.\n"); break;
			default : printf("Invalid signal.\n"); break;
		}
	} while (sig != SIGQUIT);
	return NULL;
}
