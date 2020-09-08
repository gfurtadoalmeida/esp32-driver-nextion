#ifndef __NEXTION_ESP32_CONFIG_H__
#define __NEXTION_ESP32_CONFIG_H__

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef CONFIG_NEX_RESP_WAIT_TIME_MS
/**
 * Device response wait time (ms).
 */
#define CONFIG_NEX_RESP_WAIT_TIME_MS 20
#endif

#ifndef CONFIG_NEX_UART_RECV_BUFFER_SIZE
/**
 * UART receiver buffer size.
 */
#define CONFIG_NEX_UART_RECV_BUFFER_SIZE 256
#endif

#ifndef CONFIG_NEX_UART_QUEUE_SIZE
/**
 * UART queue size.
 */
#define CONFIG_NEX_UART_QUEUE_SIZE 10
#endif

#ifdef __cplusplus
}
#endif

#endif //__NEXTION_ESP32_CONFIG_H__