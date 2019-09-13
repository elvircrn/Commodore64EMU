#pragma once

#include "CPU.h"
#include "core.h"

class CPU;

class Debugger
{
	CPU *cpu;
	std::vector<std::string> statHist;
	std::vector<std::string> instructionName;

public:
	Debugger();
	Debugger(CPU*);
	~Debugger();

	void PrintFlags() const;
	std::string GetC64TestLine() const;
	std::string GetC64TestLineWithCycles() const;
	std::string GetStatusLine() const;

	void AppendStatHist(const std::string&);

	std::string GetOpHistForLogging() const;
	std::string GetPCHistForLogging() const;

};

