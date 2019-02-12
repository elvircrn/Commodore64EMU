#include <cstdio>
#include <cstdlib>
#include <thread>
#include <iostream>
#include <chrono>

#include "boost/filesystem.hpp"
#include <SDL2/SDL.h>

#include "graphics_demo.h"
#include "NES.h"
#include "CPU.h"
#include "FileHandler.h"
#include "GUI.h"
#include "MMU.h"
#include "NanoLog.h"
#include "Clock.h"
#include "sdl2.h"

int _main(int argc, char *args[]) {
	patternTablesDemo();
	return 0;
}

int main(int argc, char *args[]) {
	std::ios_base::sync_with_stdio(false);
	nanolog::initialize(nanolog::NonGuaranteedLogger(3), "/tmp/", "nanolog", 1);
	LOG_INFO << "Sample NanoLog: ";
	LOG_INFO << 123;
	SDL_Init(SDL_INIT_VIDEO);
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
	}

	constexpr u16 WINDOW_WIDTH = 256, WINDOW_HEIGHT = 240;
	auto window = sdl2::make_window("NES Emulator", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	auto renderer = sdl2::make_renderer(window.get(), -1, 0);
	auto texture =
			sdl2::make_bmp(renderer.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);

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
	SDL_Event event;

	Clock clk(std::chrono::milliseconds(1));
	ROM rom(file);
	PPU ppu(rom);
	IO io(event);
	MMU mmu(ppu, io);

	try {
		CPU cpu(clk, mmu);
		cpu.powerUp();

		cpu.loadROM(rom);

		std::thread clockThread([&]() {
			clk.startTicking();
		});

		std::thread cpuThread([&]() {
			try {
				while (true) {
					cpu.execute();
				}
			} catch (const std::string &error) {
				std::cout << error << '\n';
			}
		});

		std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Wait until CPU is initialized.
		for (;;) {
			ppu.startVBlank();
			cpu.setNMI(true);
			std::this_thread::sleep_for(std::chrono::milliseconds(900));
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

			ppu.drawStuff(renderer.get());

			SDL_SetRenderTarget(renderer.get(), nullptr);

			// SDL_RenderCopy is responsible for making the gameloop understand that there's
			// something that wants to be rendered, inside the parentheses are (the renderer's name,
			// the name of the texture, the area it wants to crop but you can leave this as NULL
			// if you don't want the texture to be cropped, and the texture's rect).
			SDL_RenderCopy(renderer.get(), texture.get(), nullptr, nullptr);

			SDL_RenderPresent(renderer.get());
		}
		cpuThread.join();
	} catch (const std::string &error) {
		std::cout << error << '\n';
		LOG_INFO << error << '\n';
	}

//	GUI gui;
//	SDL_Quit();

	return 0;
}
