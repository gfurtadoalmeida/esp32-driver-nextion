#include "common_infra_test.h"
#include "nextion/page.h"

#ifdef __cplusplus
extern "C"
{
#endif

    TEST_CASE("Install driver", "[nextion][core]")
    {
        nextion_handle_t handle = nextion_driver_install(TEST_UART_NUM, TEST_UART_BAUD_RATE, TEST_UART_TX_PIN, TEST_UART_RX_PIN);

        TEST_ASSERT_NOT_NULL(handle);

        nextion_driver_delete(handle);
    }

    TEST_CASE("Delete driver", "[nextion][core]")
    {
        nextion_handle_t handle = nextion_driver_install(TEST_UART_NUM, TEST_UART_BAUD_RATE, TEST_UART_TX_PIN, TEST_UART_RX_PIN);

        bool result = nextion_driver_delete(handle);

        TEST_ASSERT_TRUE(result);
    }

    TEST_CASE("Cannot delete null driver", "[nextion][core]")
    {
        bool result = nextion_driver_delete(NULL);

        TEST_ASSERT_FALSE(result);
    }

    TEST_CASE("Send command", "[nextion][core]")
    {
        nextion_handle_t handle = nextion_driver_install(TEST_UART_NUM, TEST_UART_BAUD_RATE, TEST_UART_TX_PIN, TEST_UART_RX_PIN);

        if (nextion_init(handle) != NEX_OK)
        {
            TEST_FAIL_MESSAGE("Could not init the driver.");
        }

        uint8_t page_id = 255;
        nex_err_t code = nextion_page_get(handle, &page_id);

        nextion_driver_delete(handle);

        TEST_ASSERT_EQUAL(NEX_OK, code);
        TEST_ASSERT_EQUAL(0, page_id);
    }

#ifdef __cplusplus
}
#endif
