/**
 * @file jackpot.c
 * @brief      Jackpot
 *
 *             This is Sparta !
 *
 * @author     Steven Liatti
 * @author     Orphée Antoniadis
 * @author     Raed Abdennadher
 * @bug        No known bugs.
 * @date       January 29, 2017
 * @version    1.0
 */

#include "../inc/machine.h"
#include "../inc/control.h"
#include "../inc/display.h"
#include "../inc/wheel.h"
#include "../inc/logger.h"

#define CHECK_ERR(expr, msg) if (expr) { fprintf(stderr, "%s\n", msg); return EXIT_FAILURE; }

const int WHEELS_NB = 3;

/**
 * @brief      This is the main function. It initialize variables, check
 *             arguments, launch threads, join them and free the memory in use.
 *
 * @return     the code's exit of program
 */
int main() {
	machine_t* machine = new_machine(WHEELS_NB);

	// signals in the main thread must be blocked
	sigset_t mask, maskold;
	sigfillset(&mask);
	pthread_sigmask(SIG_SETMASK, &mask, &maskold);

	pthread_t th_control;
	CHECK_ERR(pthread_create(&th_control, NULL, control_thread, machine), "pthread_create failed!");

	// pthread_t th_display;
	// CHECK_ERR(pthread_create(&th_display, NULL, display_thread, machine), "pthread_create failed!");
	
	pthread_t th_wheels[WHEELS_NB];
	for (int i = 0; i < WHEELS_NB; i++) {
		CHECK_ERR(pthread_create(&th_wheels[i], NULL, wheel_thread, machine->wheel[i]), "pthread_create failed!");
	}

	CHECK_ERR(pthread_join(th_control, NULL), "pthread_join failed!");
	logger(LOG_WARNING, stderr, "after th_control join\n");
	// CHECK_ERR(pthread_join(th_display, NULL), "pthread_join failed!");
	// logger(LOG_WARNING, stderr, "after th_display join\n");
	for (int i = 0; i < WHEELS_NB; i++) {
		CHECK_ERR(pthread_join(th_wheels[i], NULL), "pthread_join failed!");
		logger(LOG_WARNING, stderr, "after th_wheels[%d] join\n", i);
	}

	free_machine(machine);
	logger(LOG_WARNING, stderr, "free_machine\n");
	return EXIT_SUCCESS;
}
