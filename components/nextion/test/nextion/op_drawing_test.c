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

    TEST_CASE("Can draw a picture", "[nextion][op_draw]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_draw_picture(0, 100, 100);

        TEST_ASSERT_NEX_OK(code);
    }

    TEST_CASE("Cannot draw an invalid picture", "[nextion][op_draw]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_draw_picture(50, 100, 100);

        TEST_ASSERT_EQUAL_INT(NEX_DVC_ERR_INVALID_PICTURE, code);
    }

    TEST_CASE("Can draw a cropped picture", "[nextion][op_draw]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_draw_crop_picture(0, 0, 0, 100, 100, 0, 0);

        TEST_ASSERT_NEX_OK(code);
    }

    TEST_CASE("Cannot draw an invalid cropped picture", "[nextion][op_draw]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_draw_crop_picture(50, 0, 0, 100, 100, 0, 0);

        TEST_ASSERT_EQUAL_INT(NEX_DVC_ERR_INVALID_PICTURE, code);
    }

    TEST_CASE("Can draw a text", "[nextion][op_draw]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_draw_text(0,
                                           0,
                                           100,
                                           100,
                                           0,
                                           0,
                                           RGB565_COLOR_RED,
                                           RGB565_COLOR_BLACK,
                                           HORZ_ALIGN_CENTER,
                                           VERT_ALIGN_CENTER,
                                           BACKG_FILL_COLOR,
                                           "text");

        TEST_ASSERT_NEX_OK(code);
    }

    TEST_CASE("Cannot draw a text with invalid font", "[nextion][op_draw]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_draw_text(0,
                                           0,
                                           100,
                                           100,
                                           5,
                                           0,
                                           RGB565_COLOR_RED,
                                           RGB565_COLOR_BLACK,
                                           HORZ_ALIGN_CENTER,
                                           VERT_ALIGN_CENTER,
                                           BACKG_FILL_COLOR,
                                           "text");

        TEST_ASSERT_EQUAL_INT(NEX_DVC_ERR_INVALID_FONT, code);
    }

    TEST_CASE("Cannot draw a text with invalid background picture", "[nextion][op_draw]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_draw_text(0,
                                           0,
                                           100,
                                           100,
                                           0,
                                           5,
                                           RGB565_COLOR_RED,
                                           RGB565_COLOR_BLACK,
                                           HORZ_ALIGN_CENTER,
                                           VERT_ALIGN_CENTER,
                                           BACKG_FILL_IMAGE,
                                           "text");

        TEST_ASSERT_EQUAL_INT(NEX_DVC_ERR_INVALID_PICTURE, code);
    }

#ifdef __cplusplus
}
#endif
