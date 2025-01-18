#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vector.h"

bool is_running = false;

void setup(void) {
	color_buffer = malloc(sizeof(uint32_t) * (window_width * window_height));
	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);
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
			break;
	}
}

void update(void) {
}

void render(void) {
	// Clear the current SDL rendering target with the drawing color. This lets
	// us start the frame with a flat color on the screen.
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// Draw a grid on screen for debugging shape sizes.
	draw_grid();

	draw_walls(level1.walls);

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
