#include "vic.h"

#include "core.h"

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

void VIC::tick() {
	u8 rasterCounter = get(RASTER_COUNTER_ADDR);

	u8 memoryPointers = get(MEMORY_POINTERS);
	u8 vm = memoryPointers >> 0x4u;
	u8 cb = (u8) (memoryPointers >> 0x1u) & 0x7u;


	rasterCounter++;
	set(RASTER_COUNTER_ADDR, rasterCounter);
}
