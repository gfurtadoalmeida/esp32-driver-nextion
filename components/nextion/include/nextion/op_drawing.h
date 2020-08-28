#ifndef __NEXTION_OP_DRAWING_H__
#define __NEXTION_OP_DRAWING_H__

#include <stdint.h>
#include "codes.h"
#include "rgb565/rgb565.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Fill an area.
     *
     * @param x Upper left corner X coordinate.
     * @param y Upper left corner Y coordinate.
     * @param width Width of the area.
     * @param heigth Height of the area.
     * @param color Color used to fill.
     *
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_draw_fill(uint16_t x, uint16_t y, uint16_t width, uint16_t height, rgb565_t color);

    /**
     * @brief Fills the screen with a color, erasing everything.
     * @param color Color used to fill.
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_draw_fill_screen(rgb565_t color);

    /**
     * @brief Draws a filled circle.
     *
     * @param cx Center position X coordinate.
     * @param cy Center position Y coordinate.
     * @param radius Radius, in pixels.
     * @param color Color used to fill.
     *
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_draw_fill_circle(uint16_t cx, uint16_t cy, uint16_t radius, rgb565_t color);

    /**
     * @brief Draws a filled rectangle.
     *
     * @param x1 Upper left corner X coordinate.
     * @param y1 Upper left corner Y coordinate.
     * @param x2 Lower right corner X coordinate.
     * @param y2 Lower right corner Y coordinate.
     * @param color Color used to fill.
     *
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_draw_fill_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, rgb565_t color);

    /**
     * @brief Draws a line, from point to point.
     *
     * @param x1 Start position X coordinate.
     * @param y1 Start position Y coordinate.
     * @param x2 End position X coordinate.
     * @param y2 End position Y coordinate.
     * @param color Color used to draw.
     *
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, rgb565_t color);

    /**
     * @brief Draws a hollow rectangle.
     *
     * @param x1 Upper left corner X coordinate.
     * @param y1 Upper left corner Y coordinate.
     * @param x2 Lower right corner X coordinate.
     * @param y2 Lower right corner Y coordinate.
     * @param color Color used to draw.
     *
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, rgb565_t color);

    /**
     * @brief Draws a holow circle.
     *
     * @param cx Center position X coordinate.
     * @param cy Center position Y coordinate.
     * @param radius Radius, in pixels.
     * @param color Color used to draw.
     *
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_draw_circle(uint16_t cx, uint16_t cy, uint16_t radius, rgb565_t color);

#ifdef __cplusplus
}
#endif

#endif //__NEXTION_OP_DRAWING_H__