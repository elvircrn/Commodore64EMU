#pragma once

#include <stdint.h>
#include <vector>
#include <variant>

#include "core.h"
#include "ROM.h"

// Flags
enum Flags { C = 0, Z, I, D, B, _, V, N };

// Adressing modes
enum AddressingModes
{
	RELATIVE = 0,
	INDIRECT, // Only used by the JMP instruction
	INDIRECT_INDEXED,
	IMMEDIATE,
	ACCUMULATOR,
	IMPLIED,
	ABSOLUTE,
	ZERO_PAGE,
	ABSOLUTE_INDEXED_X,
	ABSOLUTE_INDEXED_Y,
	ZERO_PAGE_INDEXED,
	ZERO_PAGE_X,
	ZERO_PAGE_Y,
	INDEXED_INDIRECT_X
};

// TODO: Check if every instructions increases the PC by at least 1.
class CPU
{
	// Timers
	u64 cycle = 0;

	// Registers
	uint8_t  a  = 0; // Accumulator
	uint8_t  x  = 0; // General purpose
	uint8_t  y  = 0;
	uint8_t  sp = 0; // Stack pointer
	uint8_t  p  = 0; // Flags | N | V |   | B | D | I | Z | C |
	uint16_t pc = 0;

	// Memory
	constexpr static int RAM_SIZE = 65536;
	std::vector<u8> ram;

	// Instructions
	constexpr static int RESET_VECTOR = 0x0000;
	uint16_t rst;

	// Buffers
	u16 buff16;
	u8 buff8;

	// Instruction helpers
	inline u8  _ASL(u8 &x)  { C(x & 0x80);   return (x <<= 1); }
	inline u16 _ASL(u16 &x) { C(x & 0x8000); return (x <<= 1); }

	// Addressing helpers
	inline u8 Imm() { return Read(pc++); }
	inline u16 Abs() { return Read16((pc += 2) - 2); }
	inline u16 AbsX() { return Abs() + x; }
	inline u16 AbsY() { return Abs() + y; }
	inline u8& Zp(u8 addr) { return ram[addr]; }
	inline u16 Zp16(u8 addr)
	{
		// Check for zero-page crossing
		//if (addr == 0xff)
			//return Read16(addr);
		//else
		return (ram[(addr + 1) & 0xff] << 8) + ram[addr];
	}
	inline bool CrossesZp(u16 addr) { return addr > 0xff; }
	inline void Push8(u8 val) { Stk(sp--) = val; }
	// NOTE: First the high byte is pushed, then the low.
	inline void Push16(u16 val) { Push8(val >> 8); Push8(val & 0xFF); }
	inline u8 Pop8() { return Stk(++sp); }
	inline u16 Pop16() { u8 lo = Pop8(); u16 hi = Pop8() << 8; return hi + lo; }

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

	#pragma region Debug
	static constexpr bool DEBUG = true;
	std::vector<u16> pcHist;
	std::vector<u8>  opHist;
	std::vector<u8>  bitStack;
	#pragma endregion

	#pragma region Setup
	// TODO: Consider refactoring into an external class?
	void PowerUp();
	#pragma endregion

	#pragma region Memory
	inline u8& Read(u16 addr)
	{
		// Mirrored 2KB of internal RAM
		if (addr < 0x2000)
			return ram[addr & 0x7FF];
		// Mirrors of NES PPU registers
		else if (0x2000 <= addr && addr <= 0x3fff)
			return ram[((addr - 0x2000) % 8) + 0x2000];
		else
			return ram[addr];
	}
	inline u16 Read16(u16 addr) { return Read(addr) + (Read(addr + 1) << 8); }
	inline u16 Ind(u16 addr)
	{
		u8 lo = Read(addr);
		u8 hi = (addr >> 8) + 1;
		return Read(lo) + (Read(hi) << 8);
	}
	inline u8& Stk(u8 addr) { return ram[0x0100 + static_cast<u32>(addr)]; }
	#pragma endregion

	#pragma region Flags
	inline void SetFlag(Flags, bool);
	inline bool GetFlag(Flags) const;

	inline void UpdC(u8, u8, s16);
	inline void UpdV(u8, u8, s16);
	inline void UpdCV(u8, u8, s16);

	inline void UpdN(u8);
	inline void UpdZ(u8);
	inline void UpdNZ(u8);
	#pragma endregion

	void LoadCartridge(const ROM &rom);

	CPU();
	CPU(uint16_t);
	~CPU();

	#pragma region Timing
	void Tick();
	#pragma endregion

	void Execute();

	#pragma region Register getters and setters
	void SetA(uint8_t);
	void SetX(uint8_t);
	void SetY(uint8_t);
	void SetS(uint8_t);
	void SetP(uint8_t);
	void SetPC(uint16_t);

	uint8_t GetA() const;
	uint8_t GetX() const;
	uint8_t GetY() const;
	uint8_t GetSP() const;
	uint8_t GetP() const;
	uint16_t GetPC() const;
	#pragma endregion

