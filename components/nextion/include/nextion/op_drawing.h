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
     * @brief Clears the screen and fill it with a color.
     * @param color Color used to fill the screen.
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_draw_clear_screen(rgb565_t color);

#ifdef __cplusplus
}
#endif

#endif //__NEXTION_OP_DRAWING_H__