#ifndef C64EMU_CLOCK_H
#define C64EMU_CLOCK_H

#include "core.h"
#include <chrono>
#include <atomic>
#include <unordered_map>
#include <thread>

class Clock {
	std::atomic<int> stamp{};
	std::chrono::nanoseconds t = std::chrono::nanoseconds::zero();
	std::unordered_map<std::thread::id, std::atomic<int>> stamps;
public:
	bool enabled = false;
	Clock() = default;
	explicit Clock(std::chrono::nanoseconds _t);

	void startTicking();

	/**
	 * Block thread for @ticks * t nanoseconds. This method is the starting point of @CPU::execute and is therefore on a hot
	 * code path. There is a measurable performance impact if inline is not performed, so we have to make sure
	 * that the compiler doesn't forget to inline this method.
	 * @param ticks Number of ticks to wait
	 */
	inline void waitTick(uint32_t ticks = 1) {
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
};

#endif //C64EMU_CLOCK_H
