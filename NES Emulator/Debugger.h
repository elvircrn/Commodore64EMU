#pragma once

#include "CPU.h"

class Debugger
{
	CPU *cpu;
public:
	Debugger();
	~Debugger();

	void PrintFlags();
};

