#include "../inc/timer.h"

void* timer_thread(void* arg) {
	sigset_t mask, maskold;
	sigfillset(&mask);
	pthread_sigmask(SIG_SETMASK, &mask, &maskold);

	machine_t* machine = (machine_t*) arg;

	while(!machine->stop_game) {
		pthread_mutex_lock(&(machine->mutex));
		if(!machine->started) {
			pthread_cond_wait(&(machine->cond), &(machine->mutex));
		}
		pthread_mutex_unlock(&(machine->mutex));

		if(machine->started) {
			int wheel_index = 0;
			for (wheel_index = 0; wheel_index < machine->wheels_nb; wheel_index++) {
				sleep(3);
				machine->wheels[wheel_index]->rolling = false;
			}
			machine->started = false;
		}
	}
	return NULL;
}
