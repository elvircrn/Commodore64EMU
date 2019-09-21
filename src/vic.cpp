#include "vic.h"
#include "core.h"
#include "GraphicsConstants.h"

bool VIC::isBadLine(u8 rasterCounter, u8 yscroll, bool wasDENSet) {
	return wasDENSet && 0x30u <= rasterCounter && rasterCounter <= 0xf7u && (yscroll == (rasterCounter & 0x7u));
}

void VIC::standardBitmap() {

}

void VIC::tick() {
	u16 rasterCounter = getRasterCounter();
	// C-Access base
	const u8 vicBank = 0x3u - ((mmu.read(VIC_MEMORY_BANK_ADDR)) & 0x3u);
	const u16 vicBaseAddr = vicBank << 14u; // VM15 - VM14 provided by CIA2 chip
	const u8 memoryPointers = get(MEMORY_POINTERS); //
	const u16 screenMemBase = (HI_NIBBLE(memoryPointers) << 10u); // Screen memory addresss, VM13 - VM10 will come from this register
	const u16 cAccessAddressBase = vicBaseAddr | screenMemBase;


	// Character memory base
	const u8 cb = (u8) (memoryPointers >> 0x1u) & 0x7u;
	u16 charMemBase = (cb << 11u); // CB13 CB12 CB11

	// Color rom base
	const u16 colorMemBase = 0xd800u;

	u8 blockWidth = 8;
	u8 blockHeight = 8;

	if (!isVBlank(rasterCounter)) {
		u8 borderColor = get(BORDER_COLOR);
		if (isHorizontalBorder(rasterCounter)) {
			for (size_t i = 0; i < GraphicsConstants::WINDOW_WIDTH; i++) {
				screen.drawPixel(i, rasterCounter - GraphicsConstants::FIRST_BORDER_LINE, 0, borderColor);
			}
		} else {
			u16 y = rasterCounter - GraphicsConstants::FIRST_VISIBLE_LINE - GraphicsConstants::FIRST_BORDER_LINE;
			u16 blockRow = y / blockHeight;
			for (u32 i = 0; i < GraphicsConstants::WINDOW_WIDTH; i++) {
				if (isVerticalBorder(i)) {
					screen.drawPixel(i, rasterCounter - GraphicsConstants::FIRST_BORDER_LINE, 0, borderColor);
				} else {
					u32 x = i - GraphicsConstants::FIRST_VISIBLE_VERTICAL_LINE;
					u8 blockColumn = x / blockWidth;

					u16 screenCellLocation = blockRow * 40 + blockColumn;
					u16 cAccessFullAddress = cAccessAddressBase | screenCellLocation;
					// C-Access
					u16 cData = mmu.read(cAccessFullAddress, true); // This tells us which character we want to draw, D7 - D0

					// G-Access
					u16 gAccessAddress = charMemBase | (cData << 0x3u) | (y % 8u);

					u16 characterData = mmu.read(gAccessAddress, true);

					bool pixel = BIT(characterData, 7 - (x % 8));

					u8 charColor = getCharColor(colorMemBase, blockColumn, blockRow);

					screen.drawPixel(i, rasterCounter - GraphicsConstants::FIRST_BORDER_LINE, pixel, get(BACKGROUND_COLOR_0), charColor);
				}
			}
		}
	}
	incrementRasterCounter();
}

u8 VIC::getCharColor(u16 colorMemBase, u8 blockColumn, u8 blockRow) const {
	return mmu.read(colorMemBase + blockRow * 40 + blockColumn);
}

