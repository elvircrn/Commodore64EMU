#ifndef NESEMU_CLOCK_H
#define NESEMU_CLOCK_H

#include "core.h"
#include <chrono>
#include <atomic>
#include <unordered_map>
#include <thread>

class Clock {
	std::atomic<int> stamp{};
	bool enabled = false;
	std::chrono::nanoseconds t = std::chrono::nanoseconds::zero();
	std::unordered_map<std::thread::id, std::atomic<int>> stamps;
public:
	Clock() = default;
	explicit Clock(std::chrono::nanoseconds _t);

	void startTicking();

	void waitTick(uint32_t ticks = 1);
};

#endif //NESEMU_CLOCK_H
