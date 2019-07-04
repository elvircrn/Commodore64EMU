
#ifndef NESEMU_CPU_H
#define NESEMU_CPU_H

#include <cstdint>
#include <vector>
#include <deque>
#include <tuple>
#include "MMU.h"
#include "optional.h"
#include "Clock.h"
#include <functional>

#include "core.h"
#include "ROM.h"

// Interrupt types along with their respective vectors
enum Interrupts { BRK = 0, IRQ, NMI, RST };
constexpr u16 vect[] = {0xFFFE, 0xFFFE, 0xFFFA, 0xFFFC};

// CPU Flags, _ -> empty
enum Flags { C, Z, I, D, B, _, V, N };

// Adressing modes
enum AddressingModes {
	RELATIVE = 0,
	INDIRECT, // Only used by the JMP instruction
	ABSOLUTE,
	INDIRECT_INDEXED,
	IMMEDIATE,
	ACCUMULATOR,
	IMPLIED,
	ZERO_PAGE,
	ABSOLUTE_INDEXED_X,
	ABSOLUTE_INDEXED_Y,
	ZERO_PAGE_INDEXED,
	ZERO_PAGE_X,
	ZERO_PAGE_Y,
	INDEXED_INDIRECT_X
};

// TODO: Check if every instructions increases the PC by at least 1.
class CPU {
	static constexpr bool ignoreUnknownInstr = true;
	Clock &clock;

	// Registers
	u8 a = 0; // Accumulator
	u8 x = 0; // General purpose
	u8 y = 0;
	u8 sp = 0; // Stack pointer
	u8 p = 0; // Flags | N | V |   | B | D | I | Z | C |
	u16 pc = 0;

	// Interrupts
	bool nmi = false;

	// Memory
	MMU &mmu;

	// Instructions
	bool zeroPageCrossed;
	bool calcCrossed;

	// Buffers
	u16 buff16;
	u8 buff8;

	// Absolute along with its variants, and immediate reads mutate the program counter, therefore
	// the value obtained via these two methods should only be calculated once during the instruction
	// cycle.
	std::experimental::optional<u16> instructionDataLatch;

	// Instruction helpers
	inline u8 _ASL(const u8 &x) {
		C(x & 0x80);
		return (x << 1);
	}
	inline u16 _ASL(u16 &x) {
		C(x & 0x8000);
		return (x << 1);
	}

/**
 * Get @nib-th nibble of @val.
 * @param val Value that contains the nibble
 * @param nibIdx Nibble index
 * @return
 */
	inline u8 getNibble(u16 val, u8 nibIdx) {
		return (val >> (4u * nibIdx)) & 0xfu;
	}

	inline u16 toBinary(u16 in) {
		return (((in / 1000u) % 10) << 16u) + (((in / 100u) % 10) << 8u) +  (((in / 10u) % 10) << 4u) + (in % 10u);
	}

	inline u8 toBCD(u8 in) {
		u8 pow = 1, res{};
		for (size_t i = 0; i < 2; pow *= 10, i++) {
			u8 nib = getNibble(in, i);
			if (nib > 9) {
				nib += 6;
			}
			res += nib * pow;
		}
		return res;
	}

	// Addressing helpers
	inline u8 Imm() {
		if (!instructionDataLatch) {
			instructionDataLatch = Read(pc++);
		}
		return static_cast<u8>(instructionDataLatch.value());
	}

	inline u16 Abs() {
		if (!instructionDataLatch) {
			pc += 2;
			instructionDataLatch = Read16(static_cast<u16>(pc - 2));
		}
		return instructionDataLatch.value();
	}

	inline u16 AbsX() {
		u16 abs = Abs();
		CrossesPage(abs, x);
		return abs + x;
	}

	inline u16 AbsY() {
		u16 abs = Abs();
		CrossesPage(abs, y);
		return abs + y;
	}
	inline u16 Zp16(u8 addr) {
		return (mmu.read(static_cast<u16>((addr + 1) & 0xff)) << 8) + mmu.read(addr);
	}
	inline void CrossesPage(u16 addr, u8 offset) {
		zeroPageCrossed |= ((addr & 0xff00) != ((addr + offset) & 0xff00));
	}
	// 		return static_cast<u8 &>(mmu(static_cast<u16>(0x0100 + static_cast<u32>(addr))));
	inline void Push8(u8 val) {
		mmu.write(static_cast<u16>(0x0100 + static_cast<u32>(sp--)), val);
	}
	inline void Push16(u16 val) {
		Push8(static_cast<u8>(val >> 8));
		Push8(static_cast<u8>(val & 0xFF));
	}
	inline u8 Pop8() {
		sp++;
		return mmu.read(static_cast<u16>(0x0100 + static_cast<u32>(sp)));
	}
	inline u16 Pop16() {
		u8 lo = Pop8();
		u16 hi = Pop8() << 8;
		return hi + lo;
	}

