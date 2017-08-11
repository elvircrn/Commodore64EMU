#include "CppUnitTest.h"
#include "../NES Emulator/CPU.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NESEmulatorTest
{		
	TEST_CLASS(CPUTest)
	{
	public:
		
		TEST_METHOD(CPUTestFlags)
		{
			CPU cpu;
			cpu.AND();
			cpu.SetFlag(Flags::V, 1);
			Assert::AreEqual(cpu.GetFlag(Flags::V), true, L"V test failed", LINE_INFO());
		}

	};
}