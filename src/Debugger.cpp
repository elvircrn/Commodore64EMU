#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

#include "Debugger.h"

Debugger::Debugger() : instructionName(0xff + 1)
{
	instructionName[0x69] = "ADC";
	instructionName[0x65] = "ADC";
	instructionName[0x75] = "ADC";
	instructionName[0x6D] = "ADC";
	instructionName[0x7D] = "ADC";
	instructionName[0x79] = "ADC";
	instructionName[0x71] = "ADC";
	instructionName[0x61] = "ADC";
	instructionName[0x29] = "AND";
	instructionName[0x25] = "AND";
	instructionName[0x35] = "AND";
	instructionName[0x2D] = "AND";
	instructionName[0x3D] = "AND";
	instructionName[0x39] = "AND";
	instructionName[0x21] = "AND";
	instructionName[0x31] = "AND";
	instructionName[0x0A] = "ASL";
	instructionName[0x06] = "ASL";
	instructionName[0x16] = "ASL";
	instructionName[0x0E] = "ASL";
	instructionName[0x1E] = "ASL";
	instructionName[0x24] = "BIT";
	instructionName[0x2C] = "BIT";
	instructionName[0x10] = "BPL";
	instructionName[0x30] = "BMI";
	instructionName[0x50] = "BVC";
	instructionName[0x70] = "BVS";
	instructionName[0x90] = "BCC";
	instructionName[0xB0] = "BCS";
	instructionName[0xD0] = "BNE";
	instructionName[0xF0] = "BEQ";
	instructionName[0x00] = "BRK";
	instructionName[0xC9] = "CMP";
	instructionName[0xC5] = "CMP";
	instructionName[0xD5] = "CMP";
	instructionName[0xCD] = "CMP";
	instructionName[0xDD] = "CMP";
	instructionName[0xD9] = "CMP";
	instructionName[0xC1] = "CMP";
	instructionName[0xD1] = "CMP";
	instructionName[0xE0] = "CPX";
	instructionName[0xE4] = "CPX";
	instructionName[0xEC] = "CPX";
	instructionName[0xC0] = "CPY";
	instructionName[0xC4] = "CPY";
	instructionName[0xCC] = "CPY";
	instructionName[0xC6] = "DEC";
	instructionName[0xD6] = "DEC";
	instructionName[0xCE] = "DEC";
	instructionName[0xDE] = "DEC";
	instructionName[0x49] = "EOR";
	instructionName[0x45] = "EOR";
	instructionName[0x55] = "EOR";
	instructionName[0x4D] = "EOR";
	instructionName[0x5D] = "EOR";
	instructionName[0x59] = "EOR";
	instructionName[0x41] = "EOR";
	instructionName[0x51] = "EOR";
	instructionName[0x18] = "CLC";
	instructionName[0x38] = "SEC";
	instructionName[0x58] = "CLI";
	instructionName[0x78] = "SEI";
	instructionName[0xB8] = "CLV";
	instructionName[0xD8] = "CLD";
	instructionName[0xF8] = "SED";
	instructionName[0xE6] = "INC";
	instructionName[0xF6] = "INC";
	instructionName[0xEE] = "INC";
	instructionName[0xFE] = "INC";
	instructionName[0x4C] = "JMP";
	instructionName[0x6C] = "JMP";
	instructionName[0x20] = "JSR";
	instructionName[0xA9] = "LDA";
	instructionName[0xA5] = "LDA";
	instructionName[0xB5] = "LDA";
	instructionName[0xAD] = "LDA";
	instructionName[0xBD] = "LDA";
	instructionName[0xB9] = "LDA";
	instructionName[0xA1] = "LDA";
	instructionName[0xB1] = "LDA";
	instructionName[0xA2] = "LDX";
	instructionName[0xA6] = "LDX";
	instructionName[0xB6] = "LDX";
	instructionName[0xAE] = "LDX";
	instructionName[0xBE] = "LDX";
	instructionName[0xA0] = "LDY";
	instructionName[0xA4] = "LDY";
	instructionName[0xB4] = "LDY";
	instructionName[0xAC] = "LDY";
	instructionName[0xBC] = "LDY";
	instructionName[0x4A] = "LSR";
	instructionName[0x46] = "LSR";
	instructionName[0x56] = "LSR";
	instructionName[0x4E] = "LSR";
	instructionName[0x5E] = "LSR";
	instructionName[0x09] = "ORA";
	instructionName[0x05] = "ORA";
	instructionName[0x15] = "ORA";
	instructionName[0x0D] = "ORA";
	instructionName[0x1D] = "ORA";
	instructionName[0x19] = "ORA";
	instructionName[0x01] = "ORA";
	instructionName[0x11] = "ORA";
	instructionName[0xAA] = "TAX";
	instructionName[0x8A] = "TXA";
	instructionName[0xCA] = "DEX";
	instructionName[0xE8] = "INX";
	instructionName[0xA8] = "TAY";
	instructionName[0x98] = "TYA";
	instructionName[0x88] = "DEY";
	instructionName[0xC8] = "INY";
	instructionName[0x2A] = "ROL";
	instructionName[0x26] = "ROL";
	instructionName[0x36] = "ROL";
	instructionName[0x2E] = "ROL";
	instructionName[0x3E] = "ROL";
	instructionName[0x6A] = "ROR";
	instructionName[0x66] = "ROR";
	instructionName[0x76] = "ROR";
	instructionName[0x6E] = "ROR";
	instructionName[0x7E] = "ROR";
	instructionName[0x40] = "RTI";
	instructionName[0x60] = "RTS";
	instructionName[0xE9] = "SBC";
	instructionName[0xE5] = "SBC";
	instructionName[0xF5] = "SBC";
	instructionName[0xED] = "SBC";
	instructionName[0xFD] = "SBC";
	instructionName[0xF9] = "SBC";
	instructionName[0xE1] = "SBC";
	instructionName[0xF1] = "SBC";
	instructionName[0x85] = "STA";
	instructionName[0x95] = "STA";
	instructionName[0x8D] = "STA";
	instructionName[0x9D] = "STA";
	instructionName[0x99] = "STA";
	instructionName[0x81] = "STA";
	instructionName[0x91] = "STA";
	instructionName[0x86] = "STX";
	instructionName[0x96] = "STX";
	instructionName[0x8E] = "STX";
	instructionName[0x84] = "STY";
	instructionName[0x94] = "STY";
	instructionName[0x8C] = "STY";
	instructionName[0x9A] = "TXS";
	instructionName[0xBA] = "TSX";
	instructionName[0x48] = "PHA";
	instructionName[0x68] = "PLA";
	instructionName[0x08] = "PHP";
	instructionName[0x28] = "PLP";
	instructionName[0xEA] = "NOP";
}

