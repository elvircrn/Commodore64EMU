#ifndef NESEMU_CLOCK_H
#define NESEMU_CLOCK_H

#include "core.h"

class Clock {
	u64 t;
public:
	Clock(u64 _t, const std::function<void()> &_onTick);

	void startTicking();

	void onTick();

	void tick();
};

#endif //NESEMU_CLOCK_H
