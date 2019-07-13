#ifndef NESEMU_VIC_H
#define NESEMU_VIC_H

#include "Clock.h"
#include "MMU.h"

class VIC {
	static constexpr u8 VIC_REGISTER_COUNT = 47;
	static constexpr u16 VIC_REGISTER_ADDRESS_BASE = 0xd000u;

	const u16 BANK_SIZE = 0x4000;
	const u16 CONTROL_REGISTER_1 = 0xD011; // Control register 1  |RST8| ECM| BMM| DEN|RSEL|    YSCROLL   |
	const u16 RASTER_COUNTER_ADDR = 0xD012; // Raster counter     |                 RASTER                |

	std::array<u8, 0xffu> mem{};

	const u16 VIC_MEMORY_BANK_ADDR = 0xDD00;
	const u16 MEMORY_POINTERS = 0xD018; // Memory pointers |VM13|VM12|VM11|VM10|CB13|CB12|CB11|  - |, defaults to 0x400

	/**
	The DEN bit (Display Enable, register $d011, bit 4) serves for switching on
	and off the text/bitmap graphics. It is normally set. The bit affects two
	functions of the VIC: The Bad Lines and the vertical border unit.

	 true, if den was set during raster line(0xd012) 0x30
	 false, if den was set during raster line(0xd012)
	 */
	bool wasDENSetOn30;

	Clock &clock;
	MMU &mmu;

public:
	VIC(Clock &_clock, MMU &_mmu) : clock(_clock), mmu(_mmu) {}

	// Phi 0 negative edge
	void tick();
	u64 getCharData(u16 characterId);

	void drawChars();
	bool getCharData(u16 characterId, u8 bit);
	bool isBadLine(u8 rasterCounter, u8 yscroll, bool wasDENSet);

	inline u8 getControlRegister1() {
		return get(CONTROL_REGISTER_1);
	}

	inline u8 getRasterCounter() {
		return get(RASTER_COUNTER_ADDR);
	}

	inline u16 get16(u16 addr) {
		return (get(addr + 1) << 0x8u) + get(addr);
	}

	/**
 	 * Map external addresses into internal buffer
 	 * @param addr External address
 	 * @return Value from internal memory @mem
 	 */
	inline u8 get(u16 addr) {
		addr = (addr & 0xffu) + VIC_REGISTER_ADDRESS_BASE;
		return mem[addr - VIC_REGISTER_ADDRESS_BASE];
	}

	/**
	 * Adds the ability to add custom logic during data writes to
	 * VIC-specific addresses.
	 */
	inline bool set(u16 addr, u8 val) {
		addr = (addr & 0xffu) + VIC_REGISTER_ADDRESS_BASE;
		if (addr == CONTROL_REGISTER_1) {
			// Check if DEN is set during raster 0x30.
			bool den = BIT(val, 5);
			if (den && getRasterCounter() == 0x30) {
				wasDENSetOn30 = true;
			}
		}

		return (mem[addr - VIC_REGISTER_ADDRESS_BASE] = val) == val;
	}
};

#endif //NESEMU_VIC_H
