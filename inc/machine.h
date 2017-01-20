#include <stdbool.h>

#ifndef _MACHINE_H_
#define _MACHINE_H_

typedef struct {
	int value;
	int id;
	bool rolling;
} wheel_t;

typedef struct {
	wheel_t** wheels;
	int wheels_nb;
	int cash;
} machine_t;

#endif
