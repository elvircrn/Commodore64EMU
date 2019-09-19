#ifndef C64EMU_VIC_H
#define C64EMU_VIC_H

#include "Clock.h"
#include "MMU.h"
#include "Screen.h"
#include "RegisterHolder.h"
#include "GraphicsConstants.h"
#include "CPU.h"

class VIC : public RegisterHolder<0xd000u, 0x100u> {
	enum class GraphicsModes {
		StandardText = 0,
		MulticolorText,
		StandardBitmap,
		MulticolorBitmap
	};

	static constexpr u16 CONTROL_REGISTER_1 = 0xD011; // Control register 1  |RST8| ECM| BMM| DEN|RSEL|    YSCROLL   |
	static constexpr u16 CONTROL_REGISTER_2 = 0xD016; // Control register 2  |  - |  - | RES| MCM|CSEL|    XSCROLL   |
	static constexpr u16 RASTER_COUNTER_ADDR = 0xD012; // Raster counter     |                 RASTER                |
	static constexpr u16 VIC_MEMORY_BANK_ADDR = 0xDD00;
	static constexpr u16
			MEMORY_POINTERS = 0xD018; // Memory pointers |VM13|VM12|VM11|VM10|CB13|CB12|CB11|  - | defaults to 0x40
	static constexpr u16 BORDER_COLOR = 0xD020; // Border color |  - |  - |  - |  - |         EC        | Border color
	static constexpr u16 BACKGROUND_COLOR_0 = 0xD021; // Background color 0 |  - |  - |  - |  - |        B0C        |


	u8 blockRowId{};

	/**
	The DEN bit (Display Enable, register $d011, bit 4) serves for switching on
	and off the text/bitmap graphics. It is normally set. The bit affects two
	functions of the VIC: The Bad LiC64 and the vertical border unit.

	 true, if den was set during raster line(0xd012) 0x30
	 false, if den was set during raster line(0xd012)
	 */
	bool wasDENSetOn30{};

	Clock &clock;
	MMU &mmu;
	Screen &screen;
	CPU cpu;

public:
	VIC(Clock &_clock, MMU &_mmu, Screen &_screen, CPU &_cpu) : clock(_clock), mmu(_mmu), screen(_screen), cpu(_cpu) {}

	// Phi 0 negative edge
	void tick();

	bool isBadLine(u8 rasterCounter, u8 yscroll, bool wasDENSet);

	inline bool isVBlank(u16 rasterCounter) {
		return rasterCounter < GraphicsConstants::FIRST_BORDER_LINE
				|| GraphicsConstants::FIRST_BORDER_LINE + GraphicsConstants::LAST_BORDER_LINE <= rasterCounter;
	}

	inline bool isHorizontalBorder(u16 rasterCounter) {
		return rasterCounter <= GraphicsConstants::BORDER_DIMENSIONS + GraphicsConstants::FIRST_BORDER_LINE
				|| 257 <= rasterCounter;
	}

	inline bool isVerticalBorder(u16 x) {
		return x < GraphicsConstants::BORDER_DIMENSIONS
				|| x >= GraphicsConstants::BORDER_DIMENSIONS + GraphicsConstants::TEXT_AREA_WIDTH;
	}

	inline u8 getControlRegister1() {
		return get(CONTROL_REGISTER_1);
	}

	inline u8 getControlRegister2() {
		return get(CONTROL_REGISTER_2);
	}

	inline GraphicsModes graphicsMode() {
		u8 ctrl1 = getControlRegister1();
		u8 ctrl2 = getControlRegister2();
		bool bmm = BIT(ctrl1, 5);
		bool ecm = BIT(ctrl1, 6);
		bool mcm = BIT(ctrl2, 5); // TODO: Handle this flag

		return GraphicsModes{(u8) (ecm << 0x1u) | bmm};
	}

	inline u32 getRasterCounter() {
		return (((u32) BIT(getControlRegister1(), 7)) << 0x8u) | ((u32) get(RASTER_COUNTER_ADDR));
	}

	inline void setRasterCounter(u32 rasterCounter) {
		set(RASTER_COUNTER_ADDR, rasterCounter & 0xffu);
		set(CONTROL_REGISTER_1, SET(getControlRegister1(), 7, (rasterCounter & 0x100u) > 0));
	}

	inline u32 incrementRasterCounter() {
		u32 rasterCounter = (getRasterCounter() + 1) % 312;
		setRasterCounter(rasterCounter);
		return rasterCounter;
	}

	u8 get(const u16 &addr) const override {
		u16 mappedAddr = toMem(addr);
		if (0x46u < mappedAddr) {
			return 0xffu;
		} else {
			return RegisterHolder::get(mappedAddr);
		}
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

	u8 getCharColor(u16 colorMemBase, u8 blockColumn, u8 blockRow) const;

	bool
	getCharacterPixel(u16 vicBaseAddr, u16 charMemBase, u16 screenMemBase, u32 pixelId, u8 blockColumn, u8 blockRow);

	void standardText();

	void standardBitmap();
};

#endif //C64EMU_VIC_H
