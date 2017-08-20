#include "PPU.h"

PPU::PPU() : mem(PPU_MEM_SIZE), oam(OAM_SIZE) { } 

u8 PPU::Rd(int addr)
{
	return mem[addr];
}

u8 PPU::Wr(int addr, u8 val) { return (mem[addr] = val); }
