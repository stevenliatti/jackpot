#include "../inc/display.h"

void* display_thread(void* arg) {
	sigset_t mask, maskold;
	sigfillset(&mask);
	pthread_sigmask(SIG_SETMASK, &mask, &maskold);

	machine_t* machine = (machine_t*) arg;

	//setbuf(stdout,0);
	printf("\e[2J");
	printf("\e[0;0H");
	printf("Insert a coin to start the game...\n");

	while (!machine->stop_game) {

		logger(LOG_DEBUG, stderr, "In display_thread, after first while\n");
		pthread_mutex_lock(&(machine->mutex));
		while (!machine->started) {
			pthread_cond_wait(&(machine->cond), &(machine->mutex));
		}
		pthread_mutex_unlock(&(machine->mutex));

		printf("Game started!\n");
		while (machine->started) {
			int pos = 1;
			for (int i = 0; i < machine->wheels_nb; i++) {
				printf("\e[3;%dH", pos);
				printf("%d\n", machine->wheel[i]->value);
				usleep(33333); // 30 Hz
				pos += 2;
			}
			printf("\n");
		}

		// revoir peut-être la comparaison, faire qqch de plus générique
		// carrément la déplacer dans control en fait, c'est pas le job de display
		if (!machine->started) {
			if (machine->wheel[0]->value == machine->wheel[1]->value && 
				machine->wheel[1]->value == machine->wheel[2]->value) {
				printf("JACKPOT!!\n");
			}
			else if (machine->wheel[0]->value == machine->wheel[1]->value || 
					machine->wheel[0]->value == machine->wheel[2]->value || 
					machine->wheel[1]->value == machine->wheel[2]->value) {
				printf("Double win!\n");
			}
			else {
				printf("You lost!\n");
			}
		}
	}

	printf("Come again soon!\n");
	
	
	return NULL;
}
