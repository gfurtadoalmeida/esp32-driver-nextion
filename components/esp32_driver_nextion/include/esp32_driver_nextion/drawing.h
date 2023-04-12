#ifndef __ESP32_DRIVER_NEXTION_DRAWING_H__
#define __ESP32_DRIVER_NEXTION_DRAWING_H__

#include <stdint.h>
#include "rgb565/rgb565.h"
#include "base/codes.h"
#include "base/types.h"

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
     * @typedef point_t
     * @brief A point/position on the screen.
     */
    typedef struct
    {
        uint16_t x; /*!< X coordinate. */
        uint16_t y; /*!< Y coordinate. */
    } point_t;

    /**
     * @typedef area_t
     * @brief An area on the screen.
     */
    typedef struct
    {
        point_t upper_left;   /*!< Upper left position. */
        point_t bottom_right; /*!< Bottom right position. */
    } area_t;

    /**
     * @typedef font_t
     * @brief A font.
     */
    typedef struct
    {
        rgb565_t color; /*!< Color. */
        uint8_t id;     /*!< Id. */
    } font_t;

    /**
     * @typedef background_t
     * @brief Background configuration.
     */
    typedef struct
    {
        background_fill_mode_t fill_mode; /*!< Fill mode. */
        rgb565_t color;                   /*!< Color. Used when fill_mode is BACKG_FILL_COLOR. */
        uint8_t picture_id;               /*!< Picture id. Used when fill_mode is BACKG_FILL_CROP_IMAGE or BACKG_FILL_CROP_IMAGE. */
    } background_t;

    /**
     * @typedef text_alignment_t
     * @brief Text alignment.
     */
    typedef struct
    {
        horizontal_align_t horizontal; /*!< Horizontal alignment. */
        vertical_align_t vertical;     /*!< Vertical alignment. */
    } text_alignment_t;

    /**
     * @brief Fills the screen with a color, erasing everything.
     * @param handle Nextion context pointer.
     * @param color Color used to fill.
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_draw_fill_screen(nextion_t *handle, rgb565_t color);

    /**
     * @brief Draws a filled rectangle.
     *
     * @param handle Nextion context pointer.
     * @param x1 Upper left corner X coordinate.
     * @param y1 Upper left corner Y coordinate.
     * @param x2 Lower right corner X coordinate.
     * @param y2 Lower right corner Y coordinate.
     * @param color Color used to fill.
     *
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_draw_fill_area(nextion_t *handle,
                                     area_t area,
                                     rgb565_t color);

    /**
     * @brief Draws a filled circle.
     *
     * @param handle Nextion context pointer.
     * @param cx Center position X coordinate.
     * @param cy Center position Y coordinate.
     * @param radius Radius, in pixels.
     * @param color Color used to fill.
     *
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_draw_fill_circle(nextion_t *handle,
                                       point_t center,
                                       uint16_t radius,
                                       rgb565_t color);

    /**
     * @brief Draws a line, from point to point.
     *
     * @param handle Nextion context pointer.
     * @param x1 Start position X coordinate.
     * @param y1 Start position Y coordinate.
     * @param x2 End position X coordinate.
     * @param y2 End position Y coordinate.
     * @param color Color used to draw.
     *
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_draw_line(nextion_t *handle,
                                area_t area,
                                rgb565_t color);

    /**
     * @brief Draws a hollow rectangle.
     *
     * @param handle Nextion context pointer.
     * @param x1 Upper left corner X coordinate.
     * @param y1 Upper left corner Y coordinate.
     * @param x2 Lower right corner X coordinate.
     * @param y2 Lower right corner Y coordinate.
     * @param color Color used to draw.
     *
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_draw_rectangle(nextion_t *handle,
                                     area_t area,
                                     rgb565_t color);

    /**
     * @brief Draws a hollow circle.
     *
     * @param handle Nextion context pointer.
     * @param cx Center position X coordinate.
     * @param cy Center position Y coordinate.
     * @param radius Radius, in pixels.
     * @param color Color used to draw.
     *
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_draw_circle(nextion_t *handle,
                                  point_t center,
                                  uint16_t radius,
                                  rgb565_t color);

    /**
     * @brief Draws a picture.
     *
     * @param handle Nextion context pointer.
     * @param picture_id Picture id.
     * @param x Upper left corner X coordinate.
     * @param y Upper left corner Y coordinate.
     *
     * @return NEX_OK or NEX_DVC_ERR_INVALID_PICTURE.
     */
    nex_err_t nextion_draw_picture(nextion_t *handle,
                                   uint8_t picture_id,
                                   point_t origin);

    /**
     * @brief Draws a cropped picture.
     *
     * @param handle Nextion context pointer.
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
    nex_err_t nextion_draw_crop_picture(nextion_t *handle,
                                        uint8_t picture_id,
                                        area_t crop_area,
                                        point_t destination);

    /**
     * @brief Draws a text.
     *
     * @param handle Nextion context pointer.
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
     * @return NEX_OK or NEX_FAIL | NEX_DVC_ERR_INVALID_FONT | NEX_DVC_ERR_INVALID_PICTURE.
     */
    nex_err_t nextion_draw_text(nextion_t *handle,
                                area_t area,
                                font_t font,
                                background_t background,
                                text_alignment_t alignment,
                                const char *text);

#ifdef __cplusplus
}
#endif
#endif