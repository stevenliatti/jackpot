#include "../inc/machine.h"
#include "../inc/control.h"

void* wheel(void* arg) {
	wheel_t* wheel = (wheel_t*) arg;
	// tant que le jeu n'est pas arrêté, à changer ici
	while (1) {
		pthread_mutex_lock(wheel->mutex);
		if (!wheel->rolling)
			pthread_cond_wait(wheel->cond, wheel->mutex);
		pthread_mutex_unlock(wheel->mutex);

		while (wheel->rolling) {
			wheel->value = (wheel->value + 1) % 10;
		}
	}

	return NULL;
}
