#include "CPU.h"
#include "Instructions.h"

#include <array>
#include <tuple>
#include <sstream>
#include <string>

void CPU::LoadROM(const ROM & rom)
{
	for (int i = 0; i < 0x4000; i++)
		ram[0xC000 + i] = rom[i];
}

CPU::CPU() : ram(RAM_SIZE), isOfficial(0xff), cycleCount(0), ppu(nullptr)
{
	u8 official[] = { 0x69, 0x65, 0x75, 0x6D, 0x7D, 0x79, 0x61, 0x71, 0x28, 0xEA,
		  			  0x29, 0x25, 0x35, 0x2D, 0x3D, 0x39, 0x21, 0x31, 0x0A, 0x06,
		  			  0x16, 0x0E, 0x1E, 0x24, 0x2C, 0x10, 0x30, 0x50, 0x70, 0x90,
		  			  0xB0, 0xD0, 0xF0, 0x00, 0xC9, 0xC5, 0xD5, 0xCD, 0xDD, 0xD9,
		  			  0xC1, 0xD1, 0xE0, 0xE4, 0xEC, 0xC0, 0xC4, 0xCC, 0xC6, 0xD6,
		  			  0xCE, 0xDE, 0x49, 0x45, 0x55, 0x4D, 0x5D, 0x59, 0x41, 0x51,
		  			  0x18, 0x38, 0x58, 0x78, 0xB8, 0xD8, 0xF8, 0xE6, 0xF6, 0xEE,
		  			  0xFE, 0x4C, 0x6C, 0x20, 0xA9, 0xA5, 0xB5, 0xAD, 0xBD, 0xB9,
		  			  0xA1, 0xB1, 0xA2, 0xA6, 0xB6, 0xAE, 0xBE, 0xA0, 0xA4, 0xB4,
		  			  0xAC, 0xBC, 0x4A, 0x46, 0x56, 0x4E, 0x5E, 0x09, 0x05, 0x15,
		  			  0x0D, 0x1D, 0x19, 0x01, 0x11, 0xAA, 0x8A, 0xCA, 0xE8, 0xA8,
		  			  0x98, 0x88, 0xC8, 0x2A, 0x26, 0x36, 0x2E, 0x3E, 0x6A, 0x66,
		  			  0x76, 0x6E, 0x7E, 0x40, 0x60, 0xE9, 0xE5, 0xF5, 0xED, 0xFD,
		  			  0xF9, 0xE1, 0xF1, 0x85, 0x95, 0x8D, 0x9D, 0x99, 0x81, 0x91,
					  0x86, 0x96, 0x8E, 0x84, 0x94, 0x8C, 0x9A, 0xBA, 0x48, 0x68, 0x08 };
	for (u8 o : official)
		isOfficial[o] = true;
}

CPU::CPU(PPU *_ppu) : CPU()
{
	ppu = _ppu;
}

CPU::~CPU()
{
}

void CPU::PowerUp()
{
	// TODO: Check if IRQ should be disabled
	p = 0x24; // IRQ disabled
	a = 0;
	x = 0;
	y = 0;
	sp = 0xfd;
	pc = 0xC000;
}

