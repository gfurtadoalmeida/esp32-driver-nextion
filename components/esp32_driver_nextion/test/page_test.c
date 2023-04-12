#include "esp32_driver_nextion/page.h"
#include "common_infra_test.h"

TEST_CASE("Get current page", "[page]")
{
    nextion_page_set(handle, "1");

    uint8_t page_id;
    nex_err_t code = nextion_page_get(handle, &page_id);

    nextion_page_set(handle, "0");

    CHECK_NEX_OK(code);
    LONGS_EQUAL(1, page_id);
}

TEST_CASE("Change page", "[page]")
{
    nex_err_t code = nextion_page_set(handle, "1");

    nextion_page_set(handle, "0");

    CHECK_NEX_OK(code);
}

TEST_CASE("Cannot change to invalid page", "[page]")
{
    nex_err_t code = nextion_page_set(handle, "nonecziste");

    NEX_CODES_EQUAL(NEX_DVC_ERR_INVALID_PAGE, code);
}

TEST_CASE("Refresh page", "[page]")
{
    nex_err_t code = nextion_page_refresh(handle);

    CHECK_NEX_OK(code);
}