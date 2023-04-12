#include "esp32_driver_nextion/drawing.h"
#include "common_infra_test.h"

TEST_CASE("Fill screen", "[draw]")
{
    nex_err_t code = nextion_draw_fill_screen(handle, RGB565_COLOR_GREEN);

    CHECK_NEX_OK(code);
}

TEST_CASE("Fill area", "[draw]")
{
    area_t area = {
        .upper_left = {.x = 0, .y = 0},
        .bottom_right = {.x = 100, .y = 100}};

    nex_err_t code = nextion_draw_fill_area(handle, area, RGB565_COLOR_GREEN);

    CHECK_NEX_OK(code);
}

TEST_CASE("Fill circle", "[draw]")
{
    point_t center = {.x = 100, .y = 100};

    nex_err_t code = nextion_draw_fill_circle(handle, center, 20, RGB565_COLOR_GREEN);

    CHECK_NEX_OK(code);
}

TEST_CASE("Draw line", "[draw]")
{
    area_t line = {
        .upper_left = {.x = 30, .y = 30},
        .bottom_right = {.x = 150, .y = 100}};

    nex_err_t code = nextion_draw_line(handle, line, RGB565_COLOR_GREEN);

    CHECK_NEX_OK(code);
}

TEST_CASE("Draw circle", "[draw]")
{
    point_t center = {.x = 100, .y = 100};

    nex_err_t code = nextion_draw_circle(handle, center, 20, RGB565_COLOR_GREEN);

    CHECK_NEX_OK(code);
}

TEST_CASE("Draw rectangle", "[draw]")
{
    area_t rectangle = {
        .upper_left = {.x = 20, .y = 20},
        .bottom_right = {.x = 100, .y = 100}};

    nex_err_t code = nextion_draw_rectangle(handle, rectangle, RGB565_COLOR_GREEN);

    CHECK_NEX_OK(code);
}

TEST_CASE("Draw picture", "[draw]")
{
    point_t origin = {.x = 100, .y = 100};

    nex_err_t code = nextion_draw_picture(handle, 0, origin);

    CHECK_NEX_OK(code);
}

TEST_CASE("Cannot draw invalid picture", "[draw]")
{
    point_t origin = {.x = 100, .y = 100};

    nex_err_t code = nextion_draw_picture(handle, 50, origin);

    NEX_CODES_EQUAL(NEX_DVC_ERR_INVALID_PICTURE, code);
}

TEST_CASE("Draw cropped picture", "[draw]")
{
    point_t origin = {.x = 100, .y = 100};
    area_t crop_area = {
        .upper_left = {.x = 0, .y = 0},
        .bottom_right = {.x = 100, .y = 100}};

    nex_err_t code = nextion_draw_crop_picture(handle, 0, crop_area, origin);

    CHECK_NEX_OK(code);
}

TEST_CASE("Cannot draw invalid cropped picture", "[draw]")
{
    point_t origin = {.x = 100, .y = 100};
    area_t crop_area = {
        .upper_left = {.x = 0, .y = 0},
        .bottom_right = {.x = 100, .y = 100}};

    nex_err_t code = nextion_draw_crop_picture(handle, 50, crop_area, origin);

    NEX_CODES_EQUAL(NEX_DVC_ERR_INVALID_PICTURE, code);
}

TEST_CASE("Draw text", "[draw]")
{
    area_t area = {
        .upper_left = {.x = 0, .y = 0},
        .bottom_right = {.x = 100, .y = 100}};
    font_t font = {.id = 0, .color = RGB565_COLOR_RED};
    background_t background = {.fill_mode = BACKG_FILL_COLOR, .picture_id = 0, .color = RGB565_COLOR_BLACK};
    text_alignment_t text_align = {.horizontal = HORZ_ALIGN_CENTER, .vertical = VERT_ALIGN_CENTER};

    nex_err_t code = nextion_draw_text(handle,
                                       area,
                                       font,
                                       background,
                                       text_align,
                                       "text");

    CHECK_NEX_OK(code);
}

TEST_CASE("Cannot draw text with invalid font", "[draw]")
{
    area_t area = {
        .upper_left = {.x = 0, .y = 0},
        .bottom_right = {.x = 100, .y = 100}};
    font_t font = {.id = 5, .color = RGB565_COLOR_RED};
    background_t background = {.fill_mode = BACKG_FILL_COLOR, .picture_id = 0, .color = RGB565_COLOR_BLACK};
    text_alignment_t text_align = {.horizontal = HORZ_ALIGN_CENTER, .vertical = VERT_ALIGN_CENTER};

    nex_err_t code = nextion_draw_text(handle,
                                       area,
                                       font,
                                       background,
                                       text_align,
                                       "text");

    NEX_CODES_EQUAL(NEX_DVC_ERR_INVALID_FONT, code);
}

TEST_CASE("Cannot draw text with invalid background picture", "[draw]")
{
    area_t area = {
        .upper_left = {.x = 0, .y = 0},
        .bottom_right = {.x = 100, .y = 100}};
    font_t font = {.id = 0, .color = RGB565_COLOR_RED};
    background_t background = {.fill_mode = BACKG_FILL_IMAGE, .picture_id = 50, .color = RGB565_COLOR_BLACK};
    text_alignment_t text_align = {.horizontal = HORZ_ALIGN_CENTER, .vertical = VERT_ALIGN_CENTER};

    nex_err_t code = nextion_draw_text(handle,
                                       area,
                                       font,
                                       background,
                                       text_align,
                                       "text");

    NEX_CODES_EQUAL(NEX_DVC_ERR_INVALID_PICTURE, code);
}

TEST_CASE("Cannot draw text with invalid background crop picture", "[draw]")
{
    area_t area = {
        .upper_left = {.x = 0, .y = 0},
        .bottom_right = {.x = 100, .y = 100}};
    font_t font = {.id = 0, .color = RGB565_COLOR_RED};
    background_t background = {.fill_mode = BACKG_FILL_CROP_IMAGE, .picture_id = 50, .color = RGB565_COLOR_BLACK};
    text_alignment_t text_align = {.horizontal = HORZ_ALIGN_CENTER, .vertical = VERT_ALIGN_CENTER};

    nex_err_t code = nextion_draw_text(handle,
                                       area,
                                       font,
                                       background,
                                       text_align,
                                       "text");

    NEX_CODES_EQUAL(NEX_DVC_ERR_INVALID_PICTURE, code);
}
