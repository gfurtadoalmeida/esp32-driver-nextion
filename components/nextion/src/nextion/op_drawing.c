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

#ifdef __cplusplus
}
#endif