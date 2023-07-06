#ifndef __ESP32_DRIVER_NEXTION_CONFIG_H__
#define __ESP32_DRIVER_NEXTION_CONFIG_H__

#include "sdkconfig.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef CONFIG_NEX_UART_MUTEX_WAIT_TIME_MS
/**
 * @brief Mutex acquire wait time(ms).
 */
#define CONFIG_NEX_UART_MUTEX_WAIT_TIME_MS 500
#endif

#ifndef CONFIG_NEX_UART_RECV_WAIT_TIME_MS
/**
 * @brief UART response wait time (ms).
 */
#define CONFIG_NEX_UART_RECV_WAIT_TIME_MS 100
#endif

#ifndef CONFIG_NEX_UART_TRANS_WAIT_TIME_MS
/**
 * @brief UART transmit wait time (ms).
 */
#define CONFIG_NEX_UART_TRANS_WAIT_TIME_MS 100
#endif

#ifndef CONFIG_NEX_UART_RECV_BUFFER_SIZE
/**
 * @brief UART receiver buffer size (bytes).
 */
#define CONFIG_NEX_UART_RECV_BUFFER_SIZE 256
#endif

#ifndef CONFIG_NEX_UART_TRANS_COMMAND_FORMAT_BUFFER_SIZE
/**
 * @brief UART command format buffer size (bytes).
 */
#define CONFIG_NEX_UART_TRANS_COMMAND_FORMAT_BUFFER_SIZE 256
#endif

#ifndef CONFIG_NEX_UART_TASK_PRIORITY
/**
 * @brief UART task priority.
 */
#define CONFIG_NEX_UART_TASK_PRIORITY 1
#endif

#ifdef __cplusplus
}
#endif
#endif


