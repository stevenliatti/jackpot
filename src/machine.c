#include <stdio.h>
#include <stdlib.h>
#include "../inc/machine.h"

machine_t* new_machine(int size) {
	machine_t* machine = malloc(sizeof(machine_t));
	machine->wheels_nb = size;
	machine->cash = 10;
	machine->wheel = malloc(sizeof(wheel_t*) * size);
	for (int i = 0; i < size; i++) {
		machine->wheel[i] = malloc(sizeof(wheel_t));
		machine->wheel[i]->value = 0;
		machine->wheel[i]->id = i;
		machine->wheel[i]->rolling = false;
	}
	return machine;
}

void free_machine(machine_t* machine) {
	for (int i = 0; i < machine->wheels_nb; i++) {
		free(machine->wheel[i]);
	}
	free(machine->wheel);
	free(machine);
}
