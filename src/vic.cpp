#include "vic.h"

#include "core.h"

u64 VIC::getCharData(u16 characterId) {
	u64 data{};
	u16 base = 0xd000;
	u16 offset = base + (8u * characterId);
	for (u8 i = 0; i < 0x8u; i++) {
		data <<= 0x8u;
		data |= mmu.read(offset + i, true);
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
	return BIT(mmu.read(offset + bit / 8, true), 7 - (bit % 8));
}

bool VIC::getCharData(u16 characterId, u8 bit, u32 base) {
	u64 data{};
	u16 offset = base + (8u * characterId);
	return BIT(mmu.read(offset + bit / 8, true), 7 - (bit % 8));
}

void VIC::tick() {
	u16 rasterCounter = getRasterCounter();
	u8 vicBank = 0x3u - ((mmu.read(VIC_MEMORY_BANK_ADDR)) & 0x3u);

	u8 memoryPointers = get(MEMORY_POINTERS);
	u8 vm = memoryPointers >> 0x4u;
	u8 cb = (u8) (memoryPointers >> 0x1u) & 0x7u;

	u16 charMemBase = (cb * 0x800u);
	u16 screenMemBase = (vm * 0x400u);

	u8 borderColor = get(BORDER_COLOR);
	if (!isVBlank(rasterCounter)) {
		if (isBorderLine(rasterCounter)) {
			for (size_t i = 0; i < 403; i++) {
				screen.drawPixel(i, rasterCounter - FIRST_BORDER_LINE, 0, borderColor);
			}
//		screen.drawPixel()
			// Draw border
			blockRowId = 0;
		} else {
			for (u32 i = 0; i < FIRST_VISIBLE_VERTICAL_LINE; i++) {
				screen.drawPixel(i, rasterCounter - FIRST_BORDER_LINE, 0, borderColor);
			}

			for (u32 i = FIRST_VISIBLE_VERTICAL_LINE; i < LAST_VISIBLE_VERTICAL_LINE; i++) {
				u32 pixelId = i - FIRST_VISIBLE_VERTICAL_LINE;

				u8 blockColumn = pixelId / 8; // TODO: Extract 25 as constant
				u8 blockRow = (rasterCounter - FIRST_VISIBLE_LINE - FIRST_BORDER_LINE) / 8;

				u8 characterId = mmu.read(screenMemBase + blockRow * 40 + blockColumn);
				u8 charPixelId = (blockRowId * 8) + pixelId % 8;
				bool charData = getCharData(characterId, charPixelId, 0xd000);
				screen.drawPixel(i, rasterCounter - FIRST_BORDER_LINE, charData, get(BACKGROUND_COLOR_0));
			}

//		for (size_t k = 0; k < 8; k++) {
//			for (size_t l = 0; l < 8; l++) {
//				if (getCharData(32, k * 8 + l)) {
//					std::cout << '*';
//				} else {
//					std::cout << ' ';
//				}
//			}
//			std::cout << '\n';
//		}

			for (u32 i = LAST_VISIBLE_VERTICAL_LINE; i < 403; i++) {
				screen.drawPixel(i, rasterCounter - FIRST_BORDER_LINE, 0, borderColor);
			}
			blockRowId = (blockRowId + 1) % 8;
		}
	}

	rasterCounter = incrementRasterCounter();

	if (rasterCounter == 312) {
		rasterCounter = 0;
	}
	setRasterCounter(rasterCounter);
}
