#ifndef __NEXTION_ESP32_CONFIG_H__
#define __NEXTION_ESP32_CONFIG_H__

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef CONFIG_NEX_UART_RECV_WAIT_TIME_MS
/**
 * UART response wait time (ms).
 */
#define CONFIG_NEX_UART_RECV_WAIT_TIME_MS 200
#endif

#ifndef CONFIG_NEX_UART_TRANS_WAIT_TIME_MS
/**
 * UART transmit wait time (ms).
 */
#define CONFIG_NEX_UART_TRANS_WAIT_TIME_MS 200
#endif

#ifndef CONFIG_NEX_UART_RECV_BUFFER_SIZE
/**
 * UART receiver buffer size.
 */
#define CONFIG_NEX_UART_RECV_BUFFER_SIZE 256
#endif

#ifdef __cplusplus
}
#endif

#endif //__NEXTION_ESP32_CONFIG_H__