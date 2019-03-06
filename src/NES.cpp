#include "NES.h"

#include <fstream>

void C64::LoadROM(std::ifstream &fstream) {
	ROM rom(fstream);
	cpu.loadROM(rom);
}
