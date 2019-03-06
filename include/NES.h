#pragma once

#include "core.h"
#include "CPU.h"

class C64 {
	CPU cpu;
public:

	void LoadROM(std::ifstream &filename);
};

