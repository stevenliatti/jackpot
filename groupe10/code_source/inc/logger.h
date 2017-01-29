/**
 * @file logger.h
 * @brief      Header of a basic logger.
 *
 * @author     Steven Liatti
 * @author     Orphée Antoniadis
 * @author     Raed Abdennadher
 * @bug        No known bugs.
 * @date       January 29, 2017
 * @version    1.0
 */

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include "machine.h"

/**
 * @brief      The differents log level.
 */
#define LOG_ERROR 0
#define LOG_WARNING 1
#define LOG_DEBUG 2

/**
 * @brief      The actual log level.
 */
#define LOG_LEVEL LOG_ERROR

void logger(int log_level, FILE* stream, char* format, ...);

#endif
