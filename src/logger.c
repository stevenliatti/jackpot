/**
 * @file logger.c
 * @brief      Implementation of a basic logger.
 *
 * @author     Steven Liatti
 * @author     Orphée Antoniadis
 * @author     Raed Abdennadher
 * @bug        No known bugs.
 * @date       January 29, 2017
 * @version    1.0
 */

#include "../inc/logger.h"

/**
 * @brief      This function write on the stream in argument and with a log
 *             level the variables arguments.
 *
 * @param[in]  log_level  The log level (see logger.h)
 * @param      stream     The stream (file, stdout/stderr).
 * @param      format     The string to write.
 * @param[in]  <unnamed>  A variable list of arguments.
 */
void logger(int log_level, FILE* stream, char* format, ...) {
	va_list arguments;
	va_start(arguments, format);

	if (log_level <= LOG_LEVEL)
		vfprintf(stream, format, arguments);
	
	va_end(arguments);
}
