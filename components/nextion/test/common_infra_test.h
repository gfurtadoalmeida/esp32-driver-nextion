#ifndef __NEXTION_ESP32_COMMON_INFRA_TEST_H__
#define __NEXTION_ESP32_COMMON_INFRA_TEST_H__

#include "driver/uart.h"
#include "driver/gpio.h"
#include "freertos/queue.h"
#include "unity.h"
#include "config.h"
#include "nextion/nextion-esp32.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define TEST_UART_NUM UART_NUM_2
#define TEST_UART_BAUD_RATE 9600
#define TEST_UART_TX_PIN GPIO_NUM_17
#define TEST_UART_RX_PIN GPIO_NUM_16

#ifdef __cplusplus
}
#endif

#endif //__NEXTION_ESP32_COMMON_INFRA_TEST_H__