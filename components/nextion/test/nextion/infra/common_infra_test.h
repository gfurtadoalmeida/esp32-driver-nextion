#ifndef __COMMON_INFRA_TEST_H__
#define __COMMON_INFRA_TEST_H__

#include "driver/uart.h"
#include "driver/gpio.h"
#include "freertos/queue.h"
#include "unity.h"
#include "unity_test_runner.h"
#include "nextion/codes.h"
#include "nextion/nextion.h"

#ifdef __cplusplus
extern "C"
{
#endif

    static QueueHandle_t p_event_queue;

#define TEST_ASSERT_NEX_OK(code) TEST_ASSERT_EQUAL_INT(NEX_OK, code)
#define TEST_ASSERT_NEX_FAIL(code) TEST_ASSERT_EQUAL_INT(NEX_FAIL, code)

#define INSTALL()                                                                          \
    nextion_driver_install(UART_NUM_2, 9600, GPIO_NUM_17, GPIO_NUM_16, 5, &p_event_queue); \
    nextion_driver_init();

#define DELETE() nextion_driver_delete();

#ifdef __cplusplus
}
#endif

#endif //__COMMON_INFRA_TEST_H__