Debugger::Debugger(CPU *_cpu) : Debugger()
{
	cpu = _cpu;
}

Debugger::~Debugger()
{
}

void Debugger::PrintFlags() const
{
}

std::string Debugger::GetNESTestLineWithCycles() const
{
	std::stringstream ss;

	ss << "A:" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)cpu->A()
		<< " X:" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)cpu->X()
		<< " Y:" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)cpu->Y()
		<< " P:" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)cpu->P()
		<< " SP:" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)cpu->SP()
		<< " CYC:" << std::dec << std::setfill(' ') << std::setw(3) << ((int)cpu->cycleCount + (cpu->cycleCount == 0 ? 0 : 2)) % 340;
	return ss.str();
}

std::string Debugger::GetNESTestLine() const
{
	std::stringstream ss;

	ss  << "A:" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)cpu->A()
		<< " X:" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)cpu->X()
		<< " Y:" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)cpu->Y()
		<< " P:" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)cpu->P()
		<< " SP:" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)cpu->SP();

	return ss.str();
}

std::string Debugger::GetStatusLine() const
{
	std::stringstream ss;
	ss << "PC:" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)cpu->PC();
	return ss.str() + " " + GetNESTestLine();
}

void Debugger::AppendStatHist(const std::string &stat)
{
	statHist.push_back(instructionName[cpu->Read(cpu->PC())] + " " + stat);
}

std::string Debugger::GetOpHistForLogging() const
{
	std::stringstream ss;
	ss << "op:\n";
	for (size_t i = 0; i < cpu->opHist.size(); i++)
		ss << std::hex << (int)cpu->opHist[i] << ' ' << statHist[i] << '\n';
	return ss.str();
}

std::string Debugger::GetPCHistForLogging() const
{
	std::stringstream ss;
	ss << "pc:\n";
	for (int x : cpu->pcHist)
		ss << std::hex << (int)x << '\n';
	return ss.str();
}