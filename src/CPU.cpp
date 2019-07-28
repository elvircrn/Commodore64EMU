#include "CPU.h"
#include "Instructions.h"

#include <array>
#include <sstream>

CPU::CPU(Clock &_clock, MMU &_mmu)
		: clock(_clock), mmu(_mmu), cycleCount{} {
	init();
}

CPU::CPU(Clock &_clock, MMU &_mmu, u16 _pc)
		: clock(_clock), mmu(_mmu), cycleCount{} {
	init(_pc);
}

CPU::~CPU() = default;

void CPU::init() {
	p = 0x24; // IRQ disabled
	a = 0;
	x = 0;
	y = 0;
	sp = 0xfd;
	pc = read16(vect[Interrupts::RST]);
}

void CPU::init(u16 _pc) {
	init();
	pc = _pc;
}

#include "LogUtil.h"
#include <CPU.h>

void CPU::execute() {
	clock.waitTick();
	clear();
	tick(2);

	u8 op = read(pc++);
	if (debug && DEBUG) {
		opHist.push_front(op);
		if (opHist.size() == 100) {
			opHist.pop_back();
		}
		if (pcHist.size() == 100) {
			opHist.pop_back();
		}
		if (instrHist.size() == 100) {
			opHist.pop_front();
		}
		pcHist.push_front(pc - 1);
		bitStack.clear();
		instrHist.emplace_back(pc,
													 Instructions::Name(read(pc - 1)),
													 std::array<u8, 4>({read(pc - 1), read(pc), read(pc + 1), read(pc + 2)}));
		L_DEBUG(std::cout << Instructions::Name(read(pc - 1)) << ' ' << std::hex << ' ' << (int) read(pc - 1) << ' '
											<< (int) read(
													pc) << ' ' << (int) read(
				pc + 1) << ' ' << getNESTestLine() << '\n');
		for (int i = 0; i < 5; i++)
			bitStack.push_back(read(pc - 1 + i));
	}

	if (getNMI()) {
		pc--;
		INT<Interrupts::NMI>();
	} else {
		switch (op) {
		case 0x69: ADC<AddressingModes::IMMEDIATE>();
			break;
		case 0x65: ADC<AddressingModes::ZERO_PAGE>();
			break;
		case 0x75: ADC<AddressingModes::ZERO_PAGE_X>();
			break;
		case 0x6D: ADC<AddressingModes::ABSOLUTE>();
			break;
		case 0x7D: ADC<AddressingModes::ABSOLUTE_INDEXED_X>();
			break;
		case 0x79: ADC<AddressingModes::ABSOLUTE_INDEXED_Y>();
			break;
		case 0x61: ADC<AddressingModes::INDEXED_INDIRECT_X>();
			break;
		case 0x71: ADC<AddressingModes::INDIRECT_INDEXED>();
			break;
		case 0x29: AND<AddressingModes::IMMEDIATE>();
			break;
		case 0x25: AND<AddressingModes::ZERO_PAGE>();
			break;
		case 0x35: AND<AddressingModes::ZERO_PAGE_X>();
			break;
		case 0x2D: AND<AddressingModes::ABSOLUTE>();
			break;
		case 0x3D: AND<AddressingModes::ABSOLUTE_INDEXED_X>();
			break;
		case 0x39: AND<AddressingModes::ABSOLUTE_INDEXED_Y>();
			break;
		case 0x21: AND<AddressingModes::INDEXED_INDIRECT_X>();
			break;
		case 0x31: AND<AddressingModes::INDIRECT_INDEXED>();
			break;
		case 0x0A: ASL<AddressingModes::ACCUMULATOR>();
			break;
		case 0x06: ASL<AddressingModes::ZERO_PAGE>();
			break;
		case 0x16: ASL<AddressingModes::ZERO_PAGE_X>();
			break;
		case 0x0E: ASL<AddressingModes::ABSOLUTE>();
			break;
		case 0x1E: ASL<AddressingModes::ABSOLUTE_INDEXED_X>();
			break;
		case 0x24: BIT<AddressingModes::ZERO_PAGE>();
			break;
		case 0x2C: BIT<AddressingModes::ABSOLUTE>();
			break;
		case 0x10: BPL<AddressingModes::RELATIVE>();
			break;
		case 0x30: BMI<AddressingModes::RELATIVE>();
			break;
		case 0x50: BVC<AddressingModes::RELATIVE>();
			break;
		case 0x70: BVS<AddressingModes::RELATIVE>();
			break;
		case 0x90: BCC<AddressingModes::RELATIVE>();
			break;
		case 0xB0: BCS<AddressingModes::RELATIVE>();
			break;
		case 0xD0: BNE<AddressingModes::RELATIVE>();
			break;
		case 0xF0: BEQ<AddressingModes::RELATIVE>();
			break;
		case 0x00: BRK<AddressingModes::IMPLIED>();
			break;
		case 0xC9: CMP<AddressingModes::IMMEDIATE>();
			break;
		case 0xC5: CMP<AddressingModes::ZERO_PAGE>();
			break;
		case 0xD5: CMP<AddressingModes::ZERO_PAGE_X>();
			break;
		case 0xCD: CMP<AddressingModes::ABSOLUTE>();
			break;
		case 0xDD: CMP<AddressingModes::ABSOLUTE_INDEXED_X>();
			break;
		case 0xD9: CMP<AddressingModes::ABSOLUTE_INDEXED_Y>();
			break;
		case 0xC1: CMP<AddressingModes::INDEXED_INDIRECT_X>();
			break;
		case 0xD1: CMP<AddressingModes::INDIRECT_INDEXED>();
			break;
		case 0xE0: CPX<AddressingModes::IMMEDIATE>();
			break;
		case 0xE4: CPX<AddressingModes::ZERO_PAGE>();
			break;
		case 0xEC: CPX<AddressingModes::ABSOLUTE>();
			break;
		case 0xC0: CPY<AddressingModes::IMMEDIATE>();
			break;
		case 0xC4: CPY<AddressingModes::ZERO_PAGE>();
			break;
		case 0xCC: CPY<AddressingModes::ABSOLUTE>();
			break;
		case 0xC6: DEC<AddressingModes::ZERO_PAGE>();
			break;
		case 0xD6: DEC<AddressingModes::ZERO_PAGE_X>();
			break;
		case 0xCE: DEC<AddressingModes::ABSOLUTE>();
			break;
		case 0xDE: DEC<AddressingModes::ABSOLUTE_INDEXED_X>();
			break;
		case 0x49: EOR<AddressingModes::IMMEDIATE>();
			break;
		case 0x45: EOR<AddressingModes::ZERO_PAGE>();
			break;
		case 0x55: EOR<AddressingModes::ZERO_PAGE_X>();
			break;
		case 0x4D: EOR<AddressingModes::ABSOLUTE>();
			break;
		case 0x5D: EOR<AddressingModes::ABSOLUTE_INDEXED_X>();
			break;
		case 0x59: EOR<AddressingModes::ABSOLUTE_INDEXED_Y>();
			break;
		case 0x41: EOR<AddressingModes::INDEXED_INDIRECT_X>();
			break;
		case 0x51: EOR<AddressingModes::INDIRECT_INDEXED>();
			break;
		case 0x18: CLC<AddressingModes::IMPLIED>();
			break;
		case 0x38: SEC<AddressingModes::IMPLIED>();
			break;
		case 0x58: CLI<AddressingModes::IMPLIED>();
			break;
		case 0x78: SEI<AddressingModes::IMPLIED>();
			break;
		case 0xB8: CLV<AddressingModes::IMPLIED>();
			break;
		case 0xD8: CLD<AddressingModes::IMPLIED>();
			break;
		case 0xF8: SED<AddressingModes::IMPLIED>();
			break;
		case 0xE6: INC<AddressingModes::ZERO_PAGE>();
			break;
		case 0xF6: INC<AddressingModes::ZERO_PAGE_X>();
			break;
		case 0xEE: INC<AddressingModes::ABSOLUTE>();
			break;
		case 0xFE: INC<AddressingModes::ABSOLUTE_INDEXED_X>();
			break;
		case 0x4C: JMP<AddressingModes::ABSOLUTE>();
			break;
		case 0x6C: JMP<AddressingModes::INDIRECT>();
			break;
		case 0x20: JSR<AddressingModes::ABSOLUTE>();
			break;
		case 0xA9: LDA<AddressingModes::IMMEDIATE>();
			break;
		case 0xA5: LDA<AddressingModes::ZERO_PAGE>();
			break;
		case 0xB5: LDA<AddressingModes::ZERO_PAGE_X>();
			break;
		case 0xAD: LDA<AddressingModes::ABSOLUTE>();
			break;
		case 0xBD: LDA<AddressingModes::ABSOLUTE_INDEXED_X>();
			break;
		case 0xB9: LDA<AddressingModes::ABSOLUTE_INDEXED_Y>();
			break;
		case 0xA1: LDA<AddressingModes::INDEXED_INDIRECT_X>();
			break;
		case 0xB1: LDA<AddressingModes::INDIRECT_INDEXED>();
			break;
		case 0xA2: LDX<AddressingModes::IMMEDIATE>();
			break;
		case 0xA6: LDX<AddressingModes::ZERO_PAGE>();
			break;
		case 0xB6: LDX<AddressingModes::ZERO_PAGE_Y>();
			break;
		case 0xAE: LDX<AddressingModes::ABSOLUTE>();
			break;
		case 0xBE: LDX<AddressingModes::ABSOLUTE_INDEXED_Y>();
			break;
		case 0xA0: LDY<AddressingModes::IMMEDIATE>();
			break;
		case 0xA4: LDY<AddressingModes::ZERO_PAGE>();
			break;
		case 0xB4: LDY<AddressingModes::ZERO_PAGE_X>();
			break;
		case 0xAC: LDY<AddressingModes::ABSOLUTE>();
			break;
		case 0xBC: LDY<AddressingModes::ABSOLUTE_INDEXED_X>();
			break;
		case 0x4A: LSR<AddressingModes::ACCUMULATOR>();
			break;
		case 0x46: LSR<AddressingModes::ZERO_PAGE>();
			break;
		case 0x56: LSR<AddressingModes::ZERO_PAGE_X>();
			break;
		case 0x4E: LSR<AddressingModes::ABSOLUTE>();
			break;
		case 0x5E: LSR<AddressingModes::ABSOLUTE_INDEXED_X>();
			break;
		case 0x09: ORA<AddressingModes::IMMEDIATE>();
			break;
		case 0x05: ORA<AddressingModes::ZERO_PAGE>();
			break;
		case 0x15: ORA<AddressingModes::ZERO_PAGE_X>();
			break;
		case 0x0D: ORA<AddressingModes::ABSOLUTE>();
			break;
		case 0x1D: ORA<AddressingModes::ABSOLUTE_INDEXED_X>();
			break;
		case 0x19: ORA<AddressingModes::ABSOLUTE_INDEXED_Y>();
			break;
		case 0x01: ORA<AddressingModes::INDEXED_INDIRECT_X>();
			break;
		case 0x11: ORA<AddressingModes::INDIRECT_INDEXED>();
			break;
		case 0xAA: TAX<AddressingModes::IMPLIED>();
			break;
		case 0x8A: TXA<AddressingModes::IMPLIED>();
			break;
		case 0xCA: DEX<AddressingModes::IMPLIED>();
			break;
		case 0xE8: INX<AddressingModes::IMPLIED>();
			break;
		case 0xA8: TAY<AddressingModes::IMPLIED>();
			break;
		case 0x98: TYA<AddressingModes::IMPLIED>();
			break;
		case 0x88: DEY<AddressingModes::IMPLIED>();
			break;
		case 0xC8: INY<AddressingModes::IMPLIED>();
			break;
		case 0x2A: ROL<AddressingModes::ACCUMULATOR>();
			break;
		case 0x26: ROL<AddressingModes::ZERO_PAGE>();
			break;
		case 0x36: ROL<AddressingModes::ZERO_PAGE_X>();
			break;
		case 0x2E: ROL<AddressingModes::ABSOLUTE>();
			break;
		case 0x3E: ROL<AddressingModes::ABSOLUTE_INDEXED_X>();
			break;
		case 0x6A: ROR<AddressingModes::ACCUMULATOR>();
			break;
		case 0x66: ROR<AddressingModes::ZERO_PAGE>();
			break;
		case 0x76: ROR<AddressingModes::ZERO_PAGE_X>();
			break;
		case 0x6E: ROR<AddressingModes::ABSOLUTE>();
			break;
		case 0x7E: ROR<AddressingModes::ABSOLUTE_INDEXED_X>();
			break;
		case 0x40: RTI<AddressingModes::IMPLIED>();
			break;
		case 0x60: RTS<AddressingModes::IMPLIED>();
			break;
		case 0xE9: SBC<AddressingModes::IMMEDIATE>();
			break;
		case 0xE5: SBC<AddressingModes::ZERO_PAGE>();
			break;
		case 0xF5: SBC<AddressingModes::ZERO_PAGE_X>();
			break;
		case 0xED: SBC<AddressingModes::ABSOLUTE>();
			break;
		case 0xFD: SBC<AddressingModes::ABSOLUTE_INDEXED_X>();
			break;
		case 0xF9: SBC<AddressingModes::ABSOLUTE_INDEXED_Y>();
			break;
		case 0xE1: SBC<AddressingModes::INDEXED_INDIRECT_X>();
			break;
		case 0xF1: SBC<AddressingModes::INDIRECT_INDEXED>();
			break;
		case 0x85: STA<AddressingModes::ZERO_PAGE>();
			break;
		case 0x95: STA<AddressingModes::ZERO_PAGE_X>();
			break;
		case 0x8D: STA<AddressingModes::ABSOLUTE>();
			break;
		case 0x9D: STA<AddressingModes::ABSOLUTE_INDEXED_X>();
			break;
		case 0x99: STA<AddressingModes::ABSOLUTE_INDEXED_Y>();
			break;
		case 0x81: STA<AddressingModes::INDEXED_INDIRECT_X>();
			break;
		case 0x91: STA<AddressingModes::INDIRECT_INDEXED>();
			break;
		case 0x86: STX<AddressingModes::ZERO_PAGE>();
			break;
		case 0x96: STX<AddressingModes::ZERO_PAGE_Y>();
			break;
		case 0x8E: STX<AddressingModes::ABSOLUTE>();
			break;
		case 0x84: STY<AddressingModes::ZERO_PAGE>();
			break;
		case 0x94: STY<AddressingModes::ZERO_PAGE_X>();
			break;
		case 0x8C: STY<AddressingModes::ABSOLUTE>();
			break;
		case 0x9A: TXS<AddressingModes::IMPLIED>();
			break;
		case 0xBA: TSX<AddressingModes::IMPLIED>();
			break;
		case 0x48: PHA<AddressingModes::IMPLIED>();
			break;
		case 0x68: PLA<AddressingModes::IMPLIED>();
			break;
		case 0x08: PHP<AddressingModes::IMPLIED>();
			break;
		case 0x28: PLP<AddressingModes::IMPLIED>();
			break;
		case 0xEA: NOP<AddressingModes::IMPLIED>();
			break;
		default: std::stringstream ss;
			if constexpr (DEBUG && !ignoreUnknownInstr) {
				for (int i = pcHist.back(); i < pcHist.back() + 10; i++)
					ss << std::hex << (int) read(i) << ' ';
				debugDump();
				L_ERROR(std::cout << "Instruction not found at pc: " + std::to_string(pc) + "\nDump: " + ss.str() << '\n');
			}
		}
	}

	tick(zeroPageCrossed & calcCrossed);
	cycleCount += 4;
}

