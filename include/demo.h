#ifndef NESEMU_DEMO_H
#define NESEMU_DEMO_H

#include <cstdio>
#include <cstdlib>
#include <thread>
#include <iostream>
#include <chrono>

#include "boost/filesystem.hpp"
#include <SDL2/SDL.h>

#include "NES.h"
#include "CPU.h"
#include "FileHandler.h"
#include "GUI.h"
#include "MMU.h"
#include "NanoLog.h"
#include "Clock.h"

void putPixelRGB(SDL_Renderer *renderer, int x, int y, unsigned char r, unsigned char g, unsigned char b) {
	SDL_SetRenderDrawColor(renderer, (Uint8) r, (Uint8) g, (Uint8) b, 255);
	SDL_RenderDrawPoint(renderer, x, y);
}

void _patternTablesDemo(const PPU &ppu) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
	}

	constexpr u8 PIXEL_DIM = 3, TILE_CNT = 128, PATTERN_CNT = 2;
	constexpr u16 PATTERN_PIXEL_WIDTH = PIXEL_DIM * TILE_CNT,
			PATTERN_PIXEL_HEIGHT = PIXEL_DIM * TILE_CNT;
	constexpr u16 WINDOW_WIDTH = PIXEL_DIM * TILE_CNT * PATTERN_CNT,
			WINDOW_HEIGTH = PIXEL_DIM * TILE_CNT;
	auto window = sdl2::make_window("Pattern tables", 0, 0, WINDOW_WIDTH, WINDOW_HEIGTH, 0);
	auto renderer = sdl2::make_renderer(window.get(), -1, 0);
	auto texture =
			sdl2::make_bmp(renderer.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGTH);

	SDL_Event event;
	for (;;) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT)
			break;

		// Make the screen black
		SDL_SetRenderDrawColor(renderer.get(), 0x00, 0x00, 0x00, 0x00);


		// Set a texture as the current rendering target
		SDL_SetRenderTarget(renderer.get(), texture.get());

		// Should this be set to false, the screen would slowly get
		// filled with red rectangles
		SDL_RenderClear(renderer.get());

		for (int patternId = 0; patternId < PATTERN_CNT; patternId++) {
			auto data = ppu.getPatternTable(static_cast<u16>(0x1000 * patternId));
			for (size_t i = 0; i < data.size(); i++) {
				for (size_t j = 0; j < data[0].size(); j++) {
					if (data[j][i]) {
						for (size_t k = 0; k < PIXEL_DIM; k++) {
							for (size_t l = 0; l < PIXEL_DIM; l++) {
								putPixelRGB(renderer.get(),
														static_cast<int>(patternId * PATTERN_PIXEL_WIDTH + PIXEL_DIM * i + k),
														static_cast<int>(PIXEL_DIM * j + l), 0xff, 0xff, 0xff);
							}
						}
					}
				}
			}
		}

		SDL_SetRenderTarget(renderer.get(), nullptr);

		// SDL_RenderCopy is responsible for making the gameloop understand that there's
		// something that wants to be rendered, inside the parentheses are (the renderer's name,
		// the name of the texture, the area it wants to crop but you can leave this as NULL
		// if you don't want the texture to be cropped, and the texture's rect).
		SDL_RenderCopy(renderer.get(), texture.get(), nullptr, nullptr);

		SDL_RenderPresent(renderer.get());
	}
}

void patternTablesDemo() {
	SDL_Init(SDL_INIT_VIDEO);

	std::string fileName = "donkeykong.nes";
	std::string filePath = boost::filesystem::path(__FILE__)
			.parent_path()
			.parent_path()
			.append(fileName)
			.string();

	std::cout << "Trying to load: " << filePath << '\n';
	std::ifstream file(filePath, std::ios::binary);

	if (!file) {
		std::cout << "Failed to read rom given: " << filePath << '\n';
		return;
	}

	file.unsetf(std::ios::skipws);

	ROM rom(file);
	PPU ppu(rom);

	auto data = ppu.getPatternTable(0x0000);


	for (const auto &row : data) {
		for (const auto &d : row) {
			std::cout << (int) d << ' ';
		}
		std::cout << '\n';
	}

	_patternTablesDemo(ppu);
}

#endif //NESEMU_DEMO_H
