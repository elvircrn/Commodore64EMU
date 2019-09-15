#ifndef C64EMU_MMU_H
#define C64EMU_MMU_H

#include <utility>
#include <vector>
#include "core.h"
#include "ROM.h"
#include "CIA1.h"
#include "CIA2.h"

class MMU {
    constexpr static int RAM_SIZE = 65536;

    std::vector<u8> ram;
    ROM &rom;
    CIA1 &cia1;
    CIA2 &cia2;

    std::function<u8(const u16 &)> vicReadCallback;
    std::function<bool(const u16 &, const u8 &)> vicWriteCallback;

public:
    MMU(ROM &_rom, CIA1 &cia1, CIA2 &cia2) : ram(RAM_SIZE), rom(_rom), cia1(cia1), cia2(cia2) {
        // The default is mode 31 (no cartridge) as all latch bits are logically high (set to 1).
        // https://www.c64-wiki.com/wiki/Bank_Switching
        writeRAM(1, (1u << 8u) - 1u);
        writeRAM16(0x2bu, 0x801);
        writeRAM16(0x37u, 0xa000);
    }

    inline void writeRAM16(const u16 &addr, const u16 &val) {
        ram[addr] = LO(val);
        ram[addr + 1] = HI(val);
    }

    [[nodiscard]] inline u8 readVIC(const u16 &addr) const {
        if (0x1000u <= addr && addr < 0x2000u) {
            return rom.chargen[addr - 0x1000u];
        } else if (0x9000u <= addr && addr < 0xa000u) {
            return rom.chargen[addr - 0x9000u];
        } else {
            return ram[addr];
        }
    }

    [[nodiscard]] inline u8 read(const u16 &addr, bool vic = false) const {
        if (vic) {
            return readVIC(addr);
        }

        u8 bankMask = ram[1];

		u8 retVal{};
		if (addr < 0xa000) {
            retVal = ram[addr];
		} else if (addr < 0xc000) {
			if (isBASIC(bankMask)) {
				retVal = rom.basic[addr - 0xa000];
			} else {
				retVal = ram[addr];
			}
		} else if (addr < 0xd000) {
			retVal = ram[addr];
		} else if (addr < 0xe000) {
			if (vic || isCharGen(bankMask)) {
				retVal = rom.chargen[addr - 0xd000u];
			} else if (isIO(bankMask)) {
				if (addr < 0xd400 && vicReadCallback) {
					retVal = vicReadCallback(addr);
				} else if (HI(addr) == 0xdcu) {
					retVal = cia1.read(addr);
				} else if (HI(addr) == 0xddu) {
					retVal = cia2.read(addr);
				} else {
					retVal = rom.io[addr - 0xd000];
				}
			}
		} else if (addr <= 0xffff) {
			if (isKernal(bankMask)) {
				retVal = rom.kernal[addr - 0xe000];
			} else {
				retVal = ram[addr];
			}
		} else {
			retVal = ram[addr];
		}

        return retVal;
    }

    inline bool write(u16 addr, u8 val) {
        u8 bankMask = ram[1];

        if (addr >= 0x400 && addr - 0x400 < 512 &&
            ram[addr] != 0x8e && ram[addr] != 0xe) {
            L_INFO(std::cout << "Writing to screen space addr " << std::hex << std::setw(2) << std::setfill('0')
                             << (int) addr << " prev: " << (int) ram[addr] << " " << (int) val << std::endl);
        }

        if (addr < 0xa000) {
            return ram[addr] = val;
        } else if (addr < 0xc000) {
            if (bankMask != 7 && isBASIC(bankMask)) {
                return rom.basic[addr - 0xa000] = val;
            } else {
                return ram[addr] = val;
            }
        } else if (addr < 0xd000) {
            return ram[addr] = val;
        } else if (addr < 0xe000) {
            if (isCharGen(bankMask)) {
                return rom.chargen[addr - 0xd000] = val;
            } else if (isIO(bankMask)) {
                if (addr < 0xd400 && vicWriteCallback) {
                    return vicWriteCallback(addr, val);
                } else if (HI(addr) == 0xdcu) {
                    return cia1.write(addr, val);
                } else if (HI(addr) == 0xddu) {
                    return cia2.write(addr, val);
                }
                return rom.io[addr - 0xd000] = val;
            }
        } else if (addr <= 0xffff) {
            if (isKernal(bankMask)) {
                return rom.kernal[addr - 0xe000] = val;
            } else {
                return ram[addr] = val;
            }
        }
        return ram[addr] = val;
    }

    inline void writeRAM(u16 addr, u8 val) {
        ram[addr] = val;
    }

    [[nodiscard]] inline bool isIO(const u8 &val) const {
        return val >= 0x5u;
    }

    [[nodiscard]] inline bool isCharGen(const u8 &val) const {
        return (~BIT(val, 2u)) & (BIT(val, 0) | BIT(val, 1));
    }

    [[nodiscard]] inline bool isBASIC(const u8 &val) const {
        return (BIT(val, 0) & BIT(val, 1));
    }

    [[nodiscard]] inline bool isKernal(const u8 &val) const {
        return BIT(val, 1);
    }

    inline void setVICReadListener(std::function<u8(const u16 &)> _vicReadCallback) {
        this->vicReadCallback = _vicReadCallback;
    }

    inline void setVICWriteListener(std::function<bool(const u16 &, const u8 &)> _vicWriteCallback) {
        this->vicWriteCallback = _vicWriteCallback;
    }
};

#endif //C64EMU_MMU_H