void CPU::Execute()
{
	u16 prevPc = pc;
	Clear();
	if constexpr (DEBUG)
	{
		opHist.push_back(Read(pc));
		pcHist.push_back(pc);
		bitStack.clear();
		instrHist.emplace_back(pc,
			Instructions::Name(Read(pc)),
			std::array<u8, 3>({ Read(pc + 1), Read(pc + 2), Read(pc + 3) }));
		for (int i = 0; i < 5; i++)
			bitStack.push_back(Read(pc + i));
	}

	Tick(2);

	u8 op = Read(pc++);
	switch (op)
	{
		case 0x69: ADC<AddressingModes::IMMEDIATE>(); break;
		case 0x65: ADC<AddressingModes::ZERO_PAGE>(); break;
		case 0x75: ADC<AddressingModes::ZERO_PAGE_X>(); break;
		case 0x6D: ADC<AddressingModes::ABSOLUTE>(); break;
		case 0x7D: ADC<AddressingModes::ABSOLUTE_INDEXED_X>(); break;
		case 0x79: ADC<AddressingModes::ABSOLUTE_INDEXED_Y>(); break;
		case 0x61: ADC<AddressingModes::INDEXED_INDIRECT_X>(); break;
		case 0x71: ADC<AddressingModes::INDIRECT_INDEXED>(); break;

		case 0x29: AND<AddressingModes::IMMEDIATE>(); break;
		case 0x25: AND<AddressingModes::ZERO_PAGE>(); break;
		case 0x35: AND<AddressingModes::ZERO_PAGE_X>(); break;
		case 0x2D: AND<AddressingModes::ABSOLUTE>(); break;
		case 0x3D: AND<AddressingModes::ABSOLUTE_INDEXED_X>(); break;
		case 0x39: AND<AddressingModes::ABSOLUTE_INDEXED_Y>(); break;
		case 0x21: AND<AddressingModes::INDEXED_INDIRECT_X>(); break;
		case 0x31: AND<AddressingModes::INDIRECT_INDEXED>(); break;
		// TODO: Check Zero Page, Y!!!
		case 0x0A: ASL<AddressingModes::ACCUMULATOR>(); break;
		case 0x06: ASL<AddressingModes::ZERO_PAGE>(); break;
		case 0x16: ASL<AddressingModes::ZERO_PAGE_X>(); break;
		case 0x0E: ASL<AddressingModes::ABSOLUTE>(); break;
		case 0x1E: ASL<AddressingModes::ABSOLUTE_INDEXED_X>(); break;
		case 0x24: BIT<AddressingModes::ZERO_PAGE>(); break;
		case 0x2C: BIT<AddressingModes::ABSOLUTE>(); break;
		case 0x10: BPL<AddressingModes::RELATIVE>(); break;
		case 0x30: BMI<AddressingModes::RELATIVE>(); break;
		case 0x50: BVC<AddressingModes::RELATIVE>(); break;
		case 0x70: BVS<AddressingModes::RELATIVE>(); break;
		case 0x90: BCC<AddressingModes::RELATIVE>(); break;
		case 0xB0: BCS<AddressingModes::RELATIVE>(); break;
		case 0xD0: BNE<AddressingModes::RELATIVE>(); break;
		case 0xF0: BEQ<AddressingModes::RELATIVE>(); break;
		// TODO :Implement the BRK instruction
		case 0x00: BRK<AddressingModes::IMPLIED>(); break;
		case 0xC9: CMP<AddressingModes::IMMEDIATE>(); break;
		case 0xC5: CMP<AddressingModes::ZERO_PAGE>(); break;
		case 0xD5: CMP<AddressingModes::ZERO_PAGE_X>(); break;
		case 0xCD: CMP<AddressingModes::ABSOLUTE>(); break;
		case 0xDD: CMP<AddressingModes::ABSOLUTE_INDEXED_X>(); break;
		case 0xD9: CMP<AddressingModes::ABSOLUTE_INDEXED_Y>(); break;
		case 0xC1: CMP<AddressingModes::INDEXED_INDIRECT_X>(); break;
		case 0xD1: CMP<AddressingModes::INDIRECT_INDEXED>(); break;
		case 0xE0: CPX<AddressingModes::IMMEDIATE>(); break;
		case 0xE4: CPX<AddressingModes::ZERO_PAGE>(); break;
		case 0xEC: CPX<AddressingModes::ABSOLUTE>(); break;
		case 0xC0: CPY<AddressingModes::IMMEDIATE>(); break;
		case 0xC4: CPY<AddressingModes::ZERO_PAGE>(); break;
		case 0xCC: CPY<AddressingModes::ABSOLUTE>(); break;
		case 0xC6: DEC<AddressingModes::ZERO_PAGE>(); break;
		case 0xD6: DEC<AddressingModes::ZERO_PAGE_X>(); break;
		case 0xCE: DEC<AddressingModes::ABSOLUTE>(); break;
		case 0xDE: DEC<AddressingModes::ABSOLUTE_INDEXED_X>(); break;
		case 0x49: EOR<AddressingModes::IMMEDIATE>(); break;
		case 0x45: EOR<AddressingModes::ZERO_PAGE>(); break;
		case 0x55: EOR<AddressingModes::ZERO_PAGE_X>(); break;
		case 0x4D: EOR<AddressingModes::ABSOLUTE>(); break;
		case 0x5D: EOR<AddressingModes::ABSOLUTE_INDEXED_X>(); break;
		case 0x59: EOR<AddressingModes::ABSOLUTE_INDEXED_Y>(); break;
		case 0x41: EOR<AddressingModes::INDEXED_INDIRECT_X>(); break;
		case 0x51: EOR<AddressingModes::INDIRECT_INDEXED>(); break;
		case 0x18: CLC<AddressingModes::IMPLIED>(); break;
		case 0x38: SEC<AddressingModes::IMPLIED>(); break;
		case 0x58: CLI<AddressingModes::IMPLIED>(); break;
		case 0x78: SEI<AddressingModes::IMPLIED>(); break;
		case 0xB8: CLV<AddressingModes::IMPLIED>(); break;
		case 0xD8: CLD<AddressingModes::IMPLIED>(); break;
		case 0xF8: SED<AddressingModes::IMPLIED>(); break;
		case 0xE6: INC<AddressingModes::ZERO_PAGE>(); break;
		case 0xF6: INC<AddressingModes::ZERO_PAGE_X>(); break;
		case 0xEE: INC<AddressingModes::ABSOLUTE>(); break;
		case 0xFE: INC<AddressingModes::ABSOLUTE_INDEXED_X>(); break;
		case 0x4C: JMP<AddressingModes::ABSOLUTE>(); break;
		case 0x6C: JMP<AddressingModes::INDIRECT>(); break;
		case 0x20: JSR<AddressingModes::ABSOLUTE>(); break;
		case 0xA9: LDA<AddressingModes::IMMEDIATE>(); break;
		case 0xA5: LDA<AddressingModes::ZERO_PAGE>(); break;
		case 0xB5: LDA<AddressingModes::ZERO_PAGE_X>(); break;
		case 0xAD: LDA<AddressingModes::ABSOLUTE>(); break;
		case 0xBD: LDA<AddressingModes::ABSOLUTE_INDEXED_X>(); break;
		case 0xB9: LDA<AddressingModes::ABSOLUTE_INDEXED_Y>(); break;
		case 0xA1: LDA<AddressingModes::INDEXED_INDIRECT_X>(); break;
		case 0xB1: LDA<AddressingModes::INDIRECT_INDEXED>(); break;
		case 0xA2: LDX<AddressingModes::IMMEDIATE>(); break;
		case 0xA6: LDX<AddressingModes::ZERO_PAGE>(); break;
		case 0xB6: LDX<AddressingModes::ZERO_PAGE_Y>(); break;
		case 0xAE: LDX<AddressingModes::ABSOLUTE>(); break;
		case 0xBE: LDX<AddressingModes::ABSOLUTE_INDEXED_Y>(); break;
		case 0xA0: LDY<AddressingModes::IMMEDIATE>(); break;
		case 0xA4: LDY<AddressingModes::ZERO_PAGE>(); break;
		case 0xB4: LDY<AddressingModes::ZERO_PAGE_X>(); break;
		case 0xAC: LDY<AddressingModes::ABSOLUTE>(); break;
		case 0xBC: LDY<AddressingModes::ABSOLUTE_INDEXED_X>(); break;
		case 0x4A: LSR<AddressingModes::ACCUMULATOR>(); break;
		case 0x46: LSR<AddressingModes::ZERO_PAGE>(); break;
		case 0x56: LSR<AddressingModes::ZERO_PAGE_X>(); break;
		case 0x4E: LSR<AddressingModes::ABSOLUTE>(); break;
		case 0x5E: LSR<AddressingModes::ABSOLUTE_INDEXED_X>(); break;
		case 0x09: ORA<AddressingModes::IMMEDIATE>(); break;
		case 0x05: ORA<AddressingModes::ZERO_PAGE>(); break;
		case 0x15: ORA<AddressingModes::ZERO_PAGE_X>(); break;
		case 0x0D: ORA<AddressingModes::ABSOLUTE>(); break;
		case 0x1D: ORA<AddressingModes::ABSOLUTE_INDEXED_X>(); break;
		case 0x19: ORA<AddressingModes::ABSOLUTE_INDEXED_Y>(); break;
		case 0x01: ORA<AddressingModes::INDEXED_INDIRECT_X>(); break;
		case 0x11: ORA<AddressingModes::INDIRECT_INDEXED>(); break;
		case 0xAA: TAX<AddressingModes::IMPLIED>(); break;
		case 0x8A: TXA<AddressingModes::IMPLIED>(); break;
		case 0xCA: DEX<AddressingModes::IMPLIED>(); break;
		case 0xE8: INX<AddressingModes::IMPLIED>(); break;
		case 0xA8: TAY<AddressingModes::IMPLIED>(); break;
		case 0x98: TYA<AddressingModes::IMPLIED>(); break;
		case 0x88: DEY<AddressingModes::IMPLIED>(); break;
		case 0xC8: INY<AddressingModes::IMPLIED>(); break;
		case 0x2A: ROL<AddressingModes::ACCUMULATOR>(); break;
		case 0x26: ROL<AddressingModes::ZERO_PAGE>(); break;
		case 0x36: ROL<AddressingModes::ZERO_PAGE_X>(); break;
		case 0x2E: ROL<AddressingModes::ABSOLUTE>(); break;
		case 0x3E: ROL<AddressingModes::ABSOLUTE_INDEXED_X>(); break;
		case 0x6A: ROR<AddressingModes::ACCUMULATOR>(); break;
		case 0x66: ROR<AddressingModes::ZERO_PAGE>(); break;
		case 0x76: ROR<AddressingModes::ZERO_PAGE_X>(); break;
		case 0x6E: ROR<AddressingModes::ABSOLUTE>(); break;
		case 0x7E: ROR<AddressingModes::ABSOLUTE_INDEXED_X>(); break;
		case 0x40: RTI<AddressingModes::IMPLIED>(); break;
		case 0x60: RTS<AddressingModes::IMPLIED>(); break;
		case 0xE9: SBC<AddressingModes::IMMEDIATE>(); break;
		case 0xE5: SBC<AddressingModes::ZERO_PAGE>(); break;
		case 0xF5: SBC<AddressingModes::ZERO_PAGE_X>(); break;
		case 0xED: SBC<AddressingModes::ABSOLUTE>(); break;
		case 0xFD: SBC<AddressingModes::ABSOLUTE_INDEXED_X>(); break;
		case 0xF9: SBC<AddressingModes::ABSOLUTE_INDEXED_Y>(); break;
		case 0xE1: SBC<AddressingModes::INDEXED_INDIRECT_X>(); break;
		case 0xF1: SBC<AddressingModes::INDIRECT_INDEXED>(); break;
		case 0x85: STA<AddressingModes::ZERO_PAGE>(); break;
		case 0x95: STA<AddressingModes::ZERO_PAGE_X>(); break;
		case 0x8D: STA<AddressingModes::ABSOLUTE>(); break;
		case 0x9D: STA<AddressingModes::ABSOLUTE_INDEXED_X>(); break;
		case 0x99: STA<AddressingModes::ABSOLUTE_INDEXED_Y>(); break;
		case 0x81: STA<AddressingModes::INDEXED_INDIRECT_X>(); break;
		case 0x91: STA<AddressingModes::INDIRECT_INDEXED>(); break;
		case 0x86: STX<AddressingModes::ZERO_PAGE>(); break;
		case 0x96: STX<AddressingModes::ZERO_PAGE_Y>(); break;
		case 0x8E: STX<AddressingModes::ABSOLUTE>(); break;
		case 0x84: STY<AddressingModes::ZERO_PAGE>(); break;
		case 0x94: STY<AddressingModes::ZERO_PAGE_X>(); break;
		case 0x8C: STY<AddressingModes::ABSOLUTE>(); break;
		case 0x9A: TXS<AddressingModes::IMPLIED>(); break;
		case 0xBA: TSX<AddressingModes::IMPLIED>(); break;
		case 0x48: PHA<AddressingModes::IMPLIED>(); break;
		case 0x68: PLA<AddressingModes::IMPLIED>(); break;
		case 0x08: PHP<AddressingModes::IMPLIED>(); break;
		case 0x28: PLP<AddressingModes::IMPLIED>(); break;
		case 0xEA: NOP<AddressingModes::IMPLIED>(); break;

		//Unofficial opcodes
		// NOP - skip 0, 2 cycles
		case 0x1A: NOP<AddressingModes::IMPLIED>(); break;
		case 0x3A: NOP<AddressingModes::IMPLIED>(); break;
		case 0x5A: NOP<AddressingModes::IMPLIED>(); break;
		case 0x7A: NOP<AddressingModes::IMPLIED>(); break;
		case 0xDA: NOP<AddressingModes::IMPLIED>(); break;
		case 0xFA: NOP<AddressingModes::IMPLIED>(); break;

		// NOP - skip 1
		case 0x04: NOP<AddressingModes::IMPLIED, 1>(); break;
		case 0x44: NOP<AddressingModes::IMPLIED, 1>(); break;
		case 0x64: NOP<AddressingModes::IMPLIED, 1>(); break;
		case 0x14: NOP<AddressingModes::IMPLIED, 1>(); break; 
		case 0x34: NOP<AddressingModes::IMPLIED, 1>(); break; 
		case 0x54: NOP<AddressingModes::IMPLIED, 1>(); break; 
		case 0x74: NOP<AddressingModes::IMPLIED, 1>(); break; 
		case 0xD4: NOP<AddressingModes::IMPLIED, 1>(); break; 
		case 0xF4: NOP<AddressingModes::IMPLIED, 1>(); break;
		case 0x80: NOP<AddressingModes::IMPLIED, 1>(); break;
		
		// NOP - skip 2
		case 0x0C: NOP<AddressingModes::IMPLIED, 2>(); break;
		case 0x1C: NOP<AddressingModes::IMPLIED, 2>(); break; 
		case 0x3C: NOP<AddressingModes::IMPLIED, 2>(); break; 
		case 0x5C: NOP<AddressingModes::IMPLIED, 2>(); break; 
		case 0x7C: NOP<AddressingModes::IMPLIED, 2>(); break; 
		case 0xDC: NOP<AddressingModes::IMPLIED, 2>(); break; 
		case 0xFC: NOP<AddressingModes::IMPLIED, 2>(); break;
		
		default:
			std::stringstream ss;
			for (int i = pc; i < pc + 10; i++)
				ss << std::hex << (int)Read(i) << ' ';
			DebugDump();
			throw "Instruction not found at pc: " + std::to_string(pc) + "\nDump: " + ss.str();
	}

	Tick(zeroPageCrossed & calcCrossed);
	cycleCount += 4;
}