#pragma region Debug and Logging
void CPU::debugDump() {
	for (int i = 0xfff0; i < 0x10000; i++)
		vectors.emplace_back(HexString(i), read(static_cast<u16>(i)));
}
#pragma endregion

#pragma region Register getters and setters

#pragma endregion

// Used for instructions that only use the operand for reading
template<AddressingModes mode>
u16 CPU::getPureOperand() {
	if constexpr (mode < 2)
		return getOperand16<mode>();
	else
		return getOperand8<mode>();
}

template<AddressingModes mode>
u16 CPU::getOperand16() {
	// Used by branch instructions
	// TODO: Check if -1 is needed; Probably not?
	if constexpr (mode == AddressingModes::RELATIVE) {
		u8 imm = Imm();
		return pc + (s8) imm;
	}
		// TODO: Check if -2 is needed
	else if (mode == AddressingModes::INDIRECT)
		return ind(Abs());
}

template<AddressingModes mode>
bool CPU::writeOperandVal(const u8 &val) {
	// ADC $3420 -> A + contents of memory $3420
	if constexpr (mode == AddressingModes::ABSOLUTE) {
		u16 abs = Abs();
		return write(abs, val);
	}
		// ADC #2 -> A + 2
	else if (mode == AddressingModes::IMMEDIATE) {
//		throw "Attempting to assign to immediate value";
	}
		// ADC $3420,X -> A + contents of memory $3420 + X
	else if (mode == AddressingModes::ABSOLUTE_INDEXED_X) {
		return write(AbsX(), val);
	}
		// ADC $3420,Y	-> A + contents of memory $3420 + Y
	else if (mode == AddressingModes::ABSOLUTE_INDEXED_Y) {
		return write(AbsY(), val);
	}
		// ADC $F6 -> A + contents of memory $F6
		// Zero page only considers the low bytes. TODO: Refactor
	else if (mode == AddressingModes::ZERO_PAGE) {
		return write(Imm() & 0xFFu, val);
	} else if (mode == AddressingModes::ZERO_PAGE_X) {
		return write((Imm() + x) & 0xFFu, val);
	} else if (mode == AddressingModes::ZERO_PAGE_Y) {
		return write((Imm() + y) & 0xFFu, val);
	}
		// https://www.csh.rit.edu/~moffitt/6502.html#ADDR-IIND
	else if (mode == AddressingModes::INDEXED_INDIRECT_X) {
		u8 addr8 = Imm() + x;
		u16 zp16 = Zp16(addr8);
		return write(zp16, val);
	} else if (mode == AddressingModes::INDIRECT_INDEXED) {
		u16 zp16 = Zp16(Imm());
		CrossesPage(zp16, y);
		return write(zp16 + y, val);
	} else if (mode == AddressingModes::ACCUMULATOR) {
		a = val;
	}
	// ADC ($F6),Y -> A + contents of (address at $F6) + offset Y
	//case AddressingModes::INDIRECT_INDEXED:
	//return Zp(Imm()) + y;
	return false;
}

