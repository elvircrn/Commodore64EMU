#ifndef NESEMU_SCREEN_H
#define NESEMU_SCREEN_H

#include <vector>
#include <algorithm>
#include <array>
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include "core.h"

class Screen {
	int NUM_COLS = 40;
	int NUM_ROWS = 25;

	u8 BLOCK_SIZE = 0x8u;

	u8 PIXEL_MULTIPLIER = 1;

	/**
	 * Alternatively, one could go about implementing these
	 * constants in the following manner:
	 *
	 * u16 WINDOW_WIDTH = NUM_COLS * PIXEL_MULTIPLIER * BLOCK_SIZE;
	 * u16 WINDOW_HEIGHT = NUM_ROWS * PIXEL_MULTIPLIER * BLOCK_SIZE;
	 */
	static constexpr u16 WINDOW_WIDTH = 403;
	static constexpr u16 WINDOW_HEIGHT = 284;

	SDL_Texture *texture;
	SDL_Renderer *renderer;
	std::array<u32, WINDOW_WIDTH * WINDOW_HEIGHT> screen{};
	std::array<u32, 16> colors;

public:
	Screen(SDL_Texture *_texture, SDL_Renderer *_renderer) : texture(_texture), renderer(_renderer) {
		auto format_ = SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888);
		colors[0]  = SDL_MapRGB(format_, 0x00, 0x00, 0x00);
		colors[1]  = SDL_MapRGB(format_, 0xff, 0xff, 0xff);
		colors[2]  = SDL_MapRGB(format_, 0xab, 0x31, 0x26);
		colors[3]  = SDL_MapRGB(format_, 0x66, 0xda, 0xff);
		colors[4]  = SDL_MapRGB(format_, 0xbb, 0x3f, 0xb8);
		colors[5]  = SDL_MapRGB(format_, 0x55, 0xce, 0x58);
		colors[6]  = SDL_MapRGB(format_, 0x1d, 0x0e, 0x97);
		colors[7]  = SDL_MapRGB(format_, 0xea, 0xf5, 0x7c);
		colors[8]  = SDL_MapRGB(format_, 0xb9, 0x74, 0x18);
		colors[9]  = SDL_MapRGB(format_, 0x78, 0x53, 0x00);
		colors[10] = SDL_MapRGB(format_, 0xdd, 0x93, 0x87);
		colors[11] = SDL_MapRGB(format_, 0x5b, 0x5b, 0x5b);
		colors[12] = SDL_MapRGB(format_, 0x8b, 0x8b, 0x8b);
		colors[13] = SDL_MapRGB(format_, 0xb0, 0xf4, 0xac);
		colors[14] = SDL_MapRGB(format_, 0xaa, 0x9d, 0xef);
		colors[15] = SDL_MapRGB(format_, 0xb8, 0xb8, 0xb8);
	}

	void drawPixel(u32 x, u32 y, u8 data, u32 color) {
		if (data) {
			screen[y * WINDOW_WIDTH + x] = colors[1];
		} else {
			screen[y * WINDOW_WIDTH + x] = colors[color];
		}
	}

	void drawPixels(u32 x, u32 y, const std::vector<u8> &data) {
		u32 id = NUM_COLS * x + y;
		std::copy(data.begin(), data.end(), screen.begin() + id);
	}

	void flushTexture() {
		SDL_UpdateTexture(texture, nullptr, screen.data(), WINDOW_WIDTH * sizeof(u32));
		SDL_Rect texture_rect;
		texture_rect.x = 0;  //the x coordinate
		texture_rect.y = 0; // the y coordinate
		texture_rect.w = 403; //the width of the texture
		texture_rect.h = 284; //the height of the texture

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, nullptr, &texture_rect);

	}

	void draw() {
		SDL_RenderPresent(renderer);
	}
};

#endif //NESEMU_SCREEN_H
