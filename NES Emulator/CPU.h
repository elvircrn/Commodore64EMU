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
	INDEXED_INDIRECT,
	INDIRECT_INDEXED
};


class CPU
{

	// Registers
	uint8_t  a = 0; // Accumulator
	uint8_t  x = 0; // General purpose
	uint8_t  y = 0;
	uint8_t  s = 0; // Stack pointer
	uint8_t  p = 0; // Flags | N | V |   | B | D | I | Z | C |
	uint16_t pc = 0;


	// Memory
	constexpr static int INSTR_MEM_SIZE = 0x10000;
	std::vector<u8> instrs;

	constexpr static int RAM_SIZE = 0x800;
	std::vector<u8> ram;

	// Instructions
	constexpr static int RESET_VECTOR = 0x0000;
	uint16_t rst;
	std::vector<uint8_t> instr;

	// Addressing helpers
	inline u8 Imm() { return instrs[pc++]; }
	inline u16 Abs() { u16 res = (instrs[pc] << 8) | instrs[pc + 1]; pc += 2; return res; }
	inline u8 Zp(u8 addr) { return ram[addr]; }


	// Flag helpers
	inline bool N() { return GetFlag(Flags::N); }
	inline bool V() { return GetFlag(Flags::V); }
	inline bool B() { return GetFlag(Flags::B); }
	inline bool D() { return GetFlag(Flags::D); }
	inline bool I() { return GetFlag(Flags::I); }
	inline bool Z() { return GetFlag(Flags::Z); }
	inline bool C() { return GetFlag(Flags::C); }

public:
#pragma region Memory
	//inline Read(u16) { }
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

	void AND();	//....	and (with accumulator)
	void ASL();	//....	arithmetic shift left
	void BCC();	//....	branch on carry clear
	void BCS();	//....	branch on carry set
	void BEQ();	//....	branch on equal (zero set)
	void BIT();	//....	bit test
	void BMI();	//....	branch on minus (negative set)
	void BNE();	//....	branch on not equal (zero clear)
	void BPL();	//....	branch on plus (negative clear)
	void BRK();	//....	interrupt
	void BVC();	//....	branch on overflow clear
	void BVS();	//....	branch on overflow set
	void CLC();	//....	clear carry
	void CLD();	//....	clear decimal
	void CLI();	//....	clear interrupt disable
	void CLV();	//....	clear overflow
	void CMP();	//....	compare (with accumulator)
	void CPX();	//....	compare with X
	void CPY();	//....	compare with Y
	void DEC();	//....	decrement
	void DEX();	//....	decrement X
	void DEY();	//....	decrement Y
	void EOR();	//....	exclusive or (with accumulator)
	void INC();	//....	increment
	void INX();	//....	increment X
	void INY();	//....	increment Y
	void JMP();	//....	jump
	void JSR();	//....	jump subroutine
	void LDA();	//....	load accumulator
	void LDX();	//....	load X
	void LDY();	//....	load Y
	void LSR();	//....	logical shift right
	void NOP();	//....	no operation
	void ORA();	//....	or with accumulator
	void PHA();	//....	push accumulator
	void PHP();	//....	push processor status (SR)
	void PLA();	//....	pull accumulator
	void PLP();	//....	pull processor status (SR)
	void ROL();	//....	rotate left
	void ROR();	//....	rotate right
	void RTI();	//....	return from interrupt
	void RTS();	//....	return from subroutine
	void SBC();	//....	subtract with carry
	void SEC();	//....	set carry
	void SED();	//....	set decimal
	void SEI();	//....	set interrupt disable
	void STA();	//....	store accumulator
	void STX();	//....	store X
	void STY();	//....	store Y
	void TAX();	//....	transfer accumulator to X
	void TAY();	//....	transfer accumulator to Y
	void TSX();	//....	transfer stack pointer to X
	void TXA();	//....	transfer X to accumulator
	void TXS();	//....	transfer X to stack pointer
	void TYA();	//....	transfer Y to accumulator
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