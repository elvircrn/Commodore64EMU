#define CATCH_CONFIG_MAIN
#include "CPU.h"
#include "catch.hpp"
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


TEST_CASE("PatternLoad") {
	std::string donkeyPatternName = "donkeypattern.txt";
	std::string donkeyPatternPath = boost::filesystem::path(__FILE__)
			.parent_path()
			.append(donkeyPatternName)
			.string();
	std::ifstream in(donkeyPatternPath);
	std::vector<std::vector<char>> expected(128, std::vector<char>(128));
	for (size_t i = 0; i < 128; i++) {
		for (size_t j = 0; j < 128; j++) {
			in >> expected[i][j];
			expected[i][j] -= '0';
		}
	}

	std::string fileName = "donkeykong.nes";
	std::string filePath = boost::filesystem::path(__FILE__)
			.parent_path()
			.parent_path()
			.append(fileName)
			.string();

	std::cout << "Trying to load: " << filePath << '\n';
	std::ifstream file(filePath, std::ios::binary);

	if (!file) {
		std::cout << "Failed to read rom given: " << filePath << '\n';
		ASRT(true, false);
	}

	file.unsetf(std::ios::skipws);

	ROM rom(file);
	PPU ppu(rom);

	auto actual = ppu.getPatternTable(0x0000);

	ASRT(actual, expected);
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
	Clock clk;
	ROM rom(file);
	PPU ppu(rom);
	MMU mmu(ppu);
	CPU cpu(clk, mmu);
	Debugger debugger(&cpu);
	cpu.powerUp();
	cpu.loadROM(rom);

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
			cpu.execute();
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
