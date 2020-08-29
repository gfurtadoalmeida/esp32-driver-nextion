#include <string.h>
#include "common_infra.h"
#include "nextion/nextion.h"
#include "nextion/op_drawing.h"

#ifdef __cplusplus
extern "C"
{
#endif

    nex_err_t nextion_draw_fill(uint16_t x, uint16_t y, uint16_t width, uint16_t height, rgb565_t color)
    {
        return NEX_SEND_COMMAND(35, "fill %d,%d,%d,%d,%d", x, y, width, height, color);
    }

    nex_err_t nextion_draw_fill_screen(rgb565_t color)
    {
        return NEX_SEND_COMMAND(10, "cls %d", color);
    }

    nex_err_t nextion_draw_fill_circle(uint16_t cx, uint16_t cy, uint16_t radius, rgb565_t color)
    {
        return NEX_SEND_COMMAND(30, "cirs %d,%d,%d,%d", cx, cy, radius, color);
    }

    nex_err_t nextion_draw_fill_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, rgb565_t color)
    {
        return nextion_draw_fill(x1, x2, (x2 - x1), (y2 - y1), color);
    }

    nex_err_t nextion_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, rgb565_t color)
    {
        return NEX_SEND_COMMAND(35, "line %d,%d,%d,%d,%d", x1, y1, x2, y2, color);
    }

    nex_err_t nextion_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, rgb565_t color)
    {
        return NEX_SEND_COMMAND(35, "draw %d,%d,%d,%d,%d", x1, y1, x2, y2, color);
    }

    nex_err_t nextion_draw_circle(uint16_t cx, uint16_t cy, uint16_t radius, rgb565_t color)
    {
        return NEX_SEND_COMMAND(30, "cir %d,%d,%d,%d", cx, cy, radius, color);
    }

    nex_err_t nextion_draw_picture(uint8_t picture_id, uint16_t x, uint16_t y)
    {
        return NEX_SEND_COMMAND(25, "pic %d,%d,%d", x, y, picture_id);
    }

    nex_err_t nextion_draw_crop_picture(uint8_t picture_id,
                                        uint16_t src_x,
                                        uint16_t src_y,
                                        uint16_t width,
                                        uint16_t height,
                                        uint16_t dest_x,
                                        uint16_t dest_y)
    {
        return NEX_SEND_COMMAND(45,
                                "xpic %d,%d,%d,%d,%d,%d,%d",
                                src_x,
                                src_y,
                                width,
                                height,
                                dest_x,
                                dest_y,
                                picture_id);
    }

    nex_err_t nextion_draw_text(uint16_t x,
                                uint16_t y,
                                uint16_t fill_area_width,
                                uint16_t fill_area_height,
                                uint8_t font_id,
                                uint8_t background_picture_id,
                                rgb565_t background_color,
                                rgb565_t text_color,
                                horizontal_align_t horizontal_alignment,
                                vertical_align_t vertical_alignment,
                                background_fill_mode_t background_fill,
                                const char *text)
    {
        NEX_CHECK((text != NULL), "text error(NULL)", NEX_FAIL);

        // It's not a problem having a background value if the fill mode is "none".
        uint16_t background_value = background_picture_id;

        if (background_fill == BACKG_FILL_COLOR)
        {
            background_value = background_color;
        }

        const int text_length = strlen(text);

        return NEX_SEND_COMMAND(68 + text_length,
                                "xstr %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\"%s\"",
                                x,
                                y,
                                fill_area_width,
                                fill_area_height,
                                font_id,
                                text_color,
                                background_value,
                                horizontal_alignment,
                                vertical_alignment,
                                background_fill,
                                text);
    }

#ifdef __cplusplus
}
#endif