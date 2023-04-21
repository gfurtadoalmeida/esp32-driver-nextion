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
        HORZ_ALIGN_LEFT = 0,   /** @brief Left aligned. */
        HORZ_ALIGN_CENTER = 1, /** @brief Center aligned. */
        HORZ_ALIGN_RIGHT = 2   /** @brief Right aligned. */
    } horizontal_align_t;

    /**
     * @typedef vertical_align_t
     * @brief Text vertical alignment.
     */
    typedef enum
    {
        VERT_ALIGN_TOP = 0,    /** @brief Top aligned. */
        VERT_ALIGN_CENTER = 1, /** @brief Center aligned. */
        VERT_ALIGN_BOTTOM = 2  /** @brief Bottom aligned. */
    } vertical_align_t;

    /**
     * @typedef background_fill_mode_t
     * @brief Text background fill mode.
     */
    typedef enum
    {
        BACKG_FILL_CROP_IMAGE = 0, /** @brief Fill with a cropped image. */
        BACKG_FILL_COLOR = 1,      /** @brief Fill a color. */
        BACKG_FILL_IMAGE = 2,      /** @brief Fill with an image. */
        BACKG_FILL_NONE = 3        /** @brief No background. */
    } background_fill_mode_t;

    /**
     * @typedef point_t
     * @brief A point/position on the screen.
     */
    typedef struct
    {
        uint16_t x; /** @brief X coordinate. */
        uint16_t y; /** @brief Y coordinate. */
    } point_t;

    /**
     * @typedef area_t
     * @brief An area on the screen.
     */
    typedef struct
    {
        point_t upper_left;   /** @brief Upper left position. */
        point_t bottom_right; /** @brief Bottom right position. */
    } area_t;

    /**
     * @typedef font_t
     * @brief A font.
     */
    typedef struct
    {
        rgb565_t color; /** @brief Color. */
        uint8_t id;     /** @brief Id. */
    } font_t;

    /**
     * @typedef background_t
     * @brief Background configuration.
     */
    typedef struct
    {
        background_fill_mode_t fill_mode; /** @brief Fill mode. */
        rgb565_t color;                   /** @brief Color. Used when fill_mode is BACKG_FILL_COLOR. */
        uint8_t picture_id;               /** @brief Picture id. Used when fill_mode is BACKG_FILL_CROP_IMAGE or BACKG_FILL_CROP_IMAGE. */
    } background_t;

    /**
     * @typedef text_alignment_t
     * @brief Text alignment.
     */
    typedef struct
    {
        horizontal_align_t horizontal; /** @brief Horizontal alignment. */
        vertical_align_t vertical;     /** @brief Vertical alignment. */
    } text_alignment_t;

    /**
     * @brief Fill the screen with a color, erasing everything.
     * @param[in] handle Nextion context pointer.
     * @param[in] color Color used to fill.
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_draw_fill_screen(nextion_t *handle, rgb565_t color);

    /**
     * @brief Draw a filled rectangle.
     * @param[in] handle Nextion context pointer.
     * @param[in] x1 Upper left corner X coordinate.
     * @param[in] y1 Upper left corner Y coordinate.
     * @param[in] x2 Lower right corner X coordinate.
     * @param[in] y2 Lower right corner Y coordinate.
     * @param[in] color Color used to fill.
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_draw_fill_area(nextion_t *handle,
                                     area_t area,
                                     rgb565_t color);

    /**
     * @brief Draw a filled circle.
     * @param[in] handle Nextion context pointer.
     * @param[in] cx Center position X coordinate.
     * @param[in] cy Center position Y coordinate.
     * @param[in] radius Radius, in pixels.
     * @param[in] color Color used to fill.
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_draw_fill_circle(nextion_t *handle,
                                       point_t center,
                                       uint16_t radius,
                                       rgb565_t color);

    /**
     * @brief Draw a line, from point to point.
     * @param[in] handle Nextion context pointer.
     * @param[in] x1 Start position X coordinate.
     * @param[in] y1 Start position Y coordinate.
     * @param[in] x2 End position X coordinate.
     * @param[in] y2 End position Y coordinate.
     * @param[in] color Color used to draw.
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_draw_line(nextion_t *handle,
                                area_t area,
                                rgb565_t color);

    /**
     * @brief Draw a hollow rectangle.
     * @param[in] handle Nextion context pointer.
     * @param[in] x1 Upper left corner X coordinate.
     * @param[in] y1 Upper left corner Y coordinate.
     * @param[in] x2 Lower right corner X coordinate.
     * @param[in] y2 Lower right corner Y coordinate.
     * @param[in] color Color used to draw.
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_draw_rectangle(nextion_t *handle,
                                     area_t area,
                                     rgb565_t color);

    /**
     * @brief Draw a hollow circle.
     * @param[in] handle Nextion context pointer.
     * @param[in] cx Center position X coordinate.
     * @param[in] cy Center position Y coordinate.
     * @param[in] radius Radius, in pixels.
     * @param[in] color Color used to draw.
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_draw_circle(nextion_t *handle,
                                  point_t center,
                                  uint16_t radius,
                                  rgb565_t color);

    /**
     * @brief Draw a picture.
     * @param[in] handle Nextion context pointer.
     * @param[in] picture_id Picture id.
     * @param[in] x Upper left corner X coordinate.
     * @param[in] y Upper left corner Y coordinate.
     * @return NEX_OK or NEX_DVC_ERR_INVALID_PICTURE.
     */
    nex_err_t nextion_draw_picture(nextion_t *handle,
                                   uint8_t picture_id,
                                   point_t origin);

    /**
     * @brief Draw a cropped picture.
     * @param[in] handle Nextion context pointer.
     * @param[in] picture_id Picture id.
     * @param[in] src_x Upper left corner, X coordinate, of the area to be cropped.
     * @param[in] src_y Upper left corner, Y coordinate, of the area to be cropped.
     * @param[in] width Width of the area.
     * @param[in] heigth Height of the area.
     * @param[in] dest_x Upper left corner, X coordinate of, where the crop will be drawn.
     * @param[in] dest_y Upper left corner, Y coordinate of, where the crop will be drawn.
     * @return NEX_OK or NEX_DVC_ERR_INVALID_PICTURE.
     */
    nex_err_t nextion_draw_crop_picture(nextion_t *handle,
                                        uint8_t picture_id,
                                        area_t crop_area,
                                        point_t destination);

    /**
     * @brief Draw a text.
     * @param[in] handle Nextion context pointer.
     * @param[in] x Upper left corner X coordinate.
     * @param[in] y Upper left corner Y coordinate.
     * @param[in] fill_area_width Width of the area.
     * @param[in] fill_area_heigth Height of the area.
     * @param[in] font_id Font id.
     * @param[in] background_picture_id Picture id used to fill the background, when "background_fill == Picture".
     * @param[in] background_color Color used to fill the background, when "background_fill == Color".
     * @param[in] text_color Text color.
     * @param[in] horizontal_alignment Text horizontal alignment.
     * @param[in] vertical_alignment Text vertical alignment.
     * @param[in] background_fill Background fill mode.
     * @param[in] text Null-terminated text to be drawn.
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