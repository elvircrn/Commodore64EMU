#include "vic.h"
#include "core.h"
#include "GraphicsConstants.h"

bool VIC::isBadLine(u8 rasterCounter, u8 yscroll, bool wasDENSet) {
	return wasDENSet && 0x30u <= rasterCounter && rasterCounter <= 0xf7u && (yscroll == (rasterCounter & 0x7u));
}

void VIC::standardBitmap() {

}

void VIC::standardText() {
	u16 rasterCounter = getRasterCounter();
	u8 vicBank = 0x3u - ((mmu.read(VIC_MEMORY_BANK_ADDR)) & 0x3u);
	u16 vicBaseAddr = vicBank << 14u;

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
			blockRowId = 1;
		} else {
			for (u32 i = 0; i < GraphicsConstants::WINDOW_WIDTH; i++) {
				if (isVerticalBorder(i)) {
					screen.drawPixel(i, rasterCounter - GraphicsConstants::FIRST_BORDER_LINE, 0, borderColor);
				} else {
					u32 pixelId = i - GraphicsConstants::FIRST_VISIBLE_VERTICAL_LINE;

					u8 blockColumn = pixelId / 8;
					u8 blockRow =
							(rasterCounter - GraphicsConstants::FIRST_VISIBLE_LINE -
									GraphicsConstants::FIRST_BORDER_LINE) / 8;

					u8 charColor = getCharColor(colorMemBase, blockColumn, blockRow);
					bool charData = getCharacterPixel(vicBaseAddr, charMemBase, screenMemBase, pixelId, blockColumn,
																						blockRow);
					screen.drawPixel(i, rasterCounter - GraphicsConstants::FIRST_BORDER_LINE, charData,
													 get(BACKGROUND_COLOR_0), charColor);
				}
			}
			blockRowId = (blockRowId + 1) % 8;
		}
	}

	incrementRasterCounter();
}

void VIC::tick() {
	GraphicsModes mode = graphicsMode();

	if (mode == GraphicsModes::StandardText) {
		standardText();
	} else if (mode == GraphicsModes::StandardBitmap) {
		standardBitmap();
	}
}

bool VIC::getCharacterPixel(u16 vicBaseAddr,
														u16 charMemBase,
														u16 screenMemBase,
														u32 pixelId,
														u8 blockColumn,
														u8 blockRow) {
	u8 characterId = mmu.read(vicBaseAddr + screenMemBase + blockRow * 40 + blockColumn, true);
	u8 charPixelId = (blockRowId * 8) + pixelId % 8;
	u16 offset = charMemBase + (8u * characterId);
	bool charData = BIT(mmu.read(offset + charPixelId / 8, true), 7 - (charPixelId % 8));
	return charData;
}

u8 VIC::getCharColor(u16 colorMemBase, u8 blockColumn, u8 blockRow) const {
	return mmu.read(colorMemBase + blockRow * 40 + blockColumn);
}

