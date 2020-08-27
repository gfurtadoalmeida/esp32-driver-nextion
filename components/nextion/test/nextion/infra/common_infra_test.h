#ifndef __COMMON_INFRA_TEST_H__
#define __COMMON_INFRA_TEST_H__

#include "driver/uart.h"
#include "driver/gpio.h"
#include "freertos/queue.h"
#include "unity.h"
#include "nextion/config.h"
#include "nextion/nextion.h"

#ifdef __cplusplus
extern "C"
{
#endif

    static QueueHandle_t p_event_queue;

#define TEST_ASSERT_NEX_OK(code) TEST_ASSERT_EQUAL_INT(NEX_OK, code)
#define TEST_ASSERT_NEX_FAIL(code) TEST_ASSERT_EQUAL_INT(NEX_FAIL, code)

#define TEST_UART_NUM UART_NUM_2
#define TEST_UART_BAUD_RATE 9600
#define TEST_UART_TX_PIN GPIO_NUM_17
#define TEST_UART_RX_PIN GPIO_NUM_16

/**
 * @brief Installs the driver.
 * @details Once the driver is installed, it will remaing installed forever.
 * @note Always revert the changes made (when possible), to not affect other tests.
 */
#define DRIVER_INSTALL()                                                                                                   \
    if (!nextion_is_driver_installed())                                                                                    \
    {                                                                                                                      \
        nextion_driver_install(TEST_UART_NUM, TEST_UART_BAUD_RATE, TEST_UART_TX_PIN, TEST_UART_RX_PIN, 5, &p_event_queue); \
        nextion_driver_init();                                                                                             \
    }

#define DRIVER_QUEUE_RESET() xQueueReset(p_event_queue)
#define DRIVER_QUEUE_GET_ITEM(pointer) xQueueReceive(p_event_queue, pointer, pdMS_TO_TICKS(CONFIG_NEX_EVENT_QUEUE_WAIT_TIME_MS))

#ifdef __cplusplus
}
#endif

#endif //__COMMON_INFRA_TEST_H__