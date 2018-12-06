#include "Clock.h"
#include "Clock.h"

#include <thread>

Clock::Clock() { }

Clock::Clock(std::chrono::nanoseconds _t) : t(_t) { }

void Clock::startTicking() {
	if (t != std::chrono::nanoseconds::zero()) {
		for (;; stamp++) {
			std::this_thread::sleep_for(t);
		}
	}
}

void Clock::waitTick(uint32_t ticks) {
	if (t != std::chrono::nanoseconds::zero()) {
		while (ticks--) {
			while (stamps[std::this_thread::get_id()] == stamp) { }
			stamps[std::this_thread::get_id()].store(stamp);
		}
	}
}
