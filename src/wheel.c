#include "../inc/wheel.h"

void* wheel_thread(void* arg) {
	sigset_t mask, maskold;
	sigfillset(&mask);
	pthread_sigmask(SIG_SETMASK, &mask, &maskold);
	
	wheel_t* wheel = (wheel_t*) arg;
	logger(LOG_DEBUG, stderr, "In wheel_thread %d\n", wheel->id);
	logger(LOG_DEBUG, stderr, "from wheel_thread %d, wheel->stop_game : %d\n", wheel->id, *wheel->stop_game);
	while (!*wheel->stop_game) {
		logger(LOG_DEBUG, stderr, "In wheel_thread %d, after first while\n", wheel->id);
		pthread_mutex_lock(wheel->mutex);
		while (!wheel->rolling) {
			pthread_cond_wait(wheel->cond, wheel->mutex);
		}
		pthread_mutex_unlock(wheel->mutex);
		logger(LOG_DEBUG, stderr, "signal receive in wheel_thread %d\n", wheel->id);
		logger(LOG_DEBUG, stderr, "from wheel_thread %d, wheel->stop_game (2) : %d\n", wheel->id, *wheel->stop_game);

		while (!*wheel->stop_game && wheel->rolling) {
			wheel->value = (wheel->value + 1) % 10;
		//	logger(LOG_DEBUG, stderr, "wheel value for id %d : %d\n", wheel->id, wheel->value);
			usleep(wheel->sleep);
		}
		logger(LOG_DEBUG, stderr, "Stop rolling in wheel_thread %d, wheel value : %d\n", wheel->id, wheel->value);
	}
	logger(LOG_DEBUG, stderr, "wheel_thread %d terminate\n", wheel->id);
	return NULL;
}
