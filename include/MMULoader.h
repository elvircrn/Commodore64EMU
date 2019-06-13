#ifndef NESEMU_MMULOADER_H
#define NESEMU_MMULOADER_H

#include "cmrc/cmrc.hpp"
#include "core.h"
#include "MMU.h"

class MMULoader {
	cmrc::embedded_filesystem fs;
	MMU &mmu;

public:
	MMULoader(cmrc::embedded_filesystem fs, MMU &mmu) : fs(fs), mmu(mmu) {

	}

	void dumpToRAM(const std::string &filepath, u16 addr);
};

#endif //NESEMU_MMULOADER_H
