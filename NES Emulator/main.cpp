#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "boost/filesystem.hpp"

#include "NES.h"
#include "CPU.h"
#include "FileHandler.h"

#include "SDL.h"
#include "GUI.h"
#include "MMU.h"

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

	auto data = ppu.Pattern();

	for (auto &x : data) {
		for (auto &y : x) {
			if (y == 0)
				std::cout << ' ';
			else
				std::cout << (int) y;
		}
		std::cout << '\n';
	}


	MMU mmu(ppu);
	std::function<u8&(u8)> mmuFn = [&mmu](u8 addr) -> u8& { return static_cast<u8&>(mmu(addr)); };

	try {
		CPU cpu(mmu);
		cpu.PowerUp();

		cpu.LoadROM(rom);

		for (int i = 0; i < 100; i++) {
			cpu.Execute();
		}
	} catch (const std::string &error) {
		std::cout << error << '\n';
	}

	GUI gui;
	SDL_Quit();

	return 0;
}
