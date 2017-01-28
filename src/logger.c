/**
 * @file logger.c
 * @brief      Implémentation d'un logger.
 * @author     Steven Liatti et Matthieu Constant
 * @bug        Pas de bugs connus
 * @date       Janvier 2017
 * @version    1.0
 */

#include "../inc/logger.h"

/**
 * @brief      Fonction qui écris sur le stream renseigné et selon un niveau de
 *             log fixé, les arguments variables donnés.
 *
 * @param[in]  log_level  Le niveau de log (voir logger.h)
 * @param      stream     Le flux sur lequel écrire (fichier, sortie/erreur
 *                        standard).
 * @param      format     Le string à écrire.
 * @param[in]  <unnamed>  Une liste variable d'arguments (comme pour printf par
 *                        exemple).
 */
void logger(int log_level, FILE *stream, char *format, ...) {
	va_list arguments;
	va_start(arguments, format);

	if (log_level <= LOG_LEVEL)
		vfprintf(stream, format, arguments);
	
	va_end(arguments);
}
