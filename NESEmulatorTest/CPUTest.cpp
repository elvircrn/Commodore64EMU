#include "CppUnitTest.h"
#include "../NES Emulator/CPU.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NESEmulatorTest
{		
#define ASRT(X, Y) Assert::AreEqual((X), (Y), L"V test failed", LINE_INFO());

	TEST_CLASS(CPUFlagTest)
	{
	public:
		
		TEST_METHOD(FlagSet)
		{
			CPU cpu;
			cpu.SetFlag(Flags::V, 1);
			Assert::AreEqual(cpu.GetFlag(Flags::V), true, L"V test failed", LINE_INFO());
		}
		
		TEST_METHOD(UpdateV)
		{
			CPU cpu;

			cpu.UpdV(0x80, 0x80, 0);
			Assert::AreEqual(cpu.GetFlag(Flags::V), true, L"Test failed", LINE_INFO());

			cpu.UpdV(0x9, 0x5, 0xE);
			Assert::AreEqual(cpu.GetFlag(Flags::V), false, L"Test failed", LINE_INFO());

			cpu.UpdV(0x40, 0x40, 0x80);
			Assert::AreEqual(cpu.GetFlag(Flags::V), true, L"Test failed", LINE_INFO());
		}

		TEST_METHOD(UpdateC)
		{
			CPU cpu;

			cpu.UpdC(0xff, 0xff, 0xff + 0xff);
			Assert::AreEqual(cpu.GetFlag(Flags::C), true, L"Test failed", LINE_INFO());

			cpu.UpdC(0, 1, 1);
			Assert::AreEqual(cpu.GetFlag(Flags::C), false, L"Test failed", LINE_INFO());
		}

		TEST_METHOD(UpdateNZ)
		{
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
	};
}