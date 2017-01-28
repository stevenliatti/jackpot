#include "../inc/machine.h"

machine_t* new_machine(int size) {
	machine_t* machine = malloc(sizeof(machine_t));
	machine->wheels_nb = size;
	machine->cash = 10;
	machine->started = false;
	machine->stop_game = false;
	pthread_mutex_init(&(machine->mutex), NULL);
	pthread_cond_init(&(machine->cond), NULL);
	machine->wheel = malloc(sizeof(wheel_t*) * size);
	int sleep = BASE_SLEEP;
	for (int i = 0; i < size; i++) {
		sleep = i == 0 ? BASE_SLEEP : sleep / 2;
		machine->wheel[i] = malloc(sizeof(wheel_t));
		machine->wheel[i]->value = 0;
		machine->wheel[i]->id = i;
		machine->wheel[i]->rolling = false;
		machine->wheel[i]->stop_game = &(machine->stop_game);
		machine->wheel[i]->sleep = sleep;
		machine->wheel[i]->mutex = &(machine->mutex);
		machine->wheel[i]->cond = &(machine->cond);
	}
	return machine;
}

void free_machine(machine_t* machine) {
	for (int i = 0; i < machine->wheels_nb; i++) {
		free(machine->wheel[i]);
	}
	pthread_mutex_destroy(&(machine->mutex));
	pthread_cond_destroy(&(machine->cond));
	free(machine->wheel);
	free(machine);
}
