/**
 * @file display.c
 * @brief      In this file, there is the management of the game's display.
 *
 * @author     Steven Liatti
 * @author     Orphée Antoniadis
 * @author     Raed Abdennadher
 * @bug        No known bugs.
 * @date       January 29, 2017
 * @version    1.0
 */

#include "../inc/display.h"

void won_coins_compute(machine_t* machine);
void adapt_frequency(struct timespec start, struct timespec finish, double uperiod);
void display_machine(machine_t* machine);

/**
 * @brief      This function is the display thread. It print all the messages of
 *             the game. It executes itself while the game is not stopped.
 *
 * @param      arg   A void pointer, in reality a pointer on a struct macgine_t
 *
 * @return     NULL
 */
void* display_thread(void* arg) {
	sigset_t mask, maskold;
	sigfillset(&mask);
	pthread_sigmask(SIG_SETMASK, &mask, &maskold);
	struct timespec start, finish;

	machine_t* machine = (machine_t*) arg;
	while (!machine->stop_game) {
		logger(LOG_DEBUG, stderr, "In display_thread, before first cond\n");
		pthread_mutex_lock(&(machine->mutex));
		while (machine->new_game) {
			pthread_cond_wait(&(machine->cond), &(machine->mutex));
		}
		pthread_mutex_unlock(&(machine->mutex));

		printf("\e[2J");
		printf("\e[0;0H");
		printf("Insert a coin to start the game...\n");
		printf("\e[2J");

		logger(LOG_DEBUG, stderr, "In display_thread, before second cond\n");
		pthread_mutex_lock(&(machine->mutex));
		while (!machine->started) {
			pthread_cond_wait(&(machine->cond), &(machine->mutex));
		}
		pthread_mutex_unlock(&(machine->mutex));
		logger(LOG_DEBUG, stderr, "display_thread, machine->started : %d\n", machine->started);

		while (machine->started && !machine->stop_game) {
			clock_gettime(CLOCK_MONOTONIC, &start);
			display_machine(machine);
			clock_gettime(CLOCK_MONOTONIC, &finish); 
			adapt_frequency(start, finish, DISPLAY_PERIOD);
		}

		//if (!machine->started) {
			//clock_gettime(CLOCK_MONOTONIC, &start);
			
			display_machine(machine);
			// clock_gettime(CLOCK_MONOTONIC, &finish); 
			// adapt_frequency(start, finish, DISPLAY_PERIOD);
		//}

		logger(LOG_DEBUG, stderr, "display_thread, after while machine started\n");
		if (!machine->stop_game) {
			won_coins_compute(machine);
		}
		logger(LOG_DEBUG, stderr, "display_thread, end of first while, machine->stop_game : %d\n", machine->stop_game);
	}
	printf("Come again soon!\n");
	logger(LOG_DEBUG, stderr, "display_thread terminated\n");
	return NULL;
}

/**
 * @brief      The output and the calculus of the gain at the end game.
 *
 * @param      machine  A pointer on the machine
 */
void won_coins_compute(machine_t* machine) {
	int won_coins = 0;
	if (machine->wheels[0]->value == machine->wheels[1]->value && 
		machine->wheels[1]->value == machine->wheels[2]->value) {
		won_coins = machine->cash / 2;
		if (machine->cash == 1) {
			won_coins = 1;
		}
		printf("JACKPOT!!\n");
	}
	else if (machine->wheels[0]->value == machine->wheels[1]->value || 
			machine->wheels[0]->value == machine->wheels[2]->value || 
			machine->wheels[1]->value == machine->wheels[2]->value) {
		won_coins = 2;
		if (machine->cash == 1) {
			won_coins = 1;
		}
		printf("Double win!\n");
	}
	else {
		printf("You lost!\n");
	}
	
	machine->cash -= won_coins;
	printf("You won %d coins\n", won_coins);
	printf("%d coins left in the machine...\n", machine->cash);
}


/**
 * @brief      Calculation of the real time the thread must sleep (in
 *             microseconds).
 *
 * @param      start    time
 * @param      finish   time
 * @param[in]  uperiod  The uperiod.
 */
void adapt_frequency(struct timespec start, struct timespec finish, double uperiod) {
	double elapsed = (finish.tv_sec - start.tv_sec) * 1e6;
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1e3;
	if (elapsed < uperiod) {
		useconds_t uperiod_sleep = uperiod - elapsed;
		usleep(uperiod_sleep);
	}
}


void display_machine(machine_t* machine) {
	printf("\e[1;1H");
	printf("Game started!\n");
	int pos = 1;
	for (int i = 0; i < machine->wheels_nb; i++) {
		printf("\e[3;%dH", pos);
		printf("%d\n", machine->wheels[i]->value);
		pos += 2;
	}
	printf("\n");
}