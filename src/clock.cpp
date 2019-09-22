#include "clock.h"
#include "clock.h"

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

Clock::Clock(std::chrono::nanoseconds t) : stamp{}, t(t), enabled(true) {}
