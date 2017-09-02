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

	void LoadROM(const std::string &filename);
};

