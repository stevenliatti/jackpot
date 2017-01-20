/**
 * @file jackpot.c
 * @brief Jackpot
 *
 * This is Sparta !
 *
 * @author Steven Liatti
 * @author Orphée Antoniadis
 * @author Raed Abdennadher
 * @bug No known bugs.
 * @date January 29, 2017
 * @version 1.0
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include "../inc/machine.h"
#include "../inc/control.h"

#define CHECK_ERR(expr, msg) if (expr) { fprintf(stderr, "%s\n", msg); return EXIT_FAILURE; }

const int WHEELS_NB = 3;

/**
 * This is the main function. It initialize variables, check arguments, launch
 * threads, join them and free the memory in use.
 *
 * @param argc
 * @param argv
 * @return the code's exit of program
 */
int main(int argc, char** argv) {
	machine_t* machine = new_machine(WHEELS_NB);

	// pthread_t t[WHEELS_NB];
	// for (int i = 0; i < WHEELS_NB; i++) {
	// 	CHECK_ERR(pthread_create(&t[i], NULL, machine, NULL), "pthread_create failed!");
	// }
	// pthread_t th_display;
	// CHECK_ERR(pthread_create(&th_display, NULL, display, NULL), "pthread_create failed!");

	// signals in the main thread must be blocked
	sigset_t mask, maskold;
	sigfillset(&mask);
	pthread_sigmask(SIG_SETMASK, &mask, &maskold);

	// control thread creation
	pthread_t th_control;
	CHECK_ERR(pthread_create(&th_control, NULL, control_thread, machine), "pthread_create failed!");
	CHECK_ERR(pthread_join(th_control, NULL), "pthread_join failed!");

	// for (int i = 0; i < workers_nb; i++) {
	// 	CHECK_ERR(pthread_join(t[i], NULL), "pthread_join failed!");
	// }
	// CHECK_ERR(pthread_join(th_display, NULL), "pthread_join failed!");

	free_machine(machine);
	return EXIT_SUCCESS;
}
