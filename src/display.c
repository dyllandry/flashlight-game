#include "display.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;
int window_width = 800;
int window_height = 600;
int cell_size = 20;
uint32_t white = 0xFFCCCCCC;

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
	for (int curr_y = y; curr_y < y + height; curr_y++) {
		for (int curr_x = x; curr_x < x + width; curr_x++) {
			draw_pixel(curr_x, curr_y, color);
		}
	}
}

void draw_walls(int walls[20][20]) {
	int wall_padding = 2;
	for (int y = 0; y < 20; y++) {
		for (int x = 0; x < 20; x++) {
			if (walls[y][x] == 1) {
				int wall_y = (y * cell_size) + wall_padding;
				int wall_x = (x * cell_size) + wall_padding;
				int wall_size = cell_size - (wall_padding * 2);
				draw_rect(wall_x, wall_y, wall_size, wall_size, white);
			}
		}
	}
}

void draw_finish(vec2_t finish) {

	int padding = 4;
	int x_leg_length = cell_size - (padding * 2);

	// Draw the first leg of the x starting at the top left and moving to the
	// bottom right.
	for (int i = 0; i < x_leg_length; i++) {
		int pixel_x = (finish.x * cell_size) + padding + i;
		int pixel_y = (finish.y * cell_size) + padding + i;
		draw_pixel(pixel_x, pixel_y, white);
	}

	// Draw the second leg of the x starting at the bottom left and moving to
	// the top right.
	for (int i = 0; i < x_leg_length; i++) {
		int pixel_x = (finish.x * cell_size) + padding + i;
		// It's hard for me to explain subtracting 1 from x_leg_length.
		// With a padding of 2 and a cell size of 20, we are going to draw 16
		// pixels per leg of the x with indices 0 to 15. If we are drawing one
		// leg starting at the bottom left corner, we want the y to start at
		// pixel 15. When we compute this, if we add the whole leg length of 16
		// to the start pixel position of 0, we end up at the 16th pixel. This
		// is wrong because we only want to color in 15 pixels from index 0 to
		// 15.
		int pixel_y = (finish.y * cell_size) + padding + (x_leg_length - 1) - i;
		draw_pixel(pixel_x, pixel_y, white);
	}
}

void destroy_window(void) {
	free(color_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

