#include "MMULoader.h"

void MMULoader::dumpToRAM(const std::string &filepath, u16 addr) {
	auto content = fs.open(filepath);


	for (const auto& it : content) {
		mmu.writeRAM(addr++, it);
	}
}