#pragma region Debug and Logging
void CPU::DebugDump()
{
	for (int i = 0xfff0; i < 0x10000; i++)
		vectors.emplace_back(HexString(i), Read(static_cast<u16>(i)));
}
#pragma endregion 

#pragma region Register getters and setters

#pragma endregion

// Used for instructions that only use the operand for reading
template<AddressingModes mode>
u16 CPU::GetPureOperand()
{
	if constexpr (mode < 2)
		return GetOperand16<mode>();
	else
		return GetOperand8<mode>();
}

template<AddressingModes mode>
u16 CPU::GetOperand16()
{
	// Used by branch instructions
	// TODO: Check if -1 is needed
	if constexpr (mode == AddressingModes::RELATIVE)
	{
		u8 imm = Imm();
		return pc + imm;
	}
	// TODO: Check if -2 is needed
	else if (mode == AddressingModes::INDIRECT)
		return Ind(Abs());
	else
		throw "Invalid instruction";
}

#pragma region Instruction helpers
template<AddressingModes mode>
u8& CPU::GetOperand8()
{
	// ADC $3420 -> A + contents of memory $3420
	if constexpr (mode == AddressingModes::ABSOLUTE)
	{
		u16 abs = Abs();
		return Read(abs);
	}
    // ADC #2 -> A + 2
	else if (mode == AddressingModes::IMMEDIATE)
	{
		buff8 = Imm();
		return buff8;
	}
	// ADC $3420,X -> A + contents of memory $3420 + X
	else if (mode == AddressingModes::ABSOLUTE_INDEXED_X)
		return Read(AbsX());
	// ADC $3420,Y	-> A + contents of memory $3420 + Y
	else if (mode == AddressingModes::ABSOLUTE_INDEXED_Y)
		return Read(AbsY());
	// ADC $F6 -> A + contents of memory $F6
	else if (mode == AddressingModes::ZERO_PAGE)
		return Zp(Imm());
	else if (mode == AddressingModes::ZERO_PAGE_X)
		return Zp(Imm() + x);
	else if (mode == AddressingModes::ZERO_PAGE_Y)
		return Zp(Imm() + y);
	// https://www.csh.rit.edu/~moffitt/6502.html#ADDR-IIND
	else if (mode == AddressingModes::INDEXED_INDIRECT_X)
	{
		u8 addr8 = Imm() + x;
		u16 zp16 = Zp16(addr8);
		return Read(zp16);
	}
	else if (mode == AddressingModes::INDIRECT_INDEXED)
	{
		u16 zp16 = Zp16(Imm());
		CrossesPage(zp16, y);
		return Read(zp16 + y);
	}
	else if (mode == AddressingModes::ACCUMULATOR)
		return a;
    // ADC ($F6),Y -> A + contents of (address at $F6) + offset Y
	//case AddressingModes::INDIRECT_INDEXED:
		//return Zp(Imm()) + y;
	else
		throw "Wrong addressing mode implemented! " + std::to_string(mode);
}
#pragma endregion

