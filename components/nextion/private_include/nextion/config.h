#ifndef __NEXTION_CONFIG_H__
#define __NEXTION_CONFIG_H__

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef CONFIG_NEX_EVENT_QUEUE_WAIT_TIME_MS
/**
 * Event queue, enqueue wait time (ms).
 */
#define CONFIG_NEX_EVENT_QUEUE_WAIT_TIME_MS 50
#endif

#ifndef CONFIG_NEX_RESP_WAIT_TIME_MS
/**
 * Device response wait time (ms).
 */
#define CONFIG_NEX_RESP_WAIT_TIME_MS 1000
#endif

#ifndef CONFIG_NEX_RESP_NO_BKCMD_WAIT_TIME_MS
/**
 * Device response wait time (ms), when responding only when failure.
 */
#define CONFIG_NEX_RESP_NO_BKCMD_WAIT_TIME_MS 50
#endif

#ifndef CONFIG_NEX_RESP_MSG_MAX_LENGTH
/**
 * Max response message size.
 */
#define CONFIG_NEX_RESP_MSG_MAX_LENGTH (NEX_DVC_CMD_ACK_LENGTH + 100) // 1 text response with 100 chars.
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

#endif //__NEXTION_CONFIG_H__