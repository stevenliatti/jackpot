/**
 * @file wheel.h
 * @brief      This is the header of the thread wheel.
 *
 * @author     Steven Liatti
 * @author     Orphée Antoniadis
 * @author     Raed Abdennadher
 * @bug        No known bugs.
 * @date       January 29, 2017
 * @version    1.0
 */

#ifndef _WHEEL_H_
#define _WHEEL_H_

#include "machine.h"
#include "control.h"

void* wheel_thread(void* arg);

#endif
