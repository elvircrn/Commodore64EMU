#ifndef C64EMU_PRG_LOADER_H
#define C64EMU_PRG_LOADER_H

#include "mmu.h"

class PRGLoader {
	constexpr static u8 PRG_ROM_HEADER_SIZE = 0x2u;

public:
	static void loadPRG(MMU &mmu, ROM &rom) {
		mmu.writeRAM16(0x37, 0xa000u);
		mmu.writeRAM16(0x2b, rom.prgROMOffset);

		u32 lastROMByte = rom.prgROMOffset + rom.customROM.size() - 1;
		mmu.writeRAM16(0x2du, lastROMByte);
		mmu.writeRAM16(0x2fu, lastROMByte);
		mmu.writeRAM16(0x31u, lastROMByte);

		for (u32 i = rom.prgROMOffset; i < lastROMByte; i++) {
			mmu.writeRAM(i, rom.customROM[i - rom.prgROMOffset + PRG_ROM_HEADER_SIZE]);
		}
	}
};

#endif //C64EMU_PRG_LOADER_H
