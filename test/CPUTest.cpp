#define CATCH_CONFIG_MAIN
#include "cpu.h"
#include "catch.hpp"
#include "stream_util.h"
#include "cmrc/cmrc.hpp"

#include <iomanip>
#include "mmu.h"
CMRC_DECLARE(test_resources);

TEST_CASE("CPU Test") {
	std::ios oldState(nullptr);
	oldState.copyfmt(std::cout);
	std::chrono::high_resolution_clock::time_point start(
			std::chrono::high_resolution_clock::now());

	SDL_Event evt;

	std::vector<u8> vicIO(0xffff);
	Clock clk{};
	CIA1 cia1{evt};
	CIA2 cia2{};
	ROM rom{};
	auto fs = cmrc::test_resources::get_filesystem();
	MMU mmu{rom, cia1, cia2};

	auto content = fs.open("res/6502_functional_test.bin");
	u32 addr = 0x400;
	for (const auto& it : content) {
		mmu.writeRAM(addr++, it);
	}

	CPU cpu(clk, mmu, 0x400);

	cpu.write(1, 0);

	cpu.setDebug(false);

	bool passed{};
	u16 pcPrev{};

	while (true) {
		cpu.execute();

		if (cpu.PC() == 0x3463) {
			passed = true;
			break;
		} else if (cpu.PC() == pcPrev) {
			std::cout << "Stuck in a loop at " << std::hex << pcPrev << '\n';
			break;
		}

		pcPrev = cpu.PC();
	}

	std::cout << std::endl;
	for (size_t i = cpu.instrHist.size() - 50; i < cpu.instrHist.size(); i++) {
		auto v = std::get<2>(cpu.instrHist[i]);
		std::cout << std::hex << std::setw(2) << std::setfill('0') << std::get<0>(cpu.instrHist[i]) << ' '
							<< std::get<1>(cpu.instrHist[i]) << ' ';
		for (const auto &data : v) {
			std::cout << ' ' << std::hex << std::setw(2) << std::setfill('0') << (int) data;
		}
		std::cout << '\n';
	}

	std::cout.copyfmt(oldState);

	auto microseconds =
			std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);
	std::cout << "Duration: " << microseconds.count() << "ms\n";

	REQUIRE(passed);
}
