#pragma once

#include "core.h"
#include "CPU.h"
#include "PPU.h"

class NES
{
	CPU cpu;
	PPU ppu;
public:

	NES();

	void LoadROM(std::ifstream &filename);
};

