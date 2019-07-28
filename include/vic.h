#ifndef NESEMU_VIC_H
#define NESEMU_VIC_H

#include "Clock.h"
#include "MMU.h"
#include "Screen.h"
#include "RegisterHolder.h"
#include "GraphicsConstants.h"

class VIC : public RegisterHolder<0xd000u, 0x100u> {
	static constexpr u16 VIC_REGISTER_ADDRESS_BASE = 0xd000u;
	static constexpr u8 VIC_REGISTER_COUNT = 47;

	static constexpr u16 BANK_SIZE = 0x4000;

	static constexpr u16 CONTROL_REGISTER_1 = 0xD011; // Control register 1  |RST8| ECM| BMM| DEN|RSEL|    YSCROLL   |
	static constexpr u16 RASTER_COUNTER_ADDR = 0xD012; // Raster counter     |                 RASTER                |
	static constexpr u16 VIC_MEMORY_BANK_ADDR = 0xDD00;
	static constexpr u16 MEMORY_POINTERS = 0xD018; // Memory pointers |VM13|VM12|VM11|VM10|CB13|CB12|CB11|  - | defaults to 0x40
	static constexpr u16 BORDER_COLOR = 0xD020; // Border color |  - |  - |  - |  - |         EC        | Border color
	static constexpr u16 BACKGROUND_COLOR_0 = 0xD021; // Background color 0 |  - |  - |  - |  - |        B0C        |


	u8 blockRowId{};

	/**
	The DEN bit (Display Enable, register $d011, bit 4) serves for switching on
	and off the text/bitmap graphics. It is normally set. The bit affects two
	functions of the VIC: The Bad Lines and the vertical border unit.

	 true, if den was set during raster line(0xd012) 0x30
	 false, if den was set during raster line(0xd012)
	 */
	bool wasDENSetOn30{};

	Clock &clock;
	MMU &mmu;
	Screen &screen;

public:
	VIC(Clock &_clock, MMU &_mmu, Screen &_screen) : clock(_clock), mmu(_mmu), screen(_screen) {}

	// Phi 0 negative edge
	void tick();
	u64 getCharData(u16 characterId);

	bool getCharData(u16 characterId, u8 bit);
	bool isBadLine(u8 rasterCounter, u8 yscroll, bool wasDENSet);


	static inline bool isVBlank(u16 rasterCounter) {
		return rasterCounter < GraphicsConstants::FIRST_BORDER_LINE || GraphicsConstants::FIRST_BORDER_LINE + GraphicsConstants::LAST_BORDER_LINE <= rasterCounter;
	}

	static inline bool isBorderLine(u16 rasterCounter) {
		return rasterCounter <= GraphicsConstants::BORDER_DIMENSIONS + GraphicsConstants::FIRST_BORDER_LINE || 257 <= rasterCounter;
	}

	inline u8 getControlRegister1() {
		return get(CONTROL_REGISTER_1);
	}

	inline u32 getRasterCounter() {
		return (((u32) BIT(getControlRegister1(), 7)) << 0x8u) | ((u32) get(RASTER_COUNTER_ADDR));
	}

	inline void setRasterCounter(u32 rasterCounter) {
		set(RASTER_COUNTER_ADDR, rasterCounter & 0xffu);
		set(CONTROL_REGISTER_1, SET(getControlRegister1(), 7, (rasterCounter & 0x100u) > 0));
	}

	inline u32 incrementRasterCounter() {
		u32 rasterCounter = getRasterCounter() + 1;
		setRasterCounter(rasterCounter);
		return rasterCounter;
	}

	u8 set(const u16 &addr, const u8 &val) override {
		u16 mappedAddr = normalize(addr);

		L_DEBUG(
		if (mappedAddr != 0xd012) {
			std::cout << std::hex << (u32) addr << ' ' << (u32) val << '\n';
		}
		);

		if (mappedAddr == CONTROL_REGISTER_1) {
			// Check if DEN is set during raster 0x30.
			bool den = BIT(val, 5);
			if (den && getRasterCounter() == 0x30) {
				wasDENSetOn30 = true;
			}
		}

		return RegisterHolder::set(addr, val);
	}
	bool getCharData(u16 characterId, u8 bit, u32 base);
};

#endif //NESEMU_VIC_H
