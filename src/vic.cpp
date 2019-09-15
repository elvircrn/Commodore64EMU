#include "vic.h"
#include "core.h"
#include "GraphicsConstants.h"

bool VIC::isBadLine(u8 rasterCounter, u8 yscroll, bool wasDENSet) {
	return wasDENSet && 0x30u <= rasterCounter && rasterCounter <= 0xf7u && (yscroll == (rasterCounter & 0x7u));
}

bool VIC::getCharData(u16 characterId, u8 bit, u32 base) {
	u64 data{};
	u16 offset = base + (8u * characterId);
	return BIT(mmu.read(offset + bit / 8, true), 7 - (bit % 8));
}

void VIC::tick() {
	u16 rasterCounter = getRasterCounter();
	u8 vicBank = 0x3u - ((mmu.read(VIC_MEMORY_BANK_ADDR)) & 0x3u);
	u16 vicBaseAddr = vicBank << 14;

	u8 memoryPointers = get(MEMORY_POINTERS);
	u8 vm = memoryPointers >> 0x4u;
	u8 cb = (u8) (memoryPointers >> 0x1u) & 0x7u;

	u16 charMemBase = (cb * 0x800u);
	u16 screenMemBase = (vm * 0x400u);
    u16 colorMemBase = 0xd800u;

	u8 borderColor = get(BORDER_COLOR);
	if (!isVBlank(rasterCounter)) {
		if (isHorizontalBorder(rasterCounter)) {
			for (size_t i = 0; i < GraphicsConstants::WINDOW_WIDTH; i++) {
				screen.drawPixel(i, rasterCounter - GraphicsConstants::FIRST_BORDER_LINE, 0, borderColor);
			}
			blockRowId = 0;
		} else {
			for (u32 i = 0; i < GraphicsConstants::WINDOW_WIDTH; i++) {
				if (isVerticalBorder(i)) {
					screen.drawPixel(i, rasterCounter - GraphicsConstants::FIRST_BORDER_LINE, 0, borderColor);
				} else {
					u32 pixelId = i - GraphicsConstants::FIRST_VISIBLE_VERTICAL_LINE;

					u8 blockColumn = pixelId / 8;
					u8 blockRow =
							(rasterCounter - GraphicsConstants::FIRST_VISIBLE_LINE - GraphicsConstants::FIRST_BORDER_LINE) / 8;

					u8 characterId = mmu.read(screenMemBase + blockRow * 40 + blockColumn, true);
					u8 charColor = mmu.read(colorMemBase + blockRow * 40 + blockColumn);
					u8 charPixelId = (blockRowId * 8) + pixelId % 8;
					bool charData = getCharData(characterId, charPixelId, charMemBase);
					screen.drawPixel(i, rasterCounter - GraphicsConstants::FIRST_BORDER_LINE, charData, get(BACKGROUND_COLOR_0), charColor);
				}
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