template<AddressingModes mode>
u8 CPU::getOperand8() {
	// ADC $3420 -> A + contents of memory $3420
	if constexpr (mode == AddressingModes::ABSOLUTE) {
		u16 abs = Abs();
		return read(abs);
		// ADC #2 -> A + 2
	} else if constexpr (mode == AddressingModes::IMMEDIATE) {
		buff8 = Imm();
		return buff8;
		// ADC $3420,X -> A + contents of memory $3420 + X
	} else if constexpr (mode == AddressingModes::ABSOLUTE_INDEXED_X) {
		return read(AbsX());
		// ADC $3420,Y	-> A + contents of memory $3420 + Y
	} else if constexpr (mode == AddressingModes::ABSOLUTE_INDEXED_Y) {
		return read(AbsY());
		// ADC $F6 -> A + contents of memory $F6
	} else if constexpr (mode == AddressingModes::ZERO_PAGE) {
		return read(Imm() & 0xffu);
	} else if constexpr  (mode == AddressingModes::ZERO_PAGE_X) {
		return read((Imm() + x) & 0xffu);
	} else if constexpr (mode == AddressingModes::ZERO_PAGE_Y) {
		return read((Imm() + y) & 0xffu);
		// https://www.csh.rit.edu/~moffitt/6502.html#ADDR-IIND
	} else if constexpr  (mode == AddressingModes::INDEXED_INDIRECT_X) {
		u8 addr8 = Imm() + x;
		u16 zp16 = Zp16(addr8);
		return read(zp16);
	} else if constexpr (mode == AddressingModes::INDIRECT_INDEXED) {
		u16 zp16 = Zp16(Imm());
		CrossesPage(zp16, y);
		return read(zp16 + y);
	} else if constexpr (mode == AddressingModes::ACCUMULATOR) {
		return a;
	}
}
#pragma endregion

