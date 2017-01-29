#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include "logger.h"

#ifndef _MACHINE_H_
#define _MACHINE_H_

/**
 * @brief      120 ms of sleep duration
 */
#define BASE_SLEEP 4000000

/**
 * @brief      Coins initial value
 */
#define COINS 3

typedef struct {
	int value;
	int id;
	bool rolling;
	bool* stop_game;
	int sleep;
	pthread_mutex_t* mutex;
	pthread_cond_t* cond;
} wheel_t;

typedef struct {
	wheel_t** wheels;
	int wheels_nb;
	int cash;
	bool started;
	bool stop_game;
	bool first_game;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
} machine_t;

machine_t* new_machine(int size);
void free_machine(machine_t* machine);

#endif
