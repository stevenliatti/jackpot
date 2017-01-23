#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>


#define CHECK_ERR(expr, msg) if (expr) { fprintf(stderr, "%s\n", msg); return EXIT_FAILURE; }

bool program_stopped = false;
bool wheel_stopped[3] = {false, false, false};
int counters[3] = {0, 0, 0};
//double speed[3] = {120000, 60000, 30000};	//comme dans l'énoncé
double speed[3] = {1000000, 600000, 300000}; // pour que ça se voit dans le test
pthread_mutex_t mutexes[3];

void* dispaly_thread(void* arg) {
	
	sigset_t mask, maskold;
	sigfillset(&mask);
	pthread_sigmask(SIG_SETMASK, &mask, &maskold);

	printf("\e[2J");
	printf("\e[0;0H");
	printf("Game started!\n");

	while (!program_stopped) {
		int pos = 1;
		for (int i = 0; i < 3; ++i)
		{
			printf("\e[3;%dH", pos);
			printf("%d\n", counters[i]);
			usleep(33333); // 30 Hz
			pos += 2;
		}
		printf("\n");
	}

	if (program_stopped) {
		if (counters[0] == counters[1] && counters[1] == counters[2]) {
			printf("JACKPOT!!\n");
		} else if (counters[0] == counters[1] || counters[0] == counters[2] || counters[1] == counters[2]) {
			printf("Double win!\n");
		} else {
			printf("Yous lost!\n");
		}
		printf("Come again soon!\n");
	}
	
	return NULL;
}

void* control_thread(void* arg) {
	
	sigset_t mask, maskold;
	sigfillset(&mask);
	pthread_sigmask(SIG_SETMASK, &mask, &maskold);

	int sig;
	int wheel_stopped_index = 0;
	do {
		sigwait(&mask, &sig);
		if (sig == SIGINT) {
			wheel_stopped[wheel_stopped_index] = true;
			wheel_stopped_index++;
		}
		if (wheel_stopped_index == 3){
			break;
		}
		// switch (sig) {
		// 	case SIGTSTP : puts("Coin inserted."); break;
		// 	case SIGINT: puts("Wheel stopped."); break;
		// 	default : puts("Invalid signal."); break;
		// }
	} while (sig != SIGQUIT);
	program_stopped = true;	
	return NULL;
}

void* play_thread(void* arg) {

	int* index = (int*) arg;
	
	sigset_t mask, maskold;
	sigfillset(&mask);
	pthread_sigmask(SIG_SETMASK, &mask, &maskold);

	while (!program_stopped && !wheel_stopped[*index]) {
		pthread_mutex_lock(&mutexes[*index]);
		counters[*index]++;
		if (counters[*index] == 10) {
			counters[*index] = 0;
		}
		pthread_mutex_unlock(&mutexes[*index]);
		usleep(speed[*index]);
	}

	return NULL;
}



int main (int argc, char** argv) {
	sigset_t mask, maskold;
	sigfillset(&mask);
	pthread_sigmask(SIG_SETMASK, &mask, &maskold);

	for (int i = 0; i < 3; ++i)	{
		pthread_mutex_init(&(mutexes[i]), NULL);
	}

	pthread_t th_dispaly;
	CHECK_ERR(pthread_create(&th_dispaly, NULL, dispaly_thread, NULL), "th_dispaly pthread_create failed!");

	pthread_t th_control;
	CHECK_ERR(pthread_create(&th_control, NULL, control_thread, NULL), "th_control pthread_create failed!");
	
	pthread_t th_play[3];
	int indexes[3] = {0, 1, 2};
	for (int i = 0; i < 3; ++i)	{
		CHECK_ERR(pthread_create(&th_play[i], NULL, play_thread, &indexes[i]), "th_play pthread_create failed!");
	}
	



	CHECK_ERR(pthread_join(th_dispaly, NULL), "th_dispaly pthread_join failed!");
	CHECK_ERR(pthread_join(th_control, NULL), "th_control pthread_join failed!");
	for (int i = 0; i < 3; ++i)	{
		CHECK_ERR(pthread_join(th_play[i], NULL), "th_play pthread_join failed!");
	}
	
	return EXIT_SUCCESS;
}