template<AddressingModes mode>
void CPU::ADC() {
	tick(CalcBaseTicks<mode>());
	calcCrossed = true;

	u16 res{};
	u8 lhs = a;
	u8 rhs = getOperand8<mode>();
	if (D()) {
		res = (lhs & 0xfu) + (rhs & 0xfu) + C();
		if (res > 0x9u) {
			res += 0x6u;
		}
		res += (lhs & 0xf0u) + (rhs & 0xf0u);
		if ((res & 0x1f0u) > 0x90u) {
			res += 0x60u;
		}
	} else {
		res = lhs + rhs + C();
	}

	UpdCV(lhs, rhs, res);

	a = res & 0xffu;
	UpdNZ(a);
}

template<AddressingModes mode>
void CPU::AND() {
	calcCrossed = true;
	UpdNZ(a &= getPureOperand<mode>());
}

template<AddressingModes mode>
void CPU::ASL() {
	tick(CalcBaseTicks<mode>());
	tick(CalcShiftTicks<mode>());
	u8 val = getOperand8<mode>();
	val = _ASL(val);
	writeOperandVal<mode>(val);
	UpdNZ(val);
}

template<AddressingModes mode>
void CPU::BCC() {
	u16 loc = getPureOperand<mode>();
	calcCrossed = true;
	if (!C()) {
		pc = loc;
	}
}

