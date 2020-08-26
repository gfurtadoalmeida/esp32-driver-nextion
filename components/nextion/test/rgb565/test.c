#include "unity.h"
#include "unity_test_runner.h"
#include "rgb565/rgb565.h"

#ifdef __cplusplus
extern "C"
{
#endif

    // Just need to test the primary colors conversion.

    TEST_CASE("Can convert from 888 red", "[rgb565]")
    {
        TEST_ASSERT_EQUAL_UINT16(RGB565_COLOR_RED, rgb565_convert_from_888(255, 0, 0));
    }

    TEST_CASE("Can convert from 888 green", "[rgb565]")
    {
        TEST_ASSERT_EQUAL_UINT16(RGB565_COLOR_GREEN, rgb565_convert_from_888(0, 255, 0));
    }

    TEST_CASE("Can convert from 888 blue", "[rgb565]")
    {
        TEST_ASSERT_EQUAL_UINT16(RGB565_COLOR_BLUE, rgb565_convert_from_888(0, 0, 255));
    }
#ifdef __cplusplus
}
#endif
