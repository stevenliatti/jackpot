#include "../inc/machine.h"
#include "../inc/control.h"

void insert_coin(machine_t* machine) {
	logger(LOG_DEBUG, stderr, "In insert_coin\n");
	if (!(machine->started)) {
		pthread_mutex_lock(&(machine->mutex));
		machine->started = true;
		logger(LOG_DEBUG, stderr, "machine->started = true\n");
		for (int i = 0; i < machine->wheels_nb; i++) {
			machine->wheel[i]->rolling = true;
		}
		pthread_cond_broadcast(&(machine->cond));
		logger(LOG_DEBUG, stderr, "after pthread_cond_broadcast\n");
		pthread_mutex_unlock(&(machine->mutex));
		logger(LOG_DEBUG, stderr, "all wheel rolling\n");
		machine->cash++;
		logger(LOG_DEBUG, stderr, "Coin inserted.\n");
	}
}

void stop_wheel(machine_t* machine) {
	logger(LOG_DEBUG, stderr, "In stop_wheel\n");
	if (machine->started) {
		int cnt = 0;
		while(!(machine->wheel[cnt]->rolling) && cnt < machine->wheels_nb) {
			cnt++;
		} 
		if (cnt != machine->wheels_nb) {
			// pthread_mutex_lock(&(machine->mutex)); à revoir
			machine->wheel[cnt]->rolling = false;
			logger(LOG_DEBUG, stderr, "Wheel stopped.\n");
			if (cnt == machine->wheels_nb - 1) {
				machine->started = false;
			}
			// pthread_mutex_unlock(&(machine->mutex));
		}
	}
}

void exit_game(machine_t* machine) {
	logger(LOG_DEBUG, stderr, "In exit_game\n");
	pthread_mutex_lock(&(machine->mutex));
	// ça parait paradoxal mais faut quand même mettre à true rolling
	// sinon les threads wheel restent bloqué dans le while de la variable de condition
	for (int i = 0; i < machine->wheels_nb; i++) {
		machine->wheel[i]->rolling = true;
	}
	machine->started = false;
	machine->stop_game = true;
	pthread_cond_broadcast(&(machine->cond));
	logger(LOG_DEBUG, stderr, "in exit_game, pthread_cond_broadcast\n");
	pthread_mutex_unlock(&(machine->mutex));
	logger(LOG_DEBUG, stderr, "Game terminated\n");
}

void* control_thread(void* arg) {
	machine_t* machine = (machine_t*) arg;
	sigset_t mask, maskold;
	sigfillset(&mask);
	pthread_sigmask(SIG_SETMASK, &mask, &maskold);
	logger(LOG_DEBUG, stderr, "Use SIGTSTP to insert a coin, SIGINT to stop a wheel "
	"and SIGQUIT to quit.\nreminders :\n"
	"SIGTSTP : CTRL + Z\nSIGINT : CTRL + C\nSIGQUIT : CTRL + \\\n"
	"kill -s <SIGNAL> %d\n", getpid());
	int sig;
	do {
		sigwait(&mask, &sig);
		switch (sig) {
			case SIGTSTP : insert_coin(machine); break;
			case SIGINT : stop_wheel(machine); break;
			case SIGQUIT : exit_game(machine); break;
			default : logger(LOG_DEBUG, stderr, "Invalid signal.\n"); break;
		}
	} while (sig != SIGQUIT);
	return NULL;
}