template<AddressingModes mode>
void CPU::BCS() {
	u16 loc = getPureOperand<mode>();
	calcCrossed = true;
	if (C()) {
		tick();
		pc = loc;
	}
}

template<AddressingModes mode>
void CPU::BEQ() {
	u16 loc = getPureOperand<mode>();
	calcCrossed = true;
	if (Z()) {
		tick();
		pc = loc;
	}
}

template<AddressingModes mode>
void CPU::BIT() {
	u8 op = (u8) getPureOperand<mode>();
	u8 res = a & op;
	Z(!res);
	N(op & (1u << 7u));
	V(op & (1u << 6u));
}

template<AddressingModes mode>
void CPU::BMI() {
	u16 loc = getPureOperand<mode>();
	calcCrossed = true;
	if (N()) {
		tick();
		pc = loc;
	}
}

template<AddressingModes mode>
void CPU::BNE() {
	u16 loc = getPureOperand<mode>();
	calcCrossed = true;
	if (!Z()) {
		tick();
		pc = loc;
	}
}

template<AddressingModes mode>
void CPU::BPL() {
	u16 loc = getPureOperand<mode>();
	calcCrossed = true;
	if (!N()) {
		tick();
		pc = loc;
	}
}

// TODO: Consider implementing
// NOTE: Mostly used for debugging.
template<AddressingModes mode>
void CPU::BRK() {
#ifdef TEST_MODE
	if (!I()) {
		INT<Interrupts::BRK>();
	}
#endif
}

