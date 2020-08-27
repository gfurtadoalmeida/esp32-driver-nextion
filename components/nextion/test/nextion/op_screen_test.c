#include "common_infra_test.h"
#include "nextion/op_screen.h"

#ifdef __cplusplus
extern "C"
{
#endif

    TEST_CASE("Can get the current page", "[nextion][op_screen]")
    {
        INSTALL()

        int page_id;
        nex_err_t code = nextion_get_current_page(&page_id);

        DELETE()

        TEST_ASSERT_NEX_OK(code);
        TEST_ASSERT_EQUAL_INT(0, page_id);
    }

#ifdef __cplusplus
}
#endif
