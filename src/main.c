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
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	// Clear the current rendering target with the drawing color.
	SDL_RenderClear(renderer);

	draw_grid();

	// Copies the color buffer to a texture and copies the texture to the current rendering target.
	render_color_buffer();
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