//.... add with carry
template<AddressingModes mode>
void CPU::ADC()
{
	Tick(CalcBaseTicks<mode>());
	calcCrossed = true;

	u8 lhs = a;
	u8 rhs = GetOperand8<mode>();
	s16 res = lhs + rhs + C();
	a = static_cast<u8>(res & 0xff);
	UpdCV(lhs, rhs, res);
	UpdNZ((u8)(res & 0xff));
}

//....	and (with accumulator)  
template<AddressingModes mode>
void CPU::AND()
{
	calcCrossed = true;
	UpdNZ(a &= GetPureOperand<mode>());
}

//....	arithmetic shift left  
template<AddressingModes mode>
void CPU::ASL()
{
	Tick(CalcBaseTicks<mode>());
	Tick(CalcShiftTicks<mode>());
	UpdNZ(_ASL(GetOperand8<mode>()));
}

// TODO: Consider refactoring into a non-templated function
template<AddressingModes mode>
void CPU::BCC()
{
	u16 loc = GetPureOperand<mode>();
	calcCrossed = true;
	if (!C())
	{
		pc = loc;
	}
}

template<AddressingModes mode>
void CPU::BCS()
{
	u16 loc = GetPureOperand<mode>();
	calcCrossed = true;
	if (C())
	{
		Tick();
		pc = loc;
	}
}

