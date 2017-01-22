#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#ifndef _MACHINE_H_
#define _MACHINE_H_

typedef struct {
	int value;
	int id;
	int rolling;
} wheel_t;

typedef struct {
	wheel_t** wheel;
	int wheels_nb;
	int cash;
	int started;
	pthread_mutex_t mutex;
} machine_t;

machine_t* new_machine(int size);
void free_machine(machine_t* machine);

#endif