	// Flag helpers
	inline bool N() { return GetFlag(Flags::N); }
	inline bool V() { return GetFlag(Flags::V); }
	inline bool U() { return GetFlag(Flags::_); }
	inline bool B() { return GetFlag(Flags::B); }
	inline bool D() { return GetFlag(Flags::D); }
	inline bool I() { return GetFlag(Flags::I); }
	inline bool Z() { return GetFlag(Flags::Z); }
	inline bool C() { return GetFlag(Flags::C); }

	inline void N(bool x) { SetFlag(Flags::N, x); }
	inline void V(bool x) { SetFlag(Flags::V, x); }
	inline void U(bool x) { SetFlag(Flags::_, x); }
	inline void B(bool x) { SetFlag(Flags::B, x); }
	inline void D(bool x) { SetFlag(Flags::D, x); }
	inline void I(bool x) { SetFlag(Flags::I, x); }
	inline void Z(bool x) { SetFlag(Flags::Z, x); }
	inline void C(bool x) { SetFlag(Flags::C, x); }

public:
	inline u8 A() { return a; }
	inline u8 X() { return x; }
	inline u8 Y() { return y; }
	inline u16 PC() { return pc; }
	inline u8 SP() { return sp; }
	inline u8 P() { return p; }

	inline u8 A(u8 _a) { return a = _a; }
	inline u8 X(u8 _x) { return x = _x; }
	inline u8 Y(u8 _y) { return y = _y; }
	inline u16 PC(u16 _pc) { return pc = _pc; }
	inline u8 SP(u8 _sp) { return sp = _sp; }
	inline u8 P(u8 _p) { return p = _p; }

#pragma region Debug
	static constexpr bool DEBUG = true;
	bool debug = false;
	bool isDebug() const;
	void setDebug(bool debug);
	std::deque<u16> pcHist;
	std::deque<u8> opHist;
	std::deque<u8> bitStack;
	std::deque<std::pair<std::string, u16>> vectors;
	std::deque<std::tuple<u16, std::string, std::array<u8, 4>>> instrHist;
	void debugDump();
#pragma endregion

#pragma region Setup
	// TODO: Consider refactoring into an external class?
	void init();
	void init(u16 pc);
	// Called before instruction execution
	inline void clear() {
		zeroPageCrossed = false;
		calcCrossed = false;
		delta = 0;
		instructionDataLatch.reset();
	}
#pragma endregion

#pragma region Memory
	bool inline isOAMDMA(u16 addr) {
		return addr == 0x4014;
	}

	// Default memory layout, no reference to PPU
	inline u8 Read(u16 addr) const {
		return mmu.read(addr);
	}

	inline bool write(const u16 &addr, const u8 &value) {
		if (!oamDmaIdx && isOAMDMA(addr)) {
			oamDmaIdx = 256;
		}
		mmu.write(addr, value);
		return true;
	}

	inline u16 Read16(u16 addr) { return Read(addr) + (Read(addr + 1) << 8); }
	inline u16 Ind(u16 addr) {
		tick(3);
		u8 lo = Read(addr);
		u8 hi = Read((addr & 0xff00u) + LO(addr + 1));
		return lo + (hi << 8u);
	}
#pragma endregion

#pragma region Flags
	inline void SetFlag(Flags, bool);
	inline bool GetFlag(Flags) const;

	inline void UpdC(u8, u8, u16);
	inline void UpdV(u8, u8, u16);
	inline void UpdCV(u8, u8, u16);

	inline void UpdN(u8);
	inline void UpdZ(u8);
	inline void UpdNZ(u8);

	inline void setNMI(bool _nmi) { nmi = _nmi; }
	inline bool GetNMI() { return nmi; }
#pragma endregion

#pragma region Constructors
	explicit CPU(Clock &, MMU &mmu);
	explicit CPU(Clock &, MMU &mmu, u16 _pc);
	~CPU();
#pragma endregion

#pragma region Timing
	int cycleCount;
	int delta;
	inline void tick(int cycles = 1) {
		cycleCount += cycles;
		delta += cycles;
	}