template<AddressingModes mode>
void CPU::BEQ()
{
	u16 loc = GetPureOperand<mode>();
	calcCrossed = true;
	if (Z())
	{
		Tick();
		pc = loc;
	}
}

template<AddressingModes mode>
void CPU::BIT()
{
	u8 op = (u8)GetPureOperand<mode>();
	u8 res = a & op;
	Z(!res);
	N(op & (1 << 7));
	V(op & (1 << 6));
}

template<AddressingModes mode>
void CPU::BMI()
{
	u16 loc = GetPureOperand<mode>();
	calcCrossed = true;
	if (N())
	{
		Tick();
		pc = loc;
	}
}

template<AddressingModes mode>
void CPU::BNE()
{
	u16 loc = GetPureOperand<mode>();
	calcCrossed = true;
	if (!Z())
	{
		Tick();
		pc = loc;
	}
}

template<AddressingModes mode>
void CPU::BPL()
{
	u16 loc = GetPureOperand<mode>();
	calcCrossed = true;
	if (!N())
	{
		Tick();
		pc = loc;
	}
}

// TODO: Consider implementing
// NOTE: Mostly used for debugging.
template<AddressingModes mode>
void CPU::BRK()
{
	INT<Interrupts::BRK>();
}

template<AddressingModes mode>
void CPU::BVC()
{
	u16 loc = GetPureOperand<mode>();
	calcCrossed = true;
	if (!V())
	{
		Tick();
		pc = loc;
	}
}

