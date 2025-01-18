#ifndef LEVEL_H
#define LEVEL_H

#include "vector.h"

typedef struct {
	int walls [20][20];
	vec2_t start;
	vec2_t finish;
} level_t;

extern level_t level1;

#endif
