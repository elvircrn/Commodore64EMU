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

void CPU::Tick() { }

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
		case 0xB6: LDX<AddressingModes::ZERO_PAGE>(); break;
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
	sp = _s;
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
	return sp;
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
		// TODO: Check if -1 is needed
		case AddressingModes::RELATIVE:
			buff = pc + Imm();
			return buff;
		// TODO: Check if -2 is needed
		case AddressingModes::INDIRECT:
			return Read(Abs());
		default:
			throw "Wrong addressing mode implemented!";
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

// TODO: Consider refactoring into a non-templated function
template<AddressingModes mode>
void CPU::BCC()
{
	u8 loc = GetOperand<mode>();
	if (!C())
		pc = loc;
}

template<AddressingModes mode>
void CPU::BCS()
{
	u8 loc = GetOperand<mode>();
	pc = C() ? loc : pc;
}

template<AddressingModes mode>
void CPU::BEQ()
{
	u8 loc = GetOperand<mode>();
	pc = Z() ? loc : pc;
}

template<AddressingModes mode>
void CPU::BIT()
{
	u8 res = a & GetOperand<mode>();
	UpdNZ(res);
	V(res & (1 << 6));
}

template<AddressingModes mode>
void CPU::BMI()
{
	u8 loc = GetOperand<mode>();
	pc = N() ? loc : pc;
}

template<AddressingModes mode>
void CPU::BNE()
{
	u8 loc = GetOperand<mode>();
	pc = !Z() ? loc : pc;
}

template<AddressingModes mode>
void CPU::BPL()
{
	u8 loc = GetOperand<mode>();
	pc = !N() ? loc : pc;
}

// TODO: Consider implementing
// NOTE: Mostly used for debugging.
template<AddressingModes mode>
void CPU::BRK()
{
	
}

template<AddressingModes mode>
void CPU::BVC()
{
	u8 loc = GetOperand<mode>();
	pc = !V() ? loc : pc;
}

template<AddressingModes mode>
void CPU::BVS()
{
	u8 loc = GetOperand<mode>();
	pc = V() ? loc : pc;
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
	u8 mem = GetOperand<mode>();
	UpdNZ(a - mem);
	C(a >= mem);
}

template<AddressingModes mode>
void CPU::CPX()
{
	u8 mem = GetOperand<mode>();
	UpdNZ(x - mem);
	C(x >= mem);
}

template<AddressingModes mode>
void CPU::CPY()
{
	u8 mem = GetOperand<mode>();
	UpdNZ(y - mem);
	C(y >= mem);
}

template<AddressingModes mode>
void CPU::DEC() { UpdNZ(--GetOperand<mode>()); }

template<AddressingModes mode>
void CPU::DEX() { UpdNZ(--x); }

template<AddressingModes mode>
void CPU::DEY() { UpdNZ(--y); }

template<AddressingModes mode>
void CPU::EOR() { UpdNZ(a ^= GetOperand<mode>()); }

template<AddressingModes mode>
void CPU::INC() { UpdNZ(++GetOperand<mode>()); }

template<AddressingModes mode>
void CPU::INX() { UpdNZ(++x); }

template<AddressingModes mode>
void CPU::INY() { UpdNZ(++y); }

template<AddressingModes mode>
void CPU::JMP() { pc = GetOperand<mode>(); }

// TODO: Check if pc - 1 or pc + 1
template<AddressingModes mode>
void CPU::JSR()
{
	Push16(pc + 1);
	pc = Abs();
}

template<AddressingModes mode>
void CPU::LDA() { UpdNZ(a = GetOperand<mode>()); }

template<AddressingModes mode>
void CPU::LDX() { UpdNZ(x = GetOperand<mode>()); }

template<AddressingModes mode>
void CPU::LDY() { UpdNZ(y = GetOperand<mode>()); }

template<AddressingModes mode>
void CPU::LSR()
{
	C(GetOperand<mode>() & 1);
	UpdNZ(GetOperand<mode>() <<= 1);
}

template<AddressingModes mode>
void CPU::NOP() { }

template<AddressingModes mode>
void CPU::ORA() { UpdNZ(a |= GetOperand<mode>()); }

template<AddressingModes mode>
void CPU::PHA()
{
	Push8(a);
	pc++;
}

template<AddressingModes mode>
void CPU::PHP()
{
	Push8(p);
	pc++;
}

template<AddressingModes mode>
void CPU::PLA()
{
	a = Pop8();
	pc++;
	UpdNZ(a);
}

template<AddressingModes mode>
void CPU::PLP()
{
	p = Pop8();
	pc++;
}

template<AddressingModes mode>
void CPU::ROL()
{
	C(a & 0x80);
	u8 res = _ROL(GetOperand<mode>());
	UpdNZ(res);
}

template<AddressingModes mode>
void CPU::ROR()
{
	C(a & 0x80);
	u8 res = _ROR(GetOperand<mode>());
	UpdNZ(res);
}

template<AddressingModes mode>
void CPU::RTI()
{
	p = Pop8();
	pc = Pop16();
}

template<AddressingModes mode>
void CPU::RTS()
{
	pc = Pop16() + 1;
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
void CPU::TSX() { sp = x; UpdNZ(sp); }

template<AddressingModes mode>
void CPU::TXA() { x = a; UpdNZ(x); }

template<AddressingModes mode>
void CPU::TXS() { x = sp; UpdNZ(x); }

template<AddressingModes mode>
void CPU::TYA() { y = a; UpdNZ(y); }



