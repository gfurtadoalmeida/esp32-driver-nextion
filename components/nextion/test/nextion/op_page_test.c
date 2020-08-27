#include "common_infra_test.h"
#include "nextion/op_page.h"

#ifdef __cplusplus
extern "C"
{
#endif

    TEST_CASE("Can get the current page", "[nextion][op_page]")
    {
        DRIVER_INSTALL()

        nextion_set_page(1);

        uint8_t page_id;
        nex_err_t code = nextion_get_page(&page_id);

        nextion_set_page(0);

        TEST_ASSERT_NEX_OK(code);
        TEST_ASSERT_EQUAL_INT(1, page_id);
    }

    TEST_CASE("Can change the page", "[nextion][op_page]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_set_page(1);

        nextion_set_page(0);

        TEST_ASSERT_NEX_OK(code);
    }

    TEST_CASE("Cannot change the page when invalid id", "[nextion][op_page]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_set_page(UINT8_MAX);

        TEST_ASSERT_EQUAL_INT(NEX_DVC_ERR_INVALID_PAGE, code);
    }

    TEST_CASE("Can refresh the page", "[nextion][op_comp]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_refresh_page();

        TEST_ASSERT_NEX_OK(code);
    }

#ifdef __cplusplus
}
#endif
