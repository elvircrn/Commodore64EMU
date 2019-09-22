#ifndef C64EMU_GRAPHICS_CONSTANTS_H
#define C64EMU_GRAPHICS_CONSTANTS_H

#include "core.h"

class GraphicsConstants {
public:
	static constexpr u16 WINDOW_WIDTH = 403;
	static constexpr u16 WINDOW_HEIGHT = 284;

	static constexpr u16 TEXT_AREA_HEIGHT = 200;
	static constexpr u16 TEXT_AREA_WIDTH = 320;
	static constexpr u16 FIRST_VISIBLE_LINE = 42;
	static constexpr u16 LAST_VISIBLE_LINE = TEXT_AREA_HEIGHT + FIRST_VISIBLE_LINE;

	static constexpr u16 FIRST_VISIBLE_VERTICAL_LINE = 42;
	static constexpr u16 LAST_VISIBLE_VERTICAL_LINE = 361;

	static constexpr u16 FIRST_BORDER_LINE = 14;
	static constexpr u16 LAST_BORDER_LINE = 284; // TODO: Can be computed
	static constexpr u16 BORDER_DIMENSIONS = 42;
};

#endif //C64EMU_GRAPHICS_CONSTANTS_H
