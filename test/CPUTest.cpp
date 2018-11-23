#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "CPU.h"
#include "FileHandler.h"
#include <boost/filesystem.hpp>
#include "Debugger.h"

#include <fstream>
#include "NanoLog.h"
#include <iomanip>
#include "MMU.h"

namespace NESEmulatorTest {
#define ASRT(X, Y) REQUIRE((X) == (Y))

void LoggerDump(const Debugger &debugger) {
	INFO(debugger.GetOpHistForLogging().c_str());
	// INFO(debugger.GetPCHistForLogging().c_str());
}

TEST_CASE("FlagSet") {
	u8 xx;
	CPU cpu([&xx](u16 x) -> u8 & { return xx; });
	cpu.SetFlag(Flags::V, 1);
	ASRT(cpu.GetFlag(Flags::V), true);
}

TEST_CASE("UpdateV") {
	u8 xx;
	CPU cpu([&xx](u16 x) -> u8 & { return xx; });

	cpu.UpdV(0x80, 0x80, 0);
	ASRT(cpu.GetFlag(Flags::V), true);

	cpu.UpdV(0x9, 0x5, 0xE);
	ASRT(cpu.GetFlag(Flags::V), false);

	cpu.UpdV(0x40, 0x40, 0x80);
	ASRT(cpu.GetFlag(Flags::V), true);
}

TEST_CASE("UpdateC") {
	u8 xx;
	CPU cpu([&xx](u16 x) -> u8 & { return xx; });

	cpu.UpdC(0xff, 0xff, 0xff + 0xff);
	ASRT(cpu.GetFlag(Flags::C), true);

	cpu.UpdC(0, 1, 1);
	ASRT(cpu.GetFlag(Flags::C), false);
}

TEST_CASE("UpdateNZ") {
	u8 xx;
	CPU cpu([&xx](u16 x) -> u8 & { return xx; });

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
	auto filePath = boost::filesystem::path(__FILE__)
			.parent_path()
			.append("nestest.nes")
			.string();

	std::ifstream file(filePath, std::ios::binary);

	if (!file) {
		LOG_INFO << "Failed to read rom given: " << filePath << '\n';
		ASRT(true, false);
	}

	file.unsetf(std::ios::skipws);

	ROM rom(file);

	ASRT(rom.CHRCnt(), 1);
	ASRT(rom.PRGCnt(), 1);
}

TEST_CASE("NESTestNoCycleCount") {
	auto filePath = boost::filesystem::path(__FILE__)
			.parent_path()
			.append("nestest.nes")
			.string();

	std::ifstream file(filePath, std::ios::binary);

	if (!file) {
		LOG_INFO << "Failed to read rom given: " << filePath << '\n';
		ASRT(true, false);
	}

	file.unsetf(std::ios::skipws);

	ROM rom(file);
	PPU ppu(rom);
	MMU mmu(ppu);
	auto mmuFn = [&mmu](u16 addr) -> u8 & { return static_cast<u8 &>(mmu(addr)); };
	CPU cpu(mmuFn);
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
		catch (std::string &e) {
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
