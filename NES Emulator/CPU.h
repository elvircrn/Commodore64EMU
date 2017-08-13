#pragma once

#include <stdint.h>
#include <vector>

#include "core.h"

// Flags
enum Flags { C = 0, Z, I, D, B, _, V, N };

// Adressing modes
enum AddressingModes
{
	ACCUMULATOR,
	IMMEDIATE,
	IMPLIED,
	RELATIVE,
	ABSOLUTE,
	ZERO_PAGE,
	INDIRECT, // Only used by the JMP instruction
	ABSOLUTE_INDEXED_X,
	ABSOLUTE_INDEXED_Y,
	ZERO_PAGE_INDEXED,
	ZERO_PAGE_X,
	ZERO_PAGE_Y,
	INDEXED_INDIRECT_X,
	INDIRECT_INDEXED
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
	constexpr static int RAM_SIZE = 0x800;
	std::vector<u8> ram;

	// Instructions
	constexpr static int RESET_VECTOR = 0x0000;
	uint16_t rst;
	std::vector<uint8_t> instr;

	// Instruction memory
	constexpr static int INSTR_MEM_SIZE = 0x10000;
	std::vector<u8> instrs;
	u8 buff;

	// Instruction helpers
	inline u8 _ASL(u8 &x) { C(x & 0x80); return (x <<= 1); }
	inline u8 _ROL(u8& x) { return x = (((x & 0x80) ? 1 : 0) | (x << 1)); }
	inline u8 _ROR(u8& x) { return x = (((x & 0x01) ? 0x80 : 0) | (x >> 1)); }

	// Addressing helpers
	inline u8 Imm() { return instrs[pc++]; }
	inline u16 Abs() { u16 res = (instrs[pc] << 8) | instrs[pc + 1]; pc += 2; return res; }
	inline u16 AbsX() { u16 res = (instrs[pc] << 8) | instrs[pc + 1]; pc += 2; return res + x; }
	inline u16 AbsY() { u16 res = (instrs[pc] << 8) | instrs[pc + 1]; pc += 2; return res + y; }
	inline u8& Zp(u8 addr) { return ram[addr]; }
	inline bool CrossesZp(u16 addr) { return addr > 0xff; }
	inline void Push8(u8 val) { Stk(sp--) = val; }
	// NOTE: First the high byte is pushed, then the low.
	inline void Push16(u16 val) { Push8(val & 0xFF); Push8(val >> 8); }
	inline u8 Pop8() { return Stk(sp++); }
	inline u16 Pop16() { u8 lo = Pop8(); u16 hi = Pop8() << 8; return hi + lo; }

	// Flag helpers
	inline bool N() { return GetFlag(Flags::N); }
	inline bool V() { return GetFlag(Flags::V); }
	inline bool B() { return GetFlag(Flags::B); }
	inline bool D() { return GetFlag(Flags::D); }
	inline bool I() { return GetFlag(Flags::I); }
	inline bool Z() { return GetFlag(Flags::Z); }
	inline bool C() { return GetFlag(Flags::C); }

	inline void N(bool x) { SetFlag(Flags::N, x); }
	inline void V(bool x) { SetFlag(Flags::V, x); }
	inline void B(bool x) { SetFlag(Flags::B, x); }
	inline void D(bool x) { SetFlag(Flags::D, x); }
	inline void I(bool x) { SetFlag(Flags::I, x); }
	inline void Z(bool x) { SetFlag(Flags::Z, x); }
	inline void C(bool x) { SetFlag(Flags::C, x); }

public:
#pragma region Memory
	inline u8& Read(u16 addr) { return ram[addr]; }
	inline u8& Stk(u8 addr) { return ram[0x0100 + addr]; }
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
#pragma endregion

	CPU();
	CPU(uint16_t);
	~CPU();

	void Tick();
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
	uint8_t GetS() const;
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

	template<AddressingModes m>
	u8& GetOperand();
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

inline void CPU::UpdV(u8 x, u8 y, u16 r)
{
	SetFlag(Flags::V, ~(x ^ y)  & (r ^ x) & 0x80);
}

inline void CPU::UpdC(u8 x, u8 y, u16 r)
{
	SetFlag(Flags::C, 0xff < r);
}

inline void CPU::UpdCV(u8 x, u8 y, u16 r)
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