	#pragma region Instructions
	template<AddressingModes mode>
	void ADC();	//....	add with carry
	template<AddressingModes mode>
	void AND();	//....	and (with accumulator)
	template<AddressingModes mode>
	void ASL();	//....	arithmetic shift left
	template<AddressingModes mode>
	void BCC();	//....	branch on carry clear
	template<AddressingModes mode>
	void BCS();	//....	branch on carry set
	template<AddressingModes mode>
	void BEQ();	//....	branch on equal (zero set)
	template<AddressingModes mode>
	void BIT();	//....	bit test
	template<AddressingModes mode>
	void BMI();	//....	branch on minus (negative set)
	template<AddressingModes mode>
	void BNE();	//....	branch on not equal (zero clear)
	template<AddressingModes mode>
	void BPL();	//....	branch on plus (negative clear)
	template<AddressingModes mode>
	void BRK();	//....	interrupt
	template<AddressingModes mode>
	void BVC();	//....	branch on overflow clear
	template<AddressingModes mode>
	void BVS();	//....	branch on overflow set
	template<AddressingModes mode>
	void CLC();	//....	clear carry
	template<AddressingModes mode>
	void CLD();	//....	clear decimal
	template<AddressingModes mode>
	void CLI();	//....	clear interrupt disable
	template<AddressingModes mode>
	void CLV();	//....	clear overflow
	template<AddressingModes mode>
	void CMP();	//....	compare (with accumulator)
	template<AddressingModes mode>
	void CPX();	//....	compare with X
	template<AddressingModes mode>
	void CPY();	//....	compare with Y
	template<AddressingModes mode>
	void DEC();	//....	decrement
	template<AddressingModes mode>
	void DEX();	//....	decrement X
	template<AddressingModes mode>
	void DEY();	//....	decrement Y
	template<AddressingModes mode>
	void EOR();	//....	exclusive or (with accumulator)
	template<AddressingModes mode>
	void INC();	//....	increment
	template<AddressingModes mode>
	void INX();	//....	increment X
	template<AddressingModes mode>
	void INY();	//....	increment Y
	template<AddressingModes mode>
	void JMP();	//....	jump
	template<AddressingModes mode>
	void JSR();	//....	jump subroutine
	template<AddressingModes mode>
	void LDA();	//....	load accumulator
	template<AddressingModes mode>
	void LDX();	//....	load X
	template<AddressingModes mode>
	void LDY();	//....	load Y
	template<AddressingModes mode>
	void LSR();	//....	logical shift right
	template<AddressingModes mode>
	void NOP();	//....	no operation
	template<AddressingModes mode>
	void ORA();	//....	or with accumulator
	template<AddressingModes mode>
	void PHA();	//....	push accumulator
	template<AddressingModes mode>
	void PHP();	//....	push processor status (SR)
	template<AddressingModes mode>
	void PLA();	//....	pull accumulator
	template<AddressingModes mode>
	void PLP();	//....	pull processor status (SR)
	template<AddressingModes mode>
	void ROL();	//....	rotate left
	template<AddressingModes mode>
	void ROR();	//....	rotate right
	template<AddressingModes mode>
	void RTI();	//....	return from interrupt
	template<AddressingModes mode>
	void RTS();	//....	return from subroutine
	template<AddressingModes mode>
	void SBC();	//....	subtract with carry
	template<AddressingModes mode>
	void SEC();	//....	set carry
	template<AddressingModes mode>
	void SED();	//....	set decimal
	template<AddressingModes mode>
	void SEI();	//....	set interrupt disable
	template<AddressingModes mode>
	void STA();	//....	store accumulator
	template<AddressingModes mode>
	void STX();	//....	store X
	template<AddressingModes mode>
	void STY();	//....	store Y
	template<AddressingModes mode>
	void TAX();	//....	transfer accumulator to X
	template<AddressingModes mode>
	void TAY();	//....	transfer accumulator to Y
	template<AddressingModes mode>
	void TSX();	//....	transfer stack pointer to X
	template<AddressingModes mode>
	void TXA();	//....	transfer X to accumulator
	template<AddressingModes mode>
	void TXS();	//....	transfer X to stack pointer
	template<AddressingModes mode>
	void TYA();	//....	transfer Y to accumulator
#pragma endregion

	#pragma region Operand calculator
	template<AddressingModes m>
	u16 GetOperand16();
	template<AddressingModes m>
	u8& GetOperand8();
	template<AddressingModes m>
	u16 GetPureOperand();

    template<AddressingModes m>
    constexpr inline bool Is8Bit() { return !(0 <= m && m < 2); }
	#pragma endregion
};

// Inlined functions

// Flags
inline void CPU::SetFlag(Flags f, bool bit)
{
	p ^= (-(static_cast<int>(bit)) ^ p) & (1 << f);
}

inline bool CPU::GetFlag(Flags f) const
{
	return (p & (1 << f)) > 0;
}

inline void CPU::UpdV(u8 x, u8 y, s16 r)
{
	V(~(x ^ y) & (r ^ x) & 0x80);
}

inline void CPU::UpdC(u8 x, u8 y, s16 r)
{
	C(0xff < r);
}

inline void CPU::UpdCV(u8 x, u8 y, s16 r)
{
	UpdV(x, y, r);
	UpdC(x, y, r);
}

inline void CPU::UpdN(u8 x)
{
	SetFlag(Flags::N, x & 0x80);
}

inline void CPU::UpdZ(u8 x)
{
	SetFlag(Flags::Z, !x);
}

inline void CPU::UpdNZ(u8 x)
{
	UpdN(x);
	UpdZ(x);
}