template<AddressingModes mode>
void CPU::BVC() {
	u16 loc = getPureOperand<mode>();
	calcCrossed = true;
	if (!V()) {
		tick();
		pc = loc;
	}
}

template<AddressingModes mode>
void CPU::BVS() {
	u16 loc = getPureOperand<mode>();
	calcCrossed = true;
	if (V()) {
		tick();
		pc = loc;
	}
}

template<AddressingModes mode>
void CPU::CLC() { C(false); }

template<AddressingModes mode>
void CPU::CLD() { D(false); }

template<AddressingModes mode>
void CPU::CLI() {
	I(false);
}

template<AddressingModes mode>
void CPU::CLV() { V(false); }

template<AddressingModes mode>
void CPU::CMP() {
	calcCrossed = true;
	u8 mem = (u8) getPureOperand<mode>();
	UpdNZ(a - mem);
	C(a >= mem);
}

template<AddressingModes mode>
void CPU::CPX() {
	calcCrossed = true;
	u8 mem = (u8) getPureOperand<mode>();
	UpdNZ(x - mem);
	C(x >= mem);
}

template<AddressingModes mode>
void CPU::CPY() {
	calcCrossed = true;
	u8 mem = (u8) getPureOperand<mode>();
	UpdNZ(y - mem);
	C(y >= mem);
}

