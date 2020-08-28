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
    * @typedef horizontal_align_t
    * @brief Text horizontal alignment.
    */
    typedef enum
    {
        HORZ_ALIGN_LEFT = 0,   /*!< Left aligned. */
        HORZ_ALIGN_CENTER = 1, /*!< Center aligned. */
        HORZ_ALIGN_RIGHT = 2   /*!< Right aligned. */
    } horizontal_align_t;

    /**
    * @typedef vertical_align_t
    * @brief Text vertical alignment.
    */
    typedef enum
    {
        VERT_ALIGN_TOP = 0,    /*!< Top aligned. */
        VERT_ALIGN_CENTER = 1, /*!< Center aligned. */
        VERT_ALIGN_BOTTOM = 2  /*!< Bottom aligned. */
    } vertical_align_t;

    /**
    * @typedef background_fill_mode_t
    * @brief Text background fill mode.
    */
    typedef enum
    {
        BACKG_FILL_CROP_IMAGE = 0, /*!< Fill with a cropped image. */
        BACKG_FILL_COLOR = 1,      /*!< Fill a color. */
        BACKG_FILL_IMAGE = 2,      /*!< Fill with an image. */
        BACKG_FILL_NONE = 3        /*!< No background. */
    } background_fill_mode_t;

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
     * @brief Draws a hollow circle.
     *
     * @param cx Center position X coordinate.
     * @param cy Center position Y coordinate.
     * @param radius Radius, in pixels.
     * @param color Color used to draw.
     *
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_draw_circle(uint16_t cx, uint16_t cy, uint16_t radius, rgb565_t color);

    /**
     * @brief Draws a picture.
     *
     * @param picture_id Picture id.
     * @param x Upper left corner X coordinate.
     * @param y Upper left corner Y coordinate.
     *
     * @return NEX_OK or NEX_DVC_ERR_INVALID_PICTURE.
     */
    nex_err_t nextion_draw_picture(uint8_t picture_id, uint16_t x, uint16_t y);

    /**
     * @brief Draws a picture.
     *
     * @param picture_id Picture id.
     * @param src_x Upper left corner, X coordinate, of the area to be cropped.
     * @param src_y Upper left corner, Y coordinate, of the area to be cropped.
     * @param width Width of the area.
     * @param heigth Height of the area.
     * @param dest_x Upper left corner, X coordinate of, where the crop will be drawn.
     * @param dest_y Upper left corner, Y coordinate of, where the crop will be drawn.
     *
     * @return NEX_OK or NEX_DVC_ERR_INVALID_PICTURE.
     */
    nex_err_t nextion_draw_crop_picture(uint8_t picture_id,
                                        uint16_t src_x,
                                        uint16_t src_y,
                                        uint16_t width,
                                        uint16_t height,
                                        uint16_t dest_x,
                                        uint16_t dest_y);

    /**
     * @brief Draws a text.
     *
     * @param x Upper left corner X coordinate.
     * @param y Upper left corner Y coordinate.
     * @param fill_area_width Width of the area.
     * @param fill_area_heigth Height of the area.
     * @param font_id Font id.
     * @param background_picture_id Picture id used to fill the background, when "background_fill == Picture".
     * @param background_color Color used to fill the background, when "background_fill == Color".
     * @param text_color Text color.
     * @param horizontal_alignment Text horizontal alignment.
     * @param vertical_alignment Text vertical alignment.
     * @param background_fill Background fill mode.
     * @param text Null-terminated text to be drawn.
     *
     * @return NEX_OK or NEX_DVC_ERR_INVALID_FONT | NEX_DVC_ERR_INVALID_PICTURE.
     */
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
                                const char *text);

#ifdef __cplusplus
}
#endif

#endif //__NEXTION_OP_DRAWING_H__