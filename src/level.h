#ifndef LEVEL_H
#define LEVEL_H

#include <stdbool.h>
#include "vector.h"

typedef struct {
	int walls [20][20];
	vec2_t start;
	vec2_t finish;
} level_t;

typedef struct {
	vec2_t player;
	bool player_moved;
} level_state_t;

extern level_t level1;

level_state_t create_level_state(level_t level);

#endif
