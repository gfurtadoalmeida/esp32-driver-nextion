#include "common_infra_test.h"
#include "rgb565/rgb565.h"

TEST_CASE("Convert from 888 red", "[rgb565]")
{
    RGB565_EQUAL(RGB565_COLOR_RED, rgb565_convert_from_888(255, 0, 0));
}

TEST_CASE("Convert from 888 green", "[rgb565]")
{
    RGB565_EQUAL(RGB565_COLOR_GREEN, rgb565_convert_from_888(0, 255, 0));
}

TEST_CASE("Convert from 888 blue", "[rgb565]")
{
    RGB565_EQUAL(RGB565_COLOR_BLUE, rgb565_convert_from_888(0, 0, 255));
}
