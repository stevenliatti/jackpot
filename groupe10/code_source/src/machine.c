/**
 * @file machine.c
 * @brief      This is the creation and destruction of the machine entity.
 *
 * @author     Steven Liatti
 * @author     Orphée Antoniadis
 * @author     Raed Abdennadher
 * @bug        No known bugs.
 * @date       January 29, 2017
 * @version    1.0
 */

#include "../inc/machine.h"

/**
 * @brief      This function initialize the fields of the machine structure.
 *
 * @param[in]  size  The wheel's number of the machine.
 *
 * @return     A pointer on a machine_t.
 */
machine_t* new_machine(int size) {
	machine_t* machine = malloc(sizeof(machine_t));
	machine->wheels_nb = size;
	machine->cash = COINS;
	machine->started = false;
	machine->stop_game = false;
	machine->new_game = false;
	pthread_mutex_init(&(machine->mutex), NULL);
	pthread_cond_init(&(machine->cond), NULL);
	machine->wheels = malloc(sizeof(wheel_t*) * size);
	int sleep = BASE_SLEEP;
	for (int i = 0; i < size; i++) {
		machine->wheels[i] = malloc(sizeof(wheel_t));
		machine->wheels[i]->value = 0;
		machine->wheels[i]->id = i;
		machine->wheels[i]->rolling = false;
		machine->wheels[i]->stop_game = &(machine->stop_game);
		sleep = i == 0 ? BASE_SLEEP : sleep / 2;
		logger(LOG_DEBUG, stderr, "sleep %d pour wheels %d\n", sleep, machine->wheels[i]->id);
		machine->wheels[i]->sleep = sleep;
		machine->wheels[i]->mutex = &(machine->mutex);
		machine->wheels[i]->cond = &(machine->cond);
	}
	return machine;
}

/**
 * @brief      Free the memory in use by the machine.
 *
 * @param      machine  A pointer on a machine_t.
 */
void free_machine(machine_t* machine) {
	for (int i = 0; i < machine->wheels_nb; i++) {
		free(machine->wheels[i]);
	}
	pthread_mutex_destroy(&(machine->mutex));
	pthread_cond_destroy(&(machine->cond));
	free(machine->wheels);
	free(machine);
}
