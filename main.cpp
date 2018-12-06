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

void textureDemo() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
	}

	auto window = sdl2::make_window("Texture Demo", 0, 0, 1024, 768, 0);
	auto renderer = sdl2::make_renderer(window.get(), -1, 0);
	auto texture = sdl2::make_bmp(renderer.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1024, 768);

	SDL_Event event;
	SDL_Rect r{0, 0, 100, 50};

	for (;;) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT)
			break;

		// Make the screen black
		SDL_SetRenderDrawColor(renderer.get(), 0x00, 0x00, 0x00, 0x00);

		r.x = std::rand() % 500;
		r.y = std::rand() % 500;

		// Set a texture as the current rendering target
		SDL_SetRenderTarget(renderer.get(), texture.get());

		// Should this be set to false, the screen would slowly get
		// filled with red rectangles
		SDL_RenderClear(renderer.get());


		// Render a random rectangle
		SDL_RenderDrawRect(renderer.get(), &r);
		SDL_SetRenderDrawColor(renderer.get(), 0xFF, 0x00, 0x00, 0x00);
		SDL_RenderFillRect(renderer.get(), &r);

		SDL_SetRenderTarget(renderer.get(), nullptr);

		// SDL_RenderCopy is responsible for making the gameloop understand that there's
		// something that wants to be rendered, inside the parentheses are (the renderer's name,
		// the name of the texture, the area it wants to crop but you can leave this as NULL
		// if you don't want the texture to be cropped, and the texture's rect).
		SDL_RenderCopy(renderer.get(), texture.get(), nullptr, nullptr);

		SDL_RenderPresent(renderer.get());
	}
}

void putPixelRGB(SDL_Renderer *renderer, int x, int y, unsigned char r, unsigned char g, unsigned char b) {
	SDL_SetRenderDrawColor(renderer, (Uint8) r, (Uint8) g, (Uint8) b, 255);
	SDL_RenderDrawPoint(renderer, x, y);
}

void textureDemo(PPU &ppu) {
	static int offX{}, offY{};
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
	}

	auto window = sdl2::make_window("Texture Demo1", 0, 0, 1024, 768, 0);
	auto renderer = sdl2::make_renderer(window.get(), -1, 0);
	auto texture = sdl2::make_bmp(renderer.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1024, 768);

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

		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
			case SDLK_LEFT: offX--;
				break;
			case SDLK_RIGHT: offX++;
				break;
			case SDLK_UP: offY--;
				break;
			case SDLK_DOWN: offY++;
				break;
			default: break;
			}
		}

		auto data = ppu.Pattern(offX, offY);

		for (size_t i = 0; i < data.size(); i++) {
			for (size_t j = 0; j < data[0].size(); j++) {
				if (data[i][j]) {
					for (size_t k = 0; k < 8; k++) {
						for (size_t l = 0; l < 8; l++) {
							putPixelRGB(renderer.get(), 8 * j + l, 8 * i + k, 0xff, 0xff, 0xff);
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


int _main(int argc, char *args[]) {
	SDL_Init(SDL_INIT_VIDEO);

	std::string fileName = "donkeykong.nes";
	std::string filePath = boost::filesystem::path(__FILE__)
			.parent_path()
			.append(fileName)
			.string();

	std::cout << "Trying to load: " << filePath << '\n';
	std::ifstream file(filePath, std::ios::binary);

	if (!file) {
		std::cout << "Failed to read rom given: " << filePath << '\n';
		return 0;
	}

	file.unsetf(std::ios::skipws);

	ROM rom(file);
	PPU ppu(rom);

	textureDemo(ppu);
	return 0;
}

int main(int argc, char *args[]) {
	std::cout << "Hello world\n";
	std::ios_base::sync_with_stdio(false);
	nanolog::initialize(nanolog::NonGuaranteedLogger(3), "/tmp/", "nanolog", 1);
	LOG_INFO << "Sample NanoLog: ";
	LOG_INFO << 123;
	SDL_Init(SDL_INIT_VIDEO);

	std::string fileName = "donkeykong.nes";
	std::string filePath = boost::filesystem::path(__FILE__)
			.parent_path()
			.append(fileName)
			.string();

	LOG_INFO << "Trying to load: " << filePath << '\n';
	std::ifstream file(filePath, std::ios::binary);

	if (!file) {
		LOG_INFO << "Failed to read rom given: " << filePath << '\n';
		return 0;
	}

	file.unsetf(std::ios::skipws);

	Clock clk(std::chrono::milliseconds(100));
	ROM rom(file);
	PPU ppu(rom);
	MMU mmu(ppu);
	std::function<u8 &(u8)> mmuFn = [&mmu](u8 addr) -> u8 & { return mmu(addr); };

	try {
		CPU cpu(clk, mmu);
		std::cout << "Hi " << std::hex << (int) mmu(0xfffa) << '\n';
		cpu.PowerUp();

		cpu.LoadROM(rom);

		std::thread clockThread([&]() {
			clk.startTicking();
		});

		std::thread ioThread([&]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(10000));
			cpu.SetNmi(true);
		});

		std::thread cpuThread([&]() {
			try {
				while (true) {
					cpu.Execute();
				}
			} catch (const std::string &error) {
				std::cout << error << '\n';
			}
		});

		std::thread ppuThread([&]() {
			try {
			} catch (const std::string &error) {
				std::cout << error << '\n';
			}
		});

		cpuThread.join();
		ioThread.join();
	} catch (const std::string &error) {
		std::cout << error << '\n';
		LOG_INFO << error << '\n';
	}

//	GUI gui;
//	SDL_Quit();

	return 0;
}
