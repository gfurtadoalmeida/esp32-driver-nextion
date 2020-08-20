#ifndef __NEXTION_CONFIG__
#define __NEXTION_CONFIG__

#ifndef CONFIG_NEX_RESP_MSG_MAX_LENGTH
/**
 * Max response message size.
 */
#define CONFIG_NEX_RESP_MSG_MAX_LENGTH (NEX_DVC_CMD_ACK_LENGTH + 100) // 1 text response with 100 chars.
#endif

#ifndef CONFIG_NEX_UART_BUFFER_MULT
/**
 * UART buffer size multiplier.
 */
#define CONFIG_NEX_UART_BUFFER_MULT 5
#endif

#ifndef CONFIG_NEX_UART_QUEUE_SIZE
/**
 * UART queue size.
 */
#define CONFIG_NEX_UART_QUEUE_SIZE 10
#endif

#ifndef CONFIG_NEX_RESP_WAIT_TIME_MS
/**
 * Response wait time (ms).
 */
#define CONFIG_NEX_RESP_WAIT_TIME_MS 1000
#endif

#ifndef CONFIG_NEX_EVENT_QUEUE_WAIT_TIME_MS
/**
 * Event queue wait time (ms).
 */
#define CONFIG_NEX_EVENT_QUEUE_WAIT_TIME_MS 50
#endif

#endif //__NEXTION_C__