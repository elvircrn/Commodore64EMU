#include "NES.h"

#include <fstream>

void NES::LoadROM(std::ifstream &fstream) {
	ROM rom(fstream);
	cpu.LoadROM(rom);
	ppu.LoadROM(rom);
}
