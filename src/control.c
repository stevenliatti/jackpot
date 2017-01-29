#include "../inc/control.h"

/**
 * @brief      This function will be called if a user inserts a coin (if SIGTSTP
 *             is send). It sets the booleans of our structure to true and send
 *             a broadcast to all the thread waiting on machine->cond.
 *
 * @param      machine  A pointer on a struct machine_t.
 */
void insert_coin(machine_t* machine) {
	logger(LOG_DEBUG, stderr, "control_thread, In insert_coin\n");
	machine->cash++;
	pthread_mutex_lock(&(machine->mutex));
	machine->new_game = true;
	machine->started = true;
	for (int i = 0; i < machine->wheels_nb; i++) {
		machine->wheels[i]->rolling = true;
		machine->wheels[i]->value = 0;
	}
	pthread_cond_broadcast(&(machine->cond));
	alarm(REACTION_TIME);
	logger(LOG_DEBUG, stderr, "control_thread, insert_coin, alarm set for %d sec\n", REACTION_TIME);
	pthread_mutex_unlock(&(machine->mutex));
	logger(LOG_DEBUG, stderr, "control_thread, all wheel rolling\n");
}

/**
 * @brief      This function will be called if a user stops a wheel. An alarm is
 *             set to REACTION_TIME to stop the current wheel after this time.
 *             It turn the rolling boolean at false and start a new alarm for
 *             the next wheel. If all the wheels are stopped, the machine stop
 *             itself to. It's here where the sleep at the end of game is
 *             called.
 *
 * @param      machine  A pointer on a struct machine_t.
 */
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
			pthread_mutex_lock(&(machine->wheels[cnt]->mutex_value));
			machine->wheels[cnt]->rolling = false;
			pthread_mutex_unlock(&(machine->wheels[cnt]->mutex_value));
			alarm(REACTION_TIME);
			logger(LOG_DEBUG, stderr, "control_thread, stop_wheel, alarm restarted with %d sec\n", REACTION_TIME);
			if (cnt == machine->wheels_nb - 1) {
				machine->started = false;
				alarm(0);
				logger(LOG_DEBUG, stderr, "control_thread, stop_wheel, alarm reset to 0 again (third wheel)\n");
				sleep(END_GAME);
				machine->new_game = false;

				pthread_mutex_lock(&(machine->mutex));
				pthread_cond_broadcast(&(machine->cond));
				pthread_mutex_unlock(&(machine->mutex));
			}
		}
	}
}

/**
 * @brief      This function will be called if a user wants to stop the machine
 *             (if SIGQUIT is send). It sets the booleans of our structure to
 *             true and send a broadcast to all the thread waiting on
 *             machine->cond.
 *
 * @param      machine  A pointer on a struct machine_t.
 */
void exit_game(machine_t* machine) {
	logger(LOG_DEBUG, stderr, "control_thread, In exit_game\n");
	pthread_mutex_lock(&(machine->mutex));
	for (int i = 0; i < machine->wheels_nb; i++) {
		machine->wheels[i]->rolling = true;
	}
	machine->started = true;
	machine->stop_game = true;
	pthread_cond_broadcast(&(machine->cond));
	pthread_mutex_unlock(&(machine->mutex));
	logger(LOG_DEBUG, stderr, "control_thread, Game terminated\n");
}

/**
 * @brief      This function is the control thread. This thread will wait to
 *             receive a signal. When it does, it will call the function related
 *             to the signal or send an error if the signal is incorrect.
 *
 * @param      arg   A void pointer, in reality a pointer on a struct machine_t.
 *
 * @return     NULL
 */
void* control_thread(void* arg) {
	machine_t* machine = (machine_t*) arg;
	sigset_t mask, maskold;
	sigfillset(&mask);
	pthread_sigmask(SIG_SETMASK, &mask, &maskold);

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
