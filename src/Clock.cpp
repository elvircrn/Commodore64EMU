#include "Clock.h"
#include "Clock.h"

#include <thread>


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"


void Clock::startTicking() {
	if (enabled) {
		if (t != std::chrono::nanoseconds::zero()) {
			for (;; stamp++) {
				std::this_thread::sleep_for(t);
			}
		}
	}
}
#pragma clang diagnostic pop

void Clock::waitTick(uint32_t ticks) {
	if (enabled) {
		if (t != std::chrono::nanoseconds::zero()) {
			while (ticks--) {
				while (stamps[std::this_thread::get_id()] == stamp) {
				}
				stamps[std::this_thread::get_id()].store(stamp);
			}
		}
	}
}

Clock::Clock(std::chrono::nanoseconds t) : stamp{}, enabled(true), t(t) { }
