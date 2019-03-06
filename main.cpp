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
#include "Clock.h"
#include "sdl2.h"

int main(int argc, char* args[]) {
	std::ios_base::sync_with_stdio(false);
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

	std::cout << "Trying to load: " << filePath << '\n';
	std::ifstream file(filePath, std::ios::binary);

	if (!file) {
		std::cout << "Failed to read rom given: " << filePath << '\n';
		return 0;
	}

	file.unsetf(std::ios::skipws);
	SDL_Event event;

	Clock clk(std::chrono::milliseconds(1));
	ROM rom(file);
	IO io(event);
	MMU mmu(io);
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
		}
		catch (const std::string & error) {
			std::cout << error << '\n';
		}
		});

	std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Wait until CPU is initialized.

	cpuThread.join();
	//	GUI gui;
	//	SDL_Quit();

	return 0;
}
