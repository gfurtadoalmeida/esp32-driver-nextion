#include <string.h>
#include "common_infra.h"
#include "nextion/nextion.h"
#include "nextion/op_drawing.h"

#ifdef __cplusplus
extern "C"
{
#endif

    nex_err_t nextion_draw_clear_screen(rgb565_t color)
    {
        return NEX_SEND_COMMAND(10, "cls %d", color);
    }

#ifdef __cplusplus
}
#endif