template<AddressingModes mode>
void CPU::BVS()
{
	u16 loc = GetPureOperand<mode>();
	calcCrossed = true;
	if (V())
	{
		Tick();
		pc = loc;
	}
}

template<AddressingModes mode>
void CPU::CLC() { C(0); }

template<AddressingModes mode>
void CPU::CLD() { D(0); }

template<AddressingModes mode>
void CPU::CLI() { I(0); }

template<AddressingModes mode>
void CPU::CLV() { V(0); }

template<AddressingModes mode>
void CPU::CMP()
{
	calcCrossed = true;
	u8 mem = (u8)GetPureOperand<mode>();
	UpdNZ(a - mem);
	C(a >= mem);
}

template<AddressingModes mode>
void CPU::CPX()
{
	calcCrossed = true;
	u8 mem = (u8)GetPureOperand<mode>();
	UpdNZ(x - mem);
	C(x >= mem);
}

template<AddressingModes mode>
void CPU::CPY()
{
	calcCrossed = true;
	u8 mem = (u8)GetPureOperand<mode>();
	UpdNZ(y - mem);
	C(y >= mem);
}

template<AddressingModes mode>
void CPU::DEC()
{
	Tick(CalcBaseTicks<mode>() + CalcIncDecTicks<mode>());
	UpdNZ(--GetOperand8<mode>());
}

