#include "catch.hpp"
#include "core.h"

TEST_CASE("Bit Test") {
	CHECK(BIT(129, 0) == 1);
}


TEST_CASE("Set Test") {
	for (u8 i = 0; i < 8; i++) {
		CHECK(SET(0, i, 1) == (1u << i));
	}

	for (u8 i = 0; i < 8; i++) {
		CHECK(SET(0xff, i, 0) == (0xffu - (1u << i)));
	}

	CHECK(SET(0b0010, 1, 1) == 0b0010);
	CHECK(SET(0b0010, 0, 1) == 0b0011);
}
