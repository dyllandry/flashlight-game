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
	// Set the color used for drawing operations (Rect, Line, and Clear)
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	// Clear the current rendering target with the drawing color.
	SDL_RenderClear(renderer);

	draw_grid();

	draw_rect(0, 0, 40, 80, 0xFF0000FF);

	draw_pixel(20, 20, 0xFF00FF00);

	render_color_buffer();
	clear_color_buffer(0xFF3f446d);

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
