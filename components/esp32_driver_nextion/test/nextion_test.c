#include "esp32_driver_nextion/nextion.h"
#include "common_infra_test.h"

TEST_CASE("Cannot init null context", "[core]")
{
    nex_err_t result = nextion_init(NULL);

    CHECK_NEX_FAIL(result);
}
