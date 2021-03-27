#include "unity.h"
#include "nextion/nextion.h"

#define TEST_UART_NUM UART_NUM_2
#define TEST_UART_BAUD_RATE 9600
#define TEST_UART_TX_PIN 17
#define TEST_UART_RX_PIN 16

static void print_banner(const char *text);

nextion_handle_t handle;

void app_main(void)
{
    /* This is just the test runner.
     * The real tests are on "components/nextion/test".
     */

    handle = nextion_driver_install(TEST_UART_NUM, TEST_UART_BAUD_RATE, TEST_UART_TX_PIN, TEST_UART_RX_PIN);

    if (handle == NULL)
    {
        printf("Failed to install driver");

        unity_run_menu();
    }

    if (nextion_init(handle) != NEX_OK)
    {
        printf("Failed to initialize driver");

        unity_run_menu();
    }

    UNITY_BEGIN();

    unity_run_all_tests();

    UNITY_END();

    print_banner("Starting interactive test menu");

    /* This function will not return, and will be busy waiting for UART input.
     * That's why this project compiles without CONFIG_TASK_WDT_CHECK_IDLE_TASK_CPU0.
     */
    unity_run_menu();
}

static void print_banner(const char *text)
{
    printf("\n#### %s #####\n\n", text);
}