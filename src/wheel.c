/**
 * @file wheel.c
 * @brief      In this file, there is only the thread function for the wheels.
 *
 * @author     Steven Liatti
 * @author     Orphée Antoniadis
 * @author     Raed Abdennadher
 * @bug        No known bugs.
 * @date       January 29, 2017
 * @version    1.0
 */

#include "../inc/wheel.h"

/**
 * @brief      This function is the wheel thread. There is one thread for one
 *             wheel. Basically, it wait on the start game and rotate the value
 *             assigned (from 0 to 9) until the player stop the wheel or 3
 *             seconds past (calculated in control.c).
 *
 * @param      arg   A void pointer, in reality a pointer on a struct wheel_t
 *
 * @return     NULL
 */
void* wheel_thread(void* arg) {
	sigset_t mask, maskold;
	sigfillset(&mask);
	pthread_sigmask(SIG_SETMASK, &mask, &maskold);

	wheel_t* wheel = (wheel_t*) arg;
	logger(LOG_DEBUG, stderr, "from wheel_thread %d, wheel->stop_game : %d\n", wheel->id, *wheel->stop_game);
	while (!*wheel->stop_game) {
		logger(LOG_DEBUG, stderr, "In wheel_thread %d, after first while\n", wheel->id);
		pthread_mutex_lock(wheel->mutex);
		while (!wheel->rolling) {
			pthread_cond_wait(wheel->cond, wheel->mutex);
		}
		pthread_mutex_unlock(wheel->mutex);
		logger(LOG_DEBUG, stderr, "signal receive in wheel_thread %d\n", wheel->id);

		while (!*wheel->stop_game && wheel->rolling) {
			//pthread_mutex_lock(&(wheel->mutex_value));
			wheel->value = (wheel->value + 1) % 10;
			//pthread_mutex_unlock(&(wheel->mutex_value));
			usleep(wheel->sleep);
		}
		logger(LOG_DEBUG, stderr, "Stop rolling in wheel_thread %d, wheel value : %d\n", wheel->id, wheel->value);
	}
	logger(LOG_DEBUG, stderr, "wheel_thread %d terminate\n", wheel->id);
	return NULL;
}
