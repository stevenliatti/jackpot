#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>

#ifndef _MACHINE_H_
#define _MACHINE_H_

typedef struct {
	int value;
	int id;
	bool rolling;
} wheel_t;

typedef struct {
	wheel_t** wheel;
	int wheels_nb;
	int cash;
} machine_t;

machine_t* new_machine(int size);
void free_machine(machine_t* machine);

#endif
