#pragma once

#include <stdint.h>
#include <vector>

class CPU
{
	// Registers
	uint8_t  a; // Accumulator
	uint8_t  x; // General purpose
	uint8_t  y;
	uint8_t  s; // Stack pointer
	uint8_t  p; // Flags | N | V |   | B | D | I | Z | C |
	uint16_t pc;

	// Memory
	std::vector<uint8_t> mem;
	constexpr static int MEM_SIZE = 0x10000;

	// Instructions
	constexpr static int RESET_VECTOR = 0x0000;
	uint16_t rst;
	std::vector<uint8_t> instr;

public:
	CPU();
	CPU(uint16_t);
	~CPU();

	void Tick();
	void Execute();


	// Memory map

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
};

