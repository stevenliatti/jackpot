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
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <assert.h>

#define CHECK_ERR(expr, msg) if (expr) { fprintf(stderr, "%s\n", msg); return EXIT_FAILURE; }
#define WHEELS_NB 3

/**
 * This is the main function. It initialize variables, check arguments, launch 
 * threads, join them and free the memory in use.
 *
 * @param argc 
 * @param argv
 * @return the code's exit of program
 */
int main(int argc, char** argv) {
	

	worker_t* workers = workers_init(workers_nb, width, height, seed, prob, freq);

	pthread_t t[workers_nb];
	for (int i = 0; i < workers_nb; i++) {
		CHECK_ERR(pthread_create(&t[i], NULL, work, &workers[i]), "pthread_create failed!");
	}
	pthread_t th_display;
	CHECK_ERR(pthread_create(&th_display, NULL, display, workers), "pthread_create failed!");
	pthread_t th_stop;
	CHECK_ERR(pthread_create(&th_stop, NULL, keypress_thread, workers->sync), "pthread_create failed!");

	CHECK_ERR(pthread_join(th_stop, NULL), "pthread_join failed!");
	for (int i = 0; i < workers_nb; i++) {
		CHECK_ERR(pthread_join(t[i], NULL), "pthread_join failed!");
	}
	CHECK_ERR(pthread_join(th_display, NULL), "pthread_join failed!");

	workers_free(workers);
	return EXIT_SUCCESS;
}
