// #define RUN_TESTS

#ifdef RUN_TESTS
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../NES Emulator/CPU.h"
#include "../NES Emulator/FileHandler.h"
#include <boost/filesystem.hpp>
#include "../NES Emulator/Debugger.h"

#include <iomanip>

namespace NESEmulatorTest {
#define ASRT(X, Y) REQUIRE((X) == (Y))

void LoggerDump(const Debugger &debugger) {
	INFO(debugger.GetOpHistForLogging().c_str());
	// INFO(debugger.GetPCHistForLogging().c_str());
}

TEST_CASE("FlagSet") {
	CPU cpu;
	cpu.SetFlag(Flags::V, 1);
	ASRT(cpu.GetFlag(Flags::V), true);
}

TEST_CASE("UpdateV") {
	CPU cpu;

	cpu.UpdV(0x80, 0x80, 0);
	ASRT(cpu.GetFlag(Flags::V), true);

	cpu.UpdV(0x9, 0x5, 0xE);
	ASRT(cpu.GetFlag(Flags::V), false);

	cpu.UpdV(0x40, 0x40, 0x80);
	ASRT(cpu.GetFlag(Flags::V), true);
}

TEST_CASE("UpdateC") {
	CPU cpu;

	cpu.UpdC(0xff, 0xff, 0xff + 0xff);
	ASRT(cpu.GetFlag(Flags::C), true);

	cpu.UpdC(0, 1, 1);
	ASRT(cpu.GetFlag(Flags::C), false);
}

TEST_CASE("UpdateNZ") {
	CPU cpu;

	cpu.UpdN(0);
	ASRT(cpu.GetFlag(Flags::N), false);

	cpu.UpdN(0x80);
	ASRT(cpu.GetFlag(Flags::N), true);

	cpu.UpdZ(0);
	ASRT(cpu.GetFlag(Flags::Z), true);

	cpu.UpdZ(1);
	ASRT(cpu.GetFlag(Flags::Z), false);
}

TEST_CASE("ROMHeaderTest") {
	auto filepath = boost::filesystem::path(__FILE__)
			.parent_path()
			.append("nestest.nes")
			.string();

	ROM rom(filepath);

	ASRT(rom.CHRCnt(), 1);
	ASRT(rom.PRGCnt(), 1);
}

TEST_CASE("NESTestNoCycleCount") {
	auto filepath = boost::filesystem::path(__FILE__)
			.parent_path()
			.append("nestest.nes")
			.string();

	ROM rom(filepath);
	CPU cpu;
	Debugger debugger(&cpu);
	cpu.PowerUp();
	cpu.LoadROM(rom);

	auto res = boost::filesystem::path(__FILE__)
			.parent_path()
			.append("res.txt")
			.string();

	std::ifstream myfile(res);
	std::vector<std::string> lines;
	std::string correctLine;

	int lineCount = 0;

	while (std::getline(myfile, correctLine)) {
		if (!cpu.IsOfficial())
			break;

		std::string emuLine = debugger.GetNESTestLine();

		debugger.AppendStatHist(emuLine);

		if (correctLine != emuLine) {
			LoggerDump(debugger);
			INFO(("Exp: " + correctLine + "\nFnd: " + emuLine).c_str());
			INFO(("Failed at: " + std::to_string(lineCount)).c_str());
			std::stringstream ss;
			ss << std::hex << std::setfill('0') << std::setw(2) << (int) cpu.PC();
			INFO(("PC: " + ss.str()).c_str());
			ASRT(true, false);
		}
		try {
			cpu.Execute();
		}
		catch (std::string e) {
			LoggerDump(debugger);
			std::stringstream ss;
			ss << std::hex << std::setfill('0') << std::setw(2) << (int) cpu.PC();
			INFO(("PC: " + ss.str()).c_str());
			ASRT(true, false);
		}
		lineCount++;
	}
}
}
#endif