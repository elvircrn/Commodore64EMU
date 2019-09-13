#ifndef C64EMU_TIMINGCONSTANTS_H
#define C64EMU_TIMINGCONSTANTS_H

#include "core.h"

class TimingConstants {
public:
	static constexpr u64 PAL_MASTER_NANOS = 5;
	static constexpr u64 PAL_CPU_RATIO = 1;
	static constexpr u64 PAL_VIC_RATIO = 2000;
	static constexpr u64 CONSOLE_RATIO = 10000;
	static constexpr u64 CPU_INITIAL_CYCLES = 10000;
	static constexpr u64 PAL_KEYBOARD_CYCLES = 1000000;
};

#endif //C64EMU_TIMINGCONSTANTS_H
