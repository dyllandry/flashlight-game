#include "display.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;
int window_width = 800;
int window_height = 600;

bool initialize_window(void) {
	// What bits of hardware do you want to initialize?
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL.\n");
		return false;
	}

	window = SDL_CreateWindow(
		// No window title
		NULL,
		// window X
		SDL_WINDOWPOS_CENTERED,
		// window Y
		SDL_WINDOWPOS_CENTERED,
		// Width
		window_width,
		// Height
		window_height,
		// Extra flags
		SDL_WINDOW_BORDERLESS
	);

	if (!window) {
		fprintf(stderr, "Error creating SDL window.\n");
		return false;
	}

	renderer = SDL_CreateRenderer(
		window,
		// Index of rendering driver to initialize.
		// -1 means first one that supports requested flags
		-1,
		// Renderer Flags
		// 0 is none
		0
	);

	if (!renderer) {
		fprintf(stderr, "Error creating SDL renderer.\n");
		return false;
	}

	return true;
}


void clear_color_buffer(uint32_t color) {
	for (int i = 0; i < window_width * window_height; i++) {
		color_buffer[i] = color;
	}
}

// Copies the color buffer to a texture and copies the texture to the current rendering target.
void render_color_buffer(void) {
	// Update the given texture rectangle with new pixel data.
	SDL_UpdateTexture(
		color_buffer_texture,
		// Optionally used to render just a part of the texture. Think of
		// sprite sheets.
		NULL,
		color_buffer,
		// "Texture pitch" or size of each row in texture.
		(int)(window_width * sizeof(uint32_t))
	);
	// SDL2 docs: "Copy a portion of the texture to the current rendering target."
	// We are copying the color buffer's texture to the rendering target.
	SDL_RenderCopy(
		renderer,
		color_buffer_texture,
		// Next two args are also for rendering just a part of the texture.
		NULL,
		NULL
	);
}

void draw_pixel(int x, int y, uint32_t color) {
	if (x < window_width && y < window_height) {
		color_buffer[(y*window_width)+x] = color;
	}
}

void draw_grid(void) {
	for (int row = 0; row < window_height; row++) {
		for (int col = 0; col < window_width; col++) {
			if (row % 10 == 0 && col % 10 == 0) {
				draw_pixel(col, row, 0xFF555555);
			}
		}
	}
}

void draw_rect(int x, int y, int width, int height, uint32_t color) {
	for (int row = y; row < height; row++) {
		for (int col = x; col < width; col++) {
			draw_pixel(col, row, color);
		}
	}
}

void destroy_window(void) {
	free(color_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

