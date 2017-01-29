/**
 * @file machine.h
 * @brief      This is the header of the machine management.
 *
 * @author     Steven Liatti
 * @author     Orphée Antoniadis
 * @author     Raed Abdennadher
 * @bug        No known bugs.
 * @date       January 29, 2017
 * @version    1.0
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include "logger.h"

#ifndef _MACHINE_H_
#define _MACHINE_H_

/**
 * @brief      The wheel's number.
 */
#define WHEELS_NB 3

/**
 * @brief      120 ms of sleep duration.
 */
#define BASE_SLEEP 120000

/**
 * @brief      Coins initial value.
 */
#define COINS 10

/**
 * @brief      Time (in seconds) before a wheel stop by herself.
 */
#define REACTION_TIME 3

/**
 * @brief      Time (in seconds) after a new game.
 */
#define END_GAME 5

/**
 * @brief      The period for a display at the good frequency.
 */
#define DISPLAY_PERIOD 33333

/**
 * @struct wheel_t machine.h
 * @brief      This structure represent a rotative wheel.
 */
typedef struct {
	int value; /*!< The value (from 0 to 9). */
	int id; /*!< The id of the wheel, for the thread basically. */
	bool rolling; /*!< If the wheel is rolling or not. */
	bool* stop_game; /*!< A pointer on the boolean stop_game of the machine_t. */
	int sleep; /*!< The sleep time, to change the value at the wanted speed. */
	pthread_mutex_t* mutex; /*!< A pointer on the machine mutex, work in collaboration with the
	                         * pthread condition. */
	pthread_cond_t* cond; /*!< A pointer on the machine condition, work in collaboration with
	                       * the pthread mutex. */
} wheel_t;

/**
 * @struct machine_t machine.h
 * @brief      This structure represent a game machine.
 */
typedef struct {
	wheel_t** wheels; /*!< The wheels (array of pointers on wheel_t). */
	int wheels_nb; /*!< The wheel's number. */
	int cash; /*!< The cash value in the machine. */
	bool started; /*!< A boolean indicating if the machine is started (after a
	               * insertion of coin). */
	bool stop_game; /*!< A boolean indicating if the machine is stopped (after the all
	                 * wheels are stopped). */
	bool new_game; /*!< The boolean indicating that a coin is inserted and new_game
	                * started. */
	pthread_mutex_t mutex; /*!< A condition, work in collaboration with the pthread mutex. */
	pthread_cond_t cond; /*!< A mutex, work in collaboration with the pthread condition. */
} machine_t;

machine_t* new_machine(int size);
void free_machine(machine_t* machine);

#endif
