#include "common_infra_test.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define DRIVER_INSTALL() nextion_driver_install(UART_NUM_2, 9600, GPIO_NUM_17, GPIO_NUM_16, 5, NULL)

    TEST_CASE("Can install driver", "[nextion][core]")
    {
        nex_err_t code = nextion_driver_install(UART_NUM_2, 9600, GPIO_NUM_17, GPIO_NUM_16, 5, NULL);

        TEST_ASSERT_NEX_OK(code);

        nextion_driver_delete();
    }

    TEST_CASE("Can delete driver when installed", "[nextion][core]")
    {
        DRIVER_INSTALL();

        nex_err_t code = nextion_driver_delete();

        TEST_ASSERT_NEX_OK(code);
    }

    TEST_CASE("Can delete driver when not installed", "[nextion][core]")
    {
        nex_err_t code = nextion_driver_delete();

        TEST_ASSERT_NEX_OK(code);
    }

    TEST_CASE("Can assert driver is installed when installed", "[nextion][core]")
    {
        DRIVER_INSTALL();

        bool installed = nextion_is_driver_installed();

        TEST_ASSERT_TRUE(installed);

        nextion_driver_delete();
    }

    TEST_CASE("Can assert driver is not installed when not installed", "[nextion][core]")
    {
        bool installed = nextion_is_driver_installed();

        TEST_ASSERT_FALSE(installed);
    }

    TEST_CASE("Can init driver when installed", "[nextion][core]")
    {
        DRIVER_INSTALL();

        nex_err_t code = nextion_driver_init();

        TEST_ASSERT_NEX_OK(code);

        nextion_driver_delete();
    }

    TEST_CASE("Cannot init driver  when not installed", "[nextion][core]")
    {
        nex_err_t code = nextion_driver_init();

        TEST_ASSERT_NEX_FAIL(code);
    }

#ifdef __cplusplus
}
#endif
