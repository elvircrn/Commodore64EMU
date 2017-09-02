#include "NES.h"

void NES::LoadROM(const std::string & filename)
{
	ROM rom(filename);
	cpu.LoadROM(rom);
	ppu.LoadROM(rom);
}
