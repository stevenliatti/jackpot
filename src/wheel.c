#include "../inc/machine.h"
#include "../inc/control.h"
#include "../inc/wheel.h"

void* wheel_thread(void* arg) {
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
			usleep(wheel->sleep);
		}
		logger(LOG_DEBUG, stderr, "Stop rolling in wheel_thread %d, wheel value : %d\n", wheel->id, wheel->value);
	}

	return NULL;
}
