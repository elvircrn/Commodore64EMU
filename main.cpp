#include <cstdio>
#include <cstdlib>
#include <thread>
#include <iostream>
#include <chrono>

#include "boost/filesystem.hpp"
#include <SDL2/SDL.h>

#include "demo.h"
#include "NES.h"
#include "CPU.h"
#include "FileHandler.h"
#include "GUI.h"
#include "MMU.h"
#include "NanoLog.h"
#include "Clock.h"

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
		cpu.PowerUp();

		cpu.LoadROM(rom);

		std::thread clockThread([&]() {
			clk.startTicking();
		});

		std::thread ioThread([&]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			for (;;) {
				cpu.SetNmi(true);
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				for (int i = 0; i < 34; i++) {
//					ppu.drawStuff();
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
				}
			}
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