	inline int Delta() { return delta; }

	// NOTE Do not use with jumps or returns!
#pragma region Tick calculation
	template<AddressingModes mode>
	inline int CalcBaseTicks() {
		switch (mode) {
		case AddressingModes::IMMEDIATE: return 0;
		case AddressingModes::ZERO_PAGE: return 1;
		case AddressingModes::ZERO_PAGE_X: return 2;
		case AddressingModes::ZERO_PAGE_Y: return 2;
		case AddressingModes::ABSOLUTE: return 2;
		case AddressingModes::ABSOLUTE_INDEXED_X: return 2;
		case AddressingModes::ABSOLUTE_INDEXED_Y: return 2;
		case AddressingModes::INDEXED_INDIRECT_X: return 4;
		case AddressingModes::INDIRECT_INDEXED: return 3;
		default: return 0;
		}
	}

	template<AddressingModes mode>
	inline int CalcStoreTicks() {
		switch (mode) {
		case AddressingModes::ABSOLUTE_INDEXED_X: return 1;
		case AddressingModes::ABSOLUTE_INDEXED_Y: return 1;
		case AddressingModes::INDIRECT_INDEXED: return 1;
		default: return 0;
		}
	}

	template<AddressingModes mode>
	inline int CalcShiftTicks() {
		switch (mode) {
		case AddressingModes::ZERO_PAGE: return 2;
		case AddressingModes::ZERO_PAGE_X: return 2;
		case AddressingModes::ZERO_PAGE_Y: return 2;
		case AddressingModes::ABSOLUTE: return 2;
		case AddressingModes::ABSOLUTE_INDEXED_X: return 3;
		default: return 0;
		}
	}

	template<AddressingModes mode>
	inline int CalcIncDecTicks() {
		switch (mode) {
		case AddressingModes::ZERO_PAGE: return 2;
		case AddressingModes::ZERO_PAGE_X: return 2;
		case AddressingModes::ABSOLUTE: return 2;
		case AddressingModes::ABSOLUTE_INDEXED_X: return 3;
		default: return 0;
		}
	}
#pragma endregion
#pragma endregion

	void execute();

#pragma region Instructions
	void interruptRequest();

