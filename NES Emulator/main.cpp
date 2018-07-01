#include <iostream>
#include <cstdio>
#include <cstdlib>

#define USES_SDL

// #define TEST_MODE

#ifndef TEST_MODE

#include "NES.h"
#include "CPU.h"
#include "FileHandler.h"

#ifdef USES_SDL
#include "SDL.h"
#include "GUI.h"
#endif

#ifdef USES_SDL
int main(int argc, char *args[])
#else
int main()
#endif
{

#ifdef USES_SDL

	SDL_Init(SDL_INIT_VIDEO);
	GUI gui;
	SDL_Quit();
#else

	ROM rom("donkeykong.nes");
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

	CPU cpu(&ppu);
	cpu.PowerUp();

	cpu.LoadROM(rom);

	for (int i = 0; i < 10; i++) {
		cpu.Execute();
	}

	std::getchar();
#endif

	return 0;
}
#endif