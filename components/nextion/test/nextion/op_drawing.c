#include "common_infra_test.h"
#include "nextion/op_drawing.h"

#ifdef __cplusplus
extern "C"
{
#endif

    TEST_CASE("Can clear and fill the screen", "[nextion][op_draw]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_draw_clear_screen(RGB565_COLOR_GREEN);

        TEST_ASSERT_NEX_OK(code);
    }

#ifdef __cplusplus
}
#endif
