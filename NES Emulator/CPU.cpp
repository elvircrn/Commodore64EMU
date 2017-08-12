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

//.... add with carry
template<AddressingModes mode>
void CPU::ADC()
{
	u16 lhs = a;
	u16 rhs = GetOperand<mode>() + C();
	u16 res = lhs + rhs;
	a = res;

	rhs = GetOperand<mode>();

	// TODO: Use constexpr-if after VS update
	switch (mode)
	{
		// ADC #2 -> A + #2
		case AddressingModes::IMMEDIATE:
			rhs = Imm() + C();
			res = lhs + rhs;
			a = res;
			break;
		// ADC $3420 -> A + contents of memory $3420
		case AddressingModes::ABSOLUTE:
			rhs = Abs() + C();
			res = lhs + rhs;
			a = res;
			break;
		// ADC $3420,X -> A + contents of memory $3420 + X
		case AddressingModes::ABSOLUTE_INDEXED_X:
			rhs = Read(AbsX()) + C();
			res = lhs + rhs;
			a = res;
			break;
		// ADC $3420,Y	-> A + contents of memory $3420 + Y
		case AddressingModes::ABSOLUTE_INDEXED_Y:
			rhs = Read(AbsY()) + C();
			res = lhs + rhs;
			a = res;
			break;
		// ADC $F6 -> A + contents of memory $F6
		case AddressingModes::ZERO_PAGE:
			rhs = Read(Zp(Imm())) + C();
			res = lhs + rhs;
			a = res;
			break;
		// ADC ($F6,X) -> A + contents of address at [$F6 + X]
		case AddressingModes::INDEXED_INDIRECT_X:
			rhs = Zp(Imm() + x) + C();
			res = lhs + rhs;
			a = res;
			break;
		// ADC ($F6),Y -> A + contents of (address at $F6) + offset Y
		case AddressingModes::INDIRECT_INDEXED:
			rhs = Zp(Imm()) + y + C();
			res = lhs + rhs;
			break;
	}

	UpdCV(lhs, rhs, res);
	UpdNZ(res);
}

template<AddressingModes mode>
u16 CPU::GetOperand()
{
	switch (mode)
	{
		case AddressingModes::IMMEDIATE:
			return Imm();
		// ADC $3420 -> A + contents of memory $3420
		case AddressingModes::ABSOLUTE:
			return Abs() + C();
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
			return Zp(Imm()) + y;
	}
}

//....	and (with accumulator)  
void CPU::AND()
{

}
//....	arithmetic shift left  
void CPU::ASL()
{

}
//....	branch on carry clear  
void CPU::BCC()
{

}
//....	branch on carry set  
void CPU::BCS()
{

}
//....	branch on equal (zero set)  
void CPU::BEQ()
{

}
//....	bit test  
void CPU::BIT()
{

}
//....	branch on minus (negative set)  
void CPU::BMI()
{

}
//....	branch on not equal (zero clear)  
void CPU::BNE()
{

}
//....	branch on plus (negative clear)  
void CPU::BPL()
{

}
//....	interrupt  
void CPU::BRK()
{

}
//....	branch on overflow clear  
void CPU::BVC()
{

}
//....	branch on overflow set  
void CPU::BVS()
{

}
//....	clear carry  
void CPU::CLC()
{

}
//....	clear decimal  
void CPU::CLD()
{

}
//....	clear interrupt disable  
void CPU::CLI()
{

}
//....	clear overflow  
void CPU::CLV()
{

}
//....	compare (with accumulator)  
void CPU::CMP()
{

}
//....	compare with X  
void CPU::CPX()
{

}
//....	compare with Y  
void CPU::CPY()
{

}
//....	decrement  
void CPU::DEC()
{

}
//....	decrement X  
void CPU::DEX()
{

}
//....	decrement Y  
void CPU::DEY()
{

}
//....	exclusive or (with accumulator)  
void CPU::EOR()
{

}
//....	increment  
void CPU::INC()
{

}
//....	increment X  
void CPU::INX()
{

}
//....	increment Y 
void CPU::INY()
{

}
//....	jump  
void CPU::JMP()
{

}
//....	jump subroutine  
void CPU::JSR()
{

}
//....	load accumulator  
void CPU::LDA()
{

}

//....	load X  
void CPU::LDX()
{

}

//....	load Y
void CPU::LDY()
{

}
//....	logical shift right  
void CPU::LSR()
{

}
//....	no operation  
void CPU::NOP()
{

}
//....	or with accumulator  
void CPU::ORA()
{

}
//....	push accumulator  
void CPU::PHA()
{

}
//....	push processor status (SR)  
void CPU::PHP()
{

}
//....	pull accumulator  
void CPU::PLA()
{

}
//....	pull processor status (SR)  
void CPU::PLP()
{

}
//....	rotate left  
void CPU::ROL()
{

}
//....	rotate right  
void CPU::ROR()
{

}
//....	return from interrupt  
void CPU::RTI()
{

}
//....	return from subroutine  
void CPU::RTS()
{

}
//....	subtract with carry  
void CPU::SBC()
{

}
//....	set carry  
void CPU::SEC()
{

}
//....	set decimal  
void CPU::SED()
{

}
//....	set interrupt disable  
void CPU::SEI()
{

}
//....	store accumulator  
void CPU::STA()
{

}
//....	store X  
void CPU::STX()
{

}
//....	store Y  
void CPU::STY()
{

}
//....	transfer accumulator to X  
void CPU::TAX()
{

}
//....	transfer accumulator to Y  
void CPU::TAY()
{

}
//....	transfer stack pointer to X  
void CPU::TSX()
{

}
//....	transfer X to accumulator  
void CPU::TXA()
{

}
//....	transfer X to stack pointer  
void CPU::TXS()
{

}
//....	transfer Y to accumulator  
void CPU::TYA()
{

}

