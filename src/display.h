#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "level.h"
#include "vector.h"

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern uint32_t* color_buffer;
extern SDL_Texture* color_buffer_texture;
extern int window_width;
extern int window_height;

bool initialize_window(void);
void draw_grid(void);
void draw_pixel(int x, int y, uint32_t color);
void draw_rect(int x, int y, int width, int height, uint32_t color);
void draw_line(vec2_t start, vec2_t finish, uint32_t color);
void draw_walls(const int walls[20][20], level_state_t level_state);
void draw_finish(vec2_t finish);
void draw_player(vec2_t player, level_state_t level_state);
void draw_flashlight_charges(level_state_t level_state, level_t level);
void render_color_buffer(void);
void clear_color_buffer(uint32_t color);
void destroy_window(void);

#endif
