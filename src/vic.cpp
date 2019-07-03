#include "vic.h"

#include "core.h"

void VIC::drawChars() {
	auto bup = mmu.read(1);
	mmu.write(1, 3);
	u8 bank = 0x3u - (mmu.read(VIC_MEMORY_BANK_ADDR) & 0x3u);

	// Screen memory
	u8 screenMemoryLocation = HI(mmu.read(MEMORY_POINTERS));

	u16 start = 0xd000;

	constexpr int rows = 5, cols = 5;
	std::vector<std::string> data{rows * 8};
	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < cols; j++) {
			for (size_t k = 0; k < 8; k++) {
				for (size_t l = 0; l < 8; l++) {
					data[i * 8 + k].push_back(getCharData(2, k * 8 + l) ? '1' : '0');
				}
			}
		}
	}

//	for (size_t row{}; row < 8 * rows; row++) {
//		std::cout << data[row] << '\n';
//	}
//	for (u16 cId = 0; cId < 5; cId++) {
//		u16 i = cId * 8;
//		for (size_t j = 0; j < 0x8u; j++) {
//			for (int k = 7; k >= 0; k--) {
//				std::cout << (BIT(mmu.read(start + i + j), k) ? '1' : '0');
//			}
//			std::cout << '\n';
//		}
//	}
	mmu.write(1, bup);
}

void VIC::tick() {
//	drawChars();
	u8 rasterCounter = get(RASTER_COUNTER_ADDR);




	rasterCounter++;
	set(RASTER_COUNTER_ADDR, rasterCounter);
}

u64 VIC::getCharData(u16 characterId) {
	u64 data{};
	u16 base = 0xd000;
	u16 offset = base + (8u * characterId);
	for (u8 i = 0; i < 0x8u; i++) {
		data <<= 0x8u;
		data |= mmu.read(offset + i);
	}
	return data;
}

bool VIC::isBadLine(u8 rasterCounter, u8 yscroll, bool wasDENSet) {
	return wasDENSet && 0x30u <= rasterCounter && rasterCounter <= 0xf7u && (yscroll == (rasterCounter & 0x7u));
}

bool VIC::getCharData(u16 characterId, u8 bit) {
	u64 data{};
	u16 base = 0xd000;
	u16 offset = base + (8u * characterId);
	return BIT(mmu.read(offset + bit / 8), 7 - (bit % 8));
}



