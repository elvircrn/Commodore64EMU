#ifndef C64EMU_PRGLOADER_H
#define C64EMU_PRGLOADER_H

#include "MMU.h"

class PRGLoader {

public:
    static void loadPRG(MMU &mmu, ROM &rom) {
        mmu.writeRAM16(0x37, 0xa000u);
        mmu.writeRAM16(0x2b, rom.offset);

        rom.hasCustom = true;

        u32 lastROMByte = rom.offset + rom.customROM.size() - 1;
        mmu.writeRAM16(0x2du, lastROMByte);
        mmu.writeRAM16(0x2fu, lastROMByte);
        mmu.writeRAM16(0x31u, lastROMByte);

        for (u32 i = rom.offset; i < lastROMByte; i++) {
            mmu.writeRAM(i, rom.customROM[i - rom.offset + 2]);
        }
    }
};

#endif //C64EMU_PRGLOADER_H