	template<AddressingModes mode>
	void ADC();  //....	add with carry
	template<AddressingModes mode>
	void AND();  //....	and (with accumulator)
	template<AddressingModes mode>
	void ASL();  //....	arithmetic shift left
	template<AddressingModes mode>
	void BCC();  //....	branch on carry clear
	template<AddressingModes mode>
	void BCS();  //....	branch on carry set
	template<AddressingModes mode>
	void BEQ();  //....	branch on equal (zero set)
	template<AddressingModes mode>
	void BIT();  //....	bit test
	template<AddressingModes mode>
	void BMI();  //....	branch on minus (negative set)
	template<AddressingModes mode>
	void BNE();  //....	branch on not equal (zero clear)
	template<AddressingModes mode>
	void BPL();  //....	branch on plus (negative clear)
	template<AddressingModes mode>
	void BRK();  //....	interrupt
	template<AddressingModes mode>
	void BVC();  //....	branch on overflow clear
	template<AddressingModes mode>
	void BVS();  //....	branch on overflow set
	template<AddressingModes mode>
	void CLC();  //....	clear carry
	template<AddressingModes mode>
	void CLD();  //....	clear decimal
	template<AddressingModes mode>
	void CLI();  //....	clear interrupt disable
	template<AddressingModes mode>
	void CLV();  //....	clear overflow
	template<AddressingModes mode>
	void CMP();  //....	compare (with accumulator)
	template<AddressingModes mode>
	void CPX();  //....	compare with X
	template<AddressingModes mode>
	void CPY();  //....	compare with Y
	template<AddressingModes mode>
	void DEC();  //....	decrement
	template<AddressingModes mode>
	void DEX();  //....	decrement X
	template<AddressingModes mode>
	void DEY();  //....	decrement Y
	template<AddressingModes mode>
	void EOR();  //....	exclusive or (with accumulator)
	template<AddressingModes mode>
	void INC();  //....	increment
	template<AddressingModes mode>
	void INX();  //....	increment X
	template<AddressingModes mode>
	void INY();  //....	increment Y
	template<AddressingModes mode>
	void JMP();  //....	jump
	template<AddressingModes mode>
	void JSR();  //....	jump subroutine
	template<AddressingModes mode>
	void LDA();  //....	load accumulator
	template<AddressingModes mode>
	void LDX();  //....	load X
	template<AddressingModes mode>
	void LDY();  //....	load Y
	template<AddressingModes mode>
	void LSR();  //....	logical shift right
	template<AddressingModes mode, int count = 0>
	void NOP();  //....	no operation
	template<AddressingModes mode>
	void ORA();  //....	or with accumulator
	template<AddressingModes mode>
	void PHA();  //....	push accumulator
	template<AddressingModes mode>
	void PHP();  //....	push processor status (SR)
	template<AddressingModes mode>
	void PLA();  //....	pull accumulator
	template<AddressingModes mode>
	void PLP();  //....	pull processor status (SR)
	template<AddressingModes mode>
	void ROL();  //....	rotate left
	template<AddressingModes mode>
	void ROR();  //....	rotate right
	template<AddressingModes mode>
	void RTI();  //....	return from interrupt
	template<AddressingModes mode>
	void RTS();  //....	return from subroutine
	template<AddressingModes mode>
	void SBC();  //....	subtract with carry
	template<AddressingModes mode>
	void SEC();  //....	set carry
	template<AddressingModes mode>
	void SED();  //....	set decimal
	template<AddressingModes mode>
	void SEI();  //....	set interrupt disable
	template<AddressingModes mode>
	void STA();  //....	store accumulator
	template<AddressingModes mode>
	void STX();  //....	store X
	template<AddressingModes mode>
	void STY();  //....	store Y
	template<AddressingModes mode>
	void TAX();  //....	transfer accumulator to X
	template<AddressingModes mode>
	void TAY();  //....	transfer accumulator to Y
	template<AddressingModes mode>
	void TSX();  //....	transfer stack pointer to X
	template<AddressingModes mode>
	void TXA();  //....	transfer X to accumulator
	template<AddressingModes mode>
	void TXS();  //....	transfer X to stack pointer
	template<AddressingModes mode>
	void TYA();  //....	transfer Y to accumulator
	template<Interrupts inter>
	void INT();

	// Unofficial opcodes
	template<AddressingModes mode>
	void LAX();
	template<AddressingModes mode>
	void SAX();
	template<AddressingModes mode>
	void ALR();
	template<AddressingModes mode>
	void ANC();
	template<AddressingModes mode>
	void ARR();
	template<AddressingModes mode>
	void AXS();
	template<AddressingModes mode>
	void LSE();
	template<AddressingModes mode>
	void RLA();
#pragma endregion

#pragma region Operand calculator
	template<AddressingModes m>
	u16 getOperand16();
	template<AddressingModes m>
	u8 getOperand8();
	template<AddressingModes m>
	bool writeOperandVal(const u8 &val);
	template<AddressingModes m>
	u16 getPureOperand();

	template<AddressingModes m>
	constexpr inline bool Is8Bit() { return !(0 <= m && m < 2); }
#pragma endregion
	u16 oamDmaIdx = 0;
};

// Inlined functions


// Flags
inline void CPU::SetFlag(Flags f, bool bit) {
	p ^= (-(static_cast<int>(bit)) ^ p) & (1u << f);
}

inline bool CPU::GetFlag(Flags f) const {
	return (p & (1 << f)) > 0;
}

inline void CPU::UpdV(u8 x, u8 y, u16 r) {
	V(static_cast<bool>(~(x ^ y) & (r ^ x) & 0x80u));
}

inline void CPU::UpdC(u8 x, u8 y, u16 r) {
	C(0xff < r);
}

inline void CPU::UpdCV(u8 pX, u8 pY, u16 r) {
	UpdV(pX, pY, r);
	UpdC(pX, pY, r);
}

inline void CPU::UpdN(u8 x) {
	SetFlag(Flags::N, static_cast<bool>(x & 0x80));
}

inline void CPU::UpdZ(u8 x) {
	SetFlag(Flags::Z, !x);
}

inline void CPU::UpdNZ(u8 x) {
	UpdN(x);
	UpdZ(x);
}

inline bool CPU::isDebug() const {
	return debug;
}

inline void CPU::setDebug(bool debug) {
	CPU::debug = debug;
}

#endif