// No additional cycles
template<AddressingModes mode>
void CPU::DEX() { UpdNZ(--x); }

// No additional cycles
template<AddressingModes mode>
void CPU::DEY() { UpdNZ(--y); }

template<AddressingModes mode>
void CPU::EOR()
{
	Tick(CalcBaseTicks<mode>());
	calcCrossed = true;
	UpdNZ(a ^= GetPureOperand<mode>());
}

template<AddressingModes mode>
void CPU::INC()
{
	Tick(CalcBaseTicks<mode>() + CalcIncDecTicks<mode>());
	UpdNZ(++GetOperand8<mode>());
}

template<AddressingModes mode>
void CPU::INX() { UpdNZ(++x); }

template<AddressingModes mode>
void CPU::INY() { UpdNZ(++y); }

// TODO: Check if Abs should read memory
template<AddressingModes mode>
void CPU::JMP()
{
	if (mode == AddressingModes::ABSOLUTE)
	{
		Tick();
		pc = Abs();
	}
	else if (mode == AddressingModes::INDIRECT)
		pc = Ind(Abs()); // Ind calls tick
}

// TODO: Check if pc - 1 or pc + 1
template<AddressingModes mode>
void CPU::JSR()
{
	Push16(pc + 1);
	pc = Abs();
	Tick(4);
}

template<AddressingModes mode>
void CPU::LDA()
{
	calcCrossed = true;
	Tick(CalcBaseTicks<mode>());
	UpdNZ(a = GetOperand8<mode>());
}

template<AddressingModes mode>
void CPU::LDX()
{
	Tick(CalcBaseTicks<mode>());
	calcCrossed = true;
	UpdNZ(x = GetOperand8<mode>());
}

template<AddressingModes mode>
void CPU::LDY()
{
	Tick(CalcBaseTicks<mode>());
	calcCrossed = true;
	UpdNZ(y = (u8)GetPureOperand<mode>());
}

template<AddressingModes mode>
void CPU::LSR()
{
	u8& op = GetOperand8<mode>();
	C(op & 1);
	UpdNZ(op >>= 1);
}

template<AddressingModes mode, int waste>
void CPU::NOP() { pc += waste; }

template<AddressingModes mode>
void CPU::ORA()
{
	calcCrossed = true;
	UpdNZ(a |= GetOperand8<mode>());
}

template<AddressingModes mode>
void CPU::PHA()
{
	Tick();
	Push8(a);
}

template<AddressingModes mode>
void CPU::PHP()
{
	Tick();
	Push8(p | (1 << 4));
}

template<AddressingModes mode>
void CPU::PLA()
{
	Tick(2);
	a = Pop8();
	UpdNZ(a);
}

template<AddressingModes mode>
void CPU::PLP()
{
	Tick(2);
	p = Pop8();
	B(false);
	// TODO: What?
	U(true);
}

template<AddressingModes mode>
void CPU::ROL()
{
	Tick(CalcBaseTicks<mode>());
	Tick(CalcShiftTicks<mode>());
	u8 &mem = GetOperand8<mode>();
	u8 t = mem;
	mem <<= 1;
	mem |= static_cast<u8>(C());
	C(t & 0x80);
	UpdNZ(mem);
}

