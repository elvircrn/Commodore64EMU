#ifndef NESEMU_SCREEN_H
#define NESEMU_SCREEN_H

#include <vector>
#include <algorithm>
#include <array>
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#include "GraphicsConstants.h"
#include "core.h"

class Screen {
	const SDL_Rect screenRectrangle{0, 0, GraphicsConstants::WINDOW_WIDTH, GraphicsConstants::WINDOW_HEIGHT};

	SDL_Texture *texture;
	SDL_Renderer *renderer;
	std::array<u32, GraphicsConstants::WINDOW_WIDTH * GraphicsConstants::WINDOW_HEIGHT> screen{};
	std::array<u32, 16> colors{};

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
			screen[y * GraphicsConstants::WINDOW_WIDTH + x] = colors[1];
		} else {
			screen[y * GraphicsConstants::WINDOW_WIDTH + x] = colors[color];
		}
	}

	void flushTexture() {
		SDL_UpdateTexture(texture, nullptr, screen.data(), GraphicsConstants::WINDOW_WIDTH * sizeof(u32));

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, nullptr, &screenRectrangle);

	}

	void draw() {
		SDL_RenderPresent(renderer);
	}
};

#endif //NESEMU_SCREEN_H