#include "common_infra_test.h"

#ifdef __cplusplus
extern "C"
{
#endif

// This is the only file that can install/delete a driver directly,
// as it is testing core methods.

#define RAW_INSTALL() nextion_driver_install(TEST_UART_NUM, TEST_UART_BAUD_RATE, TEST_UART_TX_PIN, TEST_UART_RX_PIN, 5, &p_event_queue)

    TEST_CASE("Can install driver", "[nextion][core]")
    {
        nex_err_t code = nextion_driver_install(TEST_UART_NUM, TEST_UART_BAUD_RATE, TEST_UART_TX_PIN, TEST_UART_RX_PIN, 5, &p_event_queue);

        TEST_ASSERT_NEX_OK(code);

        nextion_driver_delete();
    }

    TEST_CASE("Can delete driver when installed", "[nextion][core]")
    {
        RAW_INSTALL();

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
        RAW_INSTALL();

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
        RAW_INSTALL();

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
