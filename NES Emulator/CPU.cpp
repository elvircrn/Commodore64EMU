#include "CPU.h"


CPU::CPU() : instrs(INSTR_MEM_SIZE), ram(RAM_SIZE)
{

}

CPU::CPU(uint16_t _rst) : rst(_rst)
{
}

CPU::~CPU()
{
}

void CPU::Tick()
{
	Execute();
}

void CPU::Execute()
{
	switch (instrs[pc++])
	{
		// Arithmetic expressions

		// ADC
		case 0x69: ADC<AddressingModes::IMMEDIATE>(); break;
		case 0x65: ADC<AddressingModes::ZERO_PAGE>(); break;
		case 0x75: ADC<AddressingModes::ZERO_PAGE_X>(); break;
		case 0x6D: ADC<AddressingModes::ABSOLUTE>(); break;
		case 0x7D: ADC<AddressingModes::ABSOLUTE_INDEXED_X>(); break;
		case 0x79: ADC<AddressingModes::ABSOLUTE_INDEXED_Y>(); break;
		case 0x71: ADC<AddressingModes::INDEXED_INDIRECT_X>(); break;
		case 0x61: ADC<AddressingModes::INDIRECT_INDEXED>(); break;

		// AND
		case 0x29: AND<AddressingModes::IMMEDIATE>(); break;
		case 0x25: AND<AddressingModes::ZERO_PAGE>(); break;
		case 0x35: AND<AddressingModes::ZERO_PAGE_X>(); break;
		case 0x2D: AND<AddressingModes::ABSOLUTE>(); break;
		case 0x3D: AND<AddressingModes::ABSOLUTE_INDEXED_X>(); break;
		case 0x39: AND<AddressingModes::ABSOLUTE_INDEXED_Y>(); break;
		case 0x21: AND<AddressingModes::INDEXED_INDIRECT_X>(); break;
		case 0x31: AND<AddressingModes::INDIRECT_INDEXED>(); break;

		// ASL

		
	}
}

#pragma region Register getters and setters

void CPU::SetA(uint8_t _a)
{
	a = _a;
}

void CPU::SetX(uint8_t _x)
{
	x = _x;
}

void CPU::SetY(uint8_t _y)
{
	y = _y;
}

void CPU::SetS(uint8_t _s)
{
	s = _s;
}

void CPU::SetP(uint8_t _p)
{
	p = _p;
}

void CPU::SetPC(uint16_t _pc)
{
	pc = _pc;
}

uint8_t CPU::GetA() const
{
	return a;
}

uint8_t CPU::GetX() const
{
	return x;
}

uint8_t CPU::GetY() const
{
	return y;
}

uint8_t CPU::GetS() const
{
	return s;
}

uint8_t CPU::GetP() const
{
	return p;
}

uint16_t CPU::GetPC() const
{
	return pc;
}

#pragma endregion

#pragma region Instruction helpers
template<AddressingModes mode>
u8& CPU::GetOperand()
{
	switch (mode)
	{
		// ADC #2 -> A + 2
		case AddressingModes::IMMEDIATE:
			buff = Imm();
			return buff;
		// ADC $3420 -> A + contents of memory $3420
		case AddressingModes::ABSOLUTE:
			return Read(Abs());
		// ADC $3420,X -> A + contents of memory $3420 + X
		case AddressingModes::ABSOLUTE_INDEXED_X:
			return Read(AbsX());
		// ADC $3420,Y	-> A + contents of memory $3420 + Y
		case AddressingModes::ABSOLUTE_INDEXED_Y:
			return Read(AbsY());
		// ADC $F6 -> A + contents of memory $F6
		case AddressingModes::ZERO_PAGE:
			return Read(Zp(Imm()));
		// ADC ($F6,X) -> A + contents of address at [$F6 + X]
		case AddressingModes::INDEXED_INDIRECT_X:
			return Zp(Imm() + x);
		// ADC ($F6),Y -> A + contents of (address at $F6) + offset Y
		case AddressingModes::INDIRECT_INDEXED:
			buff = Zp(Imm()) + y;
			return buff;
		case AddressingModes::ACCUMULATOR:
			return a;
	}
}
#pragma endregion

