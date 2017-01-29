#include "../inc/control.h"

void insert_coin(machine_t* machine) {
	logger(LOG_DEBUG, stderr, "control_thread, In insert_coin\n");
	//if (!(machine->started)) {
		pthread_mutex_lock(&(machine->mutex));
		machine->new_game = true;
		machine->started = true;
		logger(LOG_DEBUG, stderr, "control_thread, machine->started = true\n");
		for (int i = 0; i < machine->wheels_nb; i++) {
			machine->wheels[i]->rolling = true;
			machine->wheels[i]->value = 0;
		}
		pthread_cond_broadcast(&(machine->cond));
		logger(LOG_DEBUG, stderr, "control_thread, after pthread_cond_broadcast\n");
		alarm(REACTION_TIME);
		logger(LOG_DEBUG, stderr, "control_thread, insert_coin, alarm set for %d sec\n", REACTION_TIME);
		pthread_mutex_unlock(&(machine->mutex));
	//}
	logger(LOG_DEBUG, stderr, "control_thread, all wheel rolling\n");
	machine->cash++;
	logger(LOG_DEBUG, stderr, "control_thread, Coin inserted.\n");
}

void stop_wheel(machine_t* machine) {
	logger(LOG_DEBUG, stderr, "control_thread, In stop_wheel\n");
	if (machine->started) {
		alarm(0);
		logger(LOG_DEBUG, stderr, "control_thread, stop_wheel, alarm reset to 0\n");
		int cnt = 0;
		while(!(machine->wheels[cnt]->rolling) && cnt < machine->wheels_nb) {
			cnt++;
		} 
		if (cnt != machine->wheels_nb) {
			// pthread_mutex_lock(&(machine->mutex)); // pas nécessaire finalement
			machine->wheels[cnt]->rolling = false;
			alarm(REACTION_TIME);
			logger(LOG_DEBUG, stderr, "control_thread, stop_wheel, alarm restarted with %d sec\n", REACTION_TIME);
			logger(LOG_DEBUG, stderr, "control_thread, Wheel stopped.\n");
			if (cnt == machine->wheels_nb - 1) {

				machine->started = false;
				alarm(0);
				logger(LOG_DEBUG, stderr, "control_thread, stop_wheel, alarm reset to 0 again (third wheel)\n");
				sleep(5);
				machine->new_game = false;

				pthread_mutex_lock(&(machine->mutex));
				pthread_cond_broadcast(&(machine->cond));
				pthread_mutex_unlock(&(machine->mutex));
			}
			// pthread_mutex_unlock(&(machine->mutex)); // pas nécessaire finalement
		}
	}
}

void exit_game(machine_t* machine) {
	logger(LOG_DEBUG, stderr, "control_thread, In exit_game\n");
	pthread_mutex_lock(&(machine->mutex));
	// ça parait paradoxal mais faut quand même mettre à true rolling
	// sinon les threads wheel restent bloqué dans le while de la variable de condition
	for (int i = 0; i < machine->wheels_nb; i++) {
		machine->wheels[i]->rolling = true;
	}
	machine->started = true;
	machine->stop_game = true;
	pthread_cond_broadcast(&(machine->cond));
	logger(LOG_DEBUG, stderr, "control_thread, in exit_game, pthread_cond_broadcast\n");
	pthread_mutex_unlock(&(machine->mutex));
	logger(LOG_DEBUG, stderr, "control_thread, Game terminated\n");
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
			case SIGTSTP: 
				insert_coin(machine);
				break;
			case SIGINT:
			case SIGALRM:
				stop_wheel(machine);
				break;
			case SIGQUIT:
				exit_game(machine);
				break;
			default:
				logger(LOG_DEBUG, stderr, "Invalid signal.\n");
				break;
		}
	} while (sig != SIGQUIT);
	return NULL;
}
