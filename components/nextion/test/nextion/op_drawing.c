#include "common_infra_test.h"
#include "nextion/op_drawing.h"

#ifdef __cplusplus
extern "C"
{
#endif

    TEST_CASE("Can fill the screen", "[nextion][op_draw]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_draw_fill_screen(RGB565_COLOR_GREEN);

        TEST_ASSERT_NEX_OK(code);
    }

    TEST_CASE("Can fill an area", "[nextion][op_draw]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_draw_fill(0, 0, 100, 100, RGB565_COLOR_GREEN);

        TEST_ASSERT_NEX_OK(code);
    }

    TEST_CASE("Can fill a circle", "[nextion][op_draw]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_draw_fill_circle(100, 100, 20, RGB565_COLOR_GREEN);

        TEST_ASSERT_NEX_OK(code);
    }

    TEST_CASE("Can fill a rectangle", "[nextion][op_draw]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_draw_fill_rectangle(20, 20, 100, 100, RGB565_COLOR_GREEN);

        TEST_ASSERT_NEX_OK(code);
    }

    TEST_CASE("Can draw a line", "[nextion][op_draw]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_draw_line(30, 30, 150, 100, RGB565_COLOR_GREEN);

        TEST_ASSERT_NEX_OK(code);
    }

    TEST_CASE("Can draw a circle", "[nextion][op_draw]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_draw_circle(100, 100, 20, RGB565_COLOR_GREEN);

        TEST_ASSERT_NEX_OK(code);
    }

    TEST_CASE("Can draw a rectangle", "[nextion][op_draw]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_draw_rectangle(20, 20, 100, 100, RGB565_COLOR_GREEN);

        TEST_ASSERT_NEX_OK(code);
    }

#ifdef __cplusplus
}
#endif