//.... add with carry
template<AddressingModes mode>
void CPU::ADC()
{
	u16 lhs = a;
	u16 rhs = GetOperand<mode>() + C();
	u16 res = lhs + rhs;
	a = res;
	UpdCV(lhs, rhs, res);
	UpdNZ(res);
}

//....	and (with accumulator)  
template<AddressingModes mode>
void CPU::AND() { UpdNZ(a &= GetOperand<mode>()); }

//....	arithmetic shift left  
template<AddressingModes mode>
void CPU::ASL() { UpdNZ(_ASL(GetOperand<mode>())); }

template<AddressingModes mode>
void CPU::BIT()
{
	u8 res = a & GetOperand<mode>();
	UpdNZ(res);
	V(res & (1 << 6));
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
void CPU::DEC() { UpdNZ(--GetOperand<mode>()); }

template<AddressingModes mode>
void CPU::DEX() { UpdateNZ(--x); }

template<AddressingModes mode>
void CPU::DEY() { UpdateNZ(--y); }

template<AddressingModes mode>
void CPU::EOR() { UpdNZ(a ^= GetOperand<mode>()); }

template<AddressingModes mode>
void CPU::INC() { UpdNZ(++GetOperand<mode>()); }

template<AddressingModes mode>
void CPU::INX() { UpdNZ(++x); }

template<AddressingModes mode>
void CPU::INY() { UpdNZ(++y); }

template<AddressingModes mode>
void CPU::LDA() { UpdNZ(a = GetOperand<mode>()); }

template<AddressingModes mode>
void CPU::LDX() { UpdNZ(x = GetOperand<mode>()); }

template<AddressingModes mode>
void CPU::LDY() { UpdNZ(y = GetOperand<mode>()); }

template<AddressingModes mode>
void CPU::LSR()
{
	UpdC(GetOperand<mode>() & 1);
	UpdNZ(GetOperand<mode>() <<= 1);
}

template<AddressingModes mode>
void CPU::NOP() { }

template<AddressingModes mode>
void CPU::ORA() { UpdNZ(a |= GetOperand<mode>()); }

template<AddressingModes mode>
void CPU::ROL()
{
	UpdC(a & 0x80);
	u8 res = _ROL(GetOperand<mode>());
	UpdNZ(res);
}

template<AddressingModes mode>
void CPU::ROR()
{
	UpdC(a & 0x80);
	u8 res = _ROR(GetOperand<mode>());
	UpdNZ(res);
}

template<AddressingModes mode>
void CPU::SBC()
{
	u16 lhs = a ^ 0xff;
	u16 rhs = GetOperand<mode>() + C();
	u16 res = lhs + rhs;
	a = res;
	UpdCV(lhs, rhs, res);
	UpdNZ(res);
}

template<AddressingModes mode>
void CPU::SEC() { C(1); }

template<AddressingModes mode>
void CPU::SED() { D(1); }

template<AddressingModes mode>
void CPU::SEI() { I(1); }

template<AddressingModes mode>
void CPU::STA() { GetOperand<mode>() = a; }

template<AddressingModes mode>
void CPU::STX() { GetOperand<mode>() = x; }

template<AddressingModes mode>
void CPU::STY() { GetOperand<mode>() = y; }

template<AddressingModes mode>
void CPU::TAX() { x = a; UpdNZ(x); }

template<AddressingModes mode>
void CPU::TAY() { y = a; UpdNZ(y); }

template<AddressingModes mode>
void CPU::TSX() { s = x; UpdNZ(s); }

template<AddressingModes mode>
void CPU::TXA() { x = a; UpdNZ(x); }

template<AddressingModes mode>
void CPU::TXS() { x = s; UpdNZ(x); }

template<AddressingModes mode>
void CPU::TYA() { y = a; UpdNZ(y); }



