#include <iostream>
#include "CPU.h"
#include "FileHandler.h"

void print_vector()
{
	static unsigned char rom[32 * 1024L];
	FILE* file = fopen("test.nes", "rb");
	if (!file)
		return;

	if (fseek(file, 16, SEEK_SET))
		goto error;

	if (!fread(rom, 1, sizeof rom, file))
		goto error;

	{
		unsigned offset = 0x8000;
		unsigned reset = rom[0xfffd - offset] * 0x100 +
			rom[0xfffc - offset];

		printf("Reset vector: $%04x\n", reset);
	}

error:
	fclose(file);
}

void printVector(const ROM &rom)
{
	unsigned offset = 0x8000;
	unsigned reset = rom[0xfffd - offset] << 8 + rom[0xfffc - offset];
	printf("Reset vector: $%04x\n", reset);
}


int main()
{
	ROM rom = FileHandler().LoadCartridge("test.nes");
	for (int i = 0; i < 20; i++)
		std::cout << std::hex << (int)rom.GetData()[i] << ' ';
	std::cout << '\n';

	print_vector();
	printVector(rom);

	CPU cpu;

	std::getchar();
	return 0;
}