template<AddressingModes mode>
void CPU::ROR()
{
	Tick(CalcBaseTicks<mode>());
	Tick(CalcShiftTicks<mode>());
	u8 &mem = GetOperand8<mode>();
	u8 t = mem;
	mem >>= 1;
	mem |= (C() << 7);
	C(t & 1);
	UpdNZ(mem);
}


/**
This instruction transfers from the stack into the microprocessor
the processor status and the program counter location for the instruction
which was interrupted.By virtue of the interrupt having stored this data
before executing the instruction and the fact that the RTI reinitializes
the microprocessor to the same state as when it was interrupted, the
combination of interrupt plus RTI allows truly reentrant coding.
*/
template<AddressingModes mode>
void CPU::RTI()
{
	// NOTE: Changes the p register
	PLP<mode>(); // pop8 
	Tick(2);
	pc = Pop16(); // pop16
}

template<AddressingModes mode>
void CPU::RTS() { Tick(4); pc = Pop16() + 1; }

template<AddressingModes mode>
void CPU::SBC()
{
	Tick(CalcBaseTicks<mode>());
	calcCrossed = true;
	u8 lhs = a;
	u8 rhs = GetOperand8<mode>() ^ 0xff;
	s16 res = lhs + rhs + C();
	UpdCV(a, rhs, res);
	a = static_cast<u8>(res);
	UpdNZ(a);
}

template<AddressingModes mode>
void CPU::SEC() { C(1); }

template<AddressingModes mode>
void CPU::SED() { D(1); }

template<AddressingModes mode>
void CPU::SEI() { I(1); }

template<AddressingModes mode>
void CPU::STA()
{
	Tick(CalcBaseTicks<mode>());
	Tick(CalcStoreTicks<mode>());
	GetOperand8<mode>() = a;
}

template<AddressingModes mode>
void CPU::STX()
{
	Tick(CalcBaseTicks<mode>());
	Tick(CalcStoreTicks<mode>());
	GetOperand8<mode>() = x;
}

template<AddressingModes mode>
void CPU::STY()
{
	Tick(CalcBaseTicks<mode>());
	Tick(CalcStoreTicks<mode>());
	GetOperand8<mode>() = y;
}

template<AddressingModes mode>
void CPU::TAX() { x = a; UpdNZ(x); }

template<AddressingModes mode>
void CPU::TAY() { y = a; UpdNZ(y); }

template<AddressingModes mode>
void CPU::TSX() { x = sp; UpdNZ(x); }

template<AddressingModes mode>
void CPU::TXA() { a = x; UpdNZ(a); }

template<AddressingModes mode>
void CPU::TXS() { sp = x; }

template<AddressingModes mode>
void CPU::TYA() { a = y; UpdNZ(a); }

template<AddressingModes mode>
void CPU::LAX()
{
	LDA<mode>();
	TAX<mode>();
}

template<Interrupts inter>
void CPU::INT()
{
	// Note that BRK, although it is a one - byte instruction, needs an extra
	// byte of padding after it.This is because the return address it puts on
	// the stack will cause the RTI to put the program counter back not to the
	// very next byte after the BRK, but to the second byte after it. This
	// padding byte can be used for a signature byte to tell the BReaK interrupt
	// routine which BRK caused the particular software interrupt.
	pc++;

	Tick();

	if constexpr (inter != Interrupts::BRK)
	{
		Tick();
	}

	// The microprocessor uses the stack to save the reentrant 
	// or recovery code and then uses the interrupt vectors
	// FFFE and FFFF, (or FFFA and FFFB), depending on whether 
	// or not an interrupt request or a non maskable interrupt 
	// request had occurred
	if constexpr (inter != Interrupts::RST)
	{
		Push16(pc);
		Push8(p | ((Interrupts::BRK == inter) << 4));
	}
	else
	{
		s -= 3;
		Tick(3);
	}

	// It should he noted that the interrupt disable is turned on at this 
	// point by the micro - processor automatically.

	// BRK does set the interrupt-disable I flag like an IRQ does, and if
	// you have the CMOS 6502 (65C02), it will also clear the decimal D flag.

	I(true);

	constexpr u16 vect[] = { 0xFFFA, 0xFFFC, 0xFFFE, 0xFFFE };
	pc = Read16(vect[inter]);
	
	if (inter == NMI) 
		nmi = false;

}