template<AddressingModes mode>
void CPU::DEC() {
	tick(CalcBaseTicks<mode>() + CalcIncDecTicks<mode>());
	u8 val = getOperand8<mode>();
	val--;
	writeOperandVal<mode>(val);
	UpdNZ(val);
}

// No additional cycles
template<AddressingModes mode>
void CPU::DEX() { UpdNZ(--x); }

// No additional cycles
template<AddressingModes mode>
void CPU::DEY() { UpdNZ(--y); }

template<AddressingModes mode>
void CPU::EOR() {
	tick(CalcBaseTicks<mode>());
	calcCrossed = true;
	UpdNZ(a ^= getPureOperand<mode>());
}

template<AddressingModes mode>
void CPU::INC() {
	tick(CalcBaseTicks<mode>() + CalcIncDecTicks<mode>());
	u8 val = getOperand8<mode>();
	val++;
	writeOperandVal<mode>(val);
	UpdNZ(val);
}

template<AddressingModes mode>
void CPU::INX() {
	UpdNZ(++x);
}

template<AddressingModes mode>
void CPU::INY() { UpdNZ(++y); }

// TODO: Check if Abs should read memory
template<AddressingModes mode>
void CPU::JMP() {
	if (mode == AddressingModes::ABSOLUTE) {
		tick();
		pc = Abs();
	} else if (mode == AddressingModes::INDIRECT)
		pc = ind(Abs()); // Ind calls tick
}

// TODO: Check if pc - 1 or pc + 1
template<AddressingModes mode>
void CPU::JSR() {
	Push16(pc + 1);
	pc = Abs();
	tick(4);
}

template<AddressingModes mode>
void CPU::LDA() {
	calcCrossed = true;
	tick(CalcBaseTicks<mode>());
	UpdNZ(a = getOperand8<mode>());
}

template<AddressingModes mode>
void CPU::LDX() {
	tick(CalcBaseTicks<mode>());
	calcCrossed = true;
	UpdNZ(x = getOperand8<mode>());
}

template<AddressingModes mode>
void CPU::LDY() {
	tick(CalcBaseTicks<mode>());
	calcCrossed = true;
	UpdNZ(y = (u8) getPureOperand<mode>());
}

template<AddressingModes mode>
void CPU::LSR() {
	u8 op = getOperand8<mode>();
	C(static_cast<bool>(op & 1u));
	op >>= 1u;
	writeOperandVal<mode>(op);
	UpdNZ(op);
}

template<AddressingModes mode, int waste>
void CPU::NOP() { pc += waste; }

template<AddressingModes mode>
void CPU::ORA() {
	calcCrossed = true;
	UpdNZ(a |= getOperand8<mode>());
}

template<AddressingModes mode>
void CPU::PHA() {
	tick();
	Push8(a);
}

template<AddressingModes mode>
void CPU::PHP() {
	tick();
	Push8(p | (1u << 4u));
}

template<AddressingModes mode>
void CPU::PLA() {
	tick(2);
	a = Pop8();
	UpdNZ(a);
}

template<AddressingModes mode>
void CPU::PLP() {
	tick(2);
	p = Pop8();
	B(false);
	// TODO: What?
	U(true);
}

template<AddressingModes mode>
void CPU::ROL() {
	tick(CalcBaseTicks<mode>());
	tick(CalcShiftTicks<mode>());
	u8 mem = getOperand8<mode>();
	u8 t = mem;
	mem <<= 1u;
	mem |= static_cast<u8>(C());
	writeOperandVal<mode>(mem);
	C(t & 0x80u);
	UpdNZ(mem);
}

