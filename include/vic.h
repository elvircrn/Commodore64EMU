#ifndef NESEMU_VIC_H
#define NESEMU_VIC_H

#include "Clock.h"
#include "MMU.h"

class VIC {
	static const u16 CONTROL_REGISTER_1  = 0xD011; // Control register 1  |RST8| ECM| BMM| DEN|RSEL|    YSCROLL   |
	static const u16 RASTER_COUNTER_ADDR = 0xD012; // Raster counter      |                 RASTER                |
	static const u16 MEMORY_POINTERS     = 0xD018; // Memory pointers     |VM13|VM12|VM11|VM10|CB13|CB12|CB11|  - |

	Clock &clock;
	MMU &mmu;

public:
	VIC(Clock &_clock, MMU &_mmu) : clock(_clock), mmu(_mmu) {}

	// Phi 0 negative edge
	void tick() {
		clock.waitTick();

	}
};

#endif //NESEMU_VIC_H
