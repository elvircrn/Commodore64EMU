#include "Clock.h"
#include "Clock.h"

#include <thread>


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"


template<bool enabled>
void Clock<enabled>::startTicking() {
	if constexpr (enabled) {
		if (t != std::chrono::nanoseconds::zero()) {
			for (;; stamp++) {
				std::this_thread::sleep_for(t);
			}
		}
	}
}
#pragma clang diagnostic pop

template<bool enabled>
void Clock<enabled>::waitTick(uint32_t ticks) {
	if constexpr (enabled) {
		if (t != std::chrono::nanoseconds::zero()) {
			while (ticks--) {
				while (stamps[std::this_thread::get_id()] == stamp) {
				}
				stamps[std::this_thread::get_id()].store(stamp);
			}
		}
	}
}

template<bool enabled>
Clock<enabled>::Clock(std::chrono::nanoseconds t) : t(t), stamp{} { }