template<AddressingModes mode>
void CPU::ROR() {
	tick(CalcBaseTicks<mode>());
	tick(CalcShiftTicks<mode>());
	u8 mem = getOperand8<mode>();
	u8 t = mem;
	mem >>= 1u;
	mem |= (C() << 7u);
	writeOperandVal<mode>(mem);
	C(t & 1u);
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
void CPU::RTI() {
	// NOTE: Changes the p register
	PLP<mode>(); // pop8
	tick(2);
	pc = Pop16(); // pop16
}

template<AddressingModes mode>
void CPU::RTS() {
	tick(4);
	pc = Pop16() + 1;
}

// TODO: Speed-up
template<AddressingModes mode>
void CPU::SBC() {
	tick(CalcBaseTicks<mode>());
	calcCrossed = true;
	u8 lhs = a;
	u8 rhs = getOperand8<mode>();

	u16 res{};
	if (D()) {
		lhs = toBCD(lhs);
		rhs = toBCD(rhs);
		u16 sres = lhs - rhs - !C();
		if (sres > 0x1ff) {
			sres = 99 - (0xffff - sres) + 100;
		}
		res = sres;
		res = toBinary(res);
	} else {
		rhs ^= 0xffu;
		res = lhs + rhs + C();
	}

	if (D()) {
		C(res < 0x100u);
	} else {
		updC(res);
	}
	UpdV(a, rhs, res);
	a = res & 0xffu;
	UpdNZ(a);
}

template<AddressingModes mode>
void CPU::SEC() { C(true); }

template<AddressingModes mode>
void CPU::SED() { D(true); }

template<AddressingModes mode>
void CPU::SEI() {
	I(true);
}

template<AddressingModes mode>
void CPU::STA() {
	tick(CalcBaseTicks<mode>());
	tick(CalcStoreTicks<mode>());
	writeOperandVal<mode>(a);
}

template<AddressingModes mode>
void CPU::STX() {
	tick(CalcBaseTicks<mode>());
	tick(CalcStoreTicks<mode>());
	writeOperandVal<mode>(x);
}

template<AddressingModes mode>
void CPU::STY() {
	tick(CalcBaseTicks<mode>());
	tick(CalcStoreTicks<mode>());
	writeOperandVal<mode>(y);
}

template<AddressingModes mode>
void CPU::TAX() {
	x = a;
	UpdNZ(x);
}

template<AddressingModes mode>
void CPU::TAY() {
	y = a;
	UpdNZ(y);
}

template<AddressingModes mode>
void CPU::TSX() {
	x = sp;
	UpdNZ(x);
}

template<AddressingModes mode>
void CPU::TXA() {
	a = x;
	UpdNZ(a);
}

template<AddressingModes mode>
void CPU::TXS() { sp = x; }

template<AddressingModes mode>
void CPU::TYA() {
	a = y;
	UpdNZ(a);
}

// INT is not a 6502 instruction. It is simply an interrupt handler.
template<Interrupts inter>
void CPU::INT() {
	// Note that BRK, although it is a one - byte instruction, needs an extra
	// byte of padding after it.This is because the return address it puts on
	// the stack will cause the RTI to put the program counter back not to the
	// very next byte after the BRK, but to the second byte after it. This
	// padding byte can be used for a signature byte to tell the BReaK interrupt
	// routine which BRK caused the particular software interrupt.
//#ifdef TEST_MODE
//	if constexpr (inter != Interrupts::RST) {
	if constexpr (inter != Interrupts::IRQ && inter != Interrupts::NMI) {
		pc++;
	}
//	}
//#endif

	tick();

	if constexpr (inter != Interrupts::BRK) {
		tick();
	}

	// The microprocessor uses the stack to save the reentrant
	// or recovery code.
	if constexpr (inter != Interrupts::RST) {
		Push16(pc);
#ifdef TEST_MODE
		Push8(p | (u8) ((Interrupts::BRK == inter) << 0x4u));
#else
		Push8(p & (u8) (0xffu ^ (u8) ((Interrupts::BRK == inter) << 0x4u)));
#endif
	} else {
		// s -= 3;
		tick(3);
	}

	// It should he noted that the interrupt disable is turned on at this 
	// point by the micro - processor automatically.

	// BRK does set the interrupt-disable I flag like an IRQ does, and if
	// you have the CMOS 6502 (65C02), it will also clear the decimal D flag.

	I(true);

	pc = read16(vect[inter]);

	if constexpr (inter == NMI) {
		nmi = false;
	}
}

void CPU::interruptRequest() {
	if (!I()) {
		INT<Interrupts::IRQ>();
	}
}

