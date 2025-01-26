#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vector.h"

bool is_running = false;
level_state_t level_state;
int level_index = 0;

void setup(void) {
	color_buffer = malloc(sizeof(uint32_t) * (window_width * window_height));
	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);
	level_state = create_level_state(levels[level_index]);
}

void process_input(void) {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
		case SDL_QUIT:
			is_running = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				is_running = false;
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_UP || event.key.keysym.scancode == SDL_SCANCODE_W) {
				level_state.player.y--;
				if (level_state.flashlight_on) {
					level_state.flashlight_on = false;
				}
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_DOWN || event.key.keysym.scancode == SDL_SCANCODE_S) {
				level_state.player.y++;
				if (level_state.flashlight_on) {
					level_state.flashlight_on = false;
				}
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_LEFT || event.key.keysym.scancode == SDL_SCANCODE_A) {
				level_state.player.x--;
				if (level_state.flashlight_on) {
					level_state.flashlight_on = false;
				}
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT || event.key.keysym.scancode == SDL_SCANCODE_D) {
				level_state.player.x++;
				if (level_state.flashlight_on) {
					level_state.flashlight_on = false;
				}
			}
			if (
				event.key.keysym.scancode == SDL_SCANCODE_SPACE
				&& !level_state.flashlight_on
				&& level_state.flashlight_charges > 0
			) {
				level_state.flashlight_on = true;
				level_state.flashlight_charges--;
			}
			break;
	}
}

void update(void) {
	level_t level = levels[level_index];

	if ((level_state.player.x != level.start.x || level_state.player.y != level.start.y) && !level_state.player_moved) {
		level_state.player_moved = true;
	}

	bool wallCollision = level.walls[(int)level_state.player.y][(int)level_state.player.x];
	if (wallCollision) {
		level_state = create_level_state(level);
	}

	bool levelFinished = level.finish.x == (int)level_state.player.x && level.finish.y == (int)level_state.player.y;
	if (levelFinished) {
		level_index++;
		if (level_index == sizeof(levels) / sizeof(levels[0])) {
			level_index = 0;
		}
		level_t next_level = levels[level_index];
		level_state = create_level_state(next_level);
	}
}

void render(void) {
	// Clear the current SDL rendering target with the drawing color. This lets
	// us start the frame with a flat color on the screen.
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// Draw a grid on screen for debugging shape sizes.
	draw_grid();

	level_t level = levels[level_index];

	draw_walls(level.walls, level_state);
	draw_finish(level.finish);
	draw_player(level_state.player);
	draw_flashlight_charges(level_state, level);

	// Copies our color buffer to an SDL texture and copies the SDL texture to
	// the current SDL rendering target.
	render_color_buffer();
	// Clear our color buffer so we can start fresh in the next frame.
	clear_color_buffer(0xFF000000);

	// Update the screen with any rendering performed since the previous call.
	SDL_RenderPresent(renderer);
}

int main(void) {
	is_running = initialize_window();

	setup();

	while (is_running) {
		process_input();
		update();
		render();
	}

	destroy_window();

	return 0;
}
