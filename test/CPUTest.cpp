#define CATCH_CONFIG_MAIN
#include "CPU.h"
#include "catch.hpp"
#include "StreamUtil.h"
#include "cmrc/cmrc.hpp"
#include <boost/filesystem.hpp>
#include "Debugger.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include "MMULoader.h"
#include "MMU.h"

CMRC_DECLARE(test_resources);

TEST_CASE("CPU Test") {
	std::vector<u8> vicIO(0xffff);
	Clock clk{}; //	Clock clk(std::chrono::milliseconds(1));
	ROM rom{};
	auto fs = cmrc::test_resources::get_filesystem();
	MMU mmu{rom};

	MMULoader mmuLoader{fs, mmu};
	mmuLoader.dumpToRAM("res/6502_functional_test.bin", 0x400);

	CPU cpu(clk, mmu, 0x400);

	cpu.write(1, 0);

	bool passed{};


	long long int cnt{};
	u16 pcPrev{};

	while (true) {
		cpu.execute();

		cnt++;

		if (cnt == 1000000) {
			std::cout << "progress\n";
			cnt = 0;
		}

		if (cpu.PC() == pcPrev) {
			break;
		} else if (cpu.PC() == 0x3463 || cpu.PC() == 0x3462 || cpu.PC() == 0x3464) {
			passed = true;
			break;
		}

		pcPrev = cpu.PC();
	}

	REQUIRE(passed);
}