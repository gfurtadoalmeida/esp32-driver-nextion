#ifndef __NEXTION_CONSTANTS_H__
#define __NEXTION_CONSTANTS_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define NEX_SERIAL_BAUD_RATE_MIN 9600
#define NEX_SERIAL_BAUD_RATE_MAX 921600

/**
 * Number of bytes used to identify a response type.
 */
#define NEX_DVC_CMD_START_LENGTH 1

/**
 * Number of bytes used to identify the end of a response.
 */
#define NEX_DVC_CMD_END_LENGTH 3

/**
 * Character used to identify the end of a response.
 * All commands end with {0xFF, 0xFF, 0xFF}
 * i.e: {255, 255, 255} or {ÿ, ÿ, ÿ}
 */
#define NEX_DVC_CMD_END_VALUE ((uint8_t)0xFF)

/**
 * Sequence that determines the end of a command and response.
 */
#define NEX_DVC_CMD_END_SEQUENCE NEX_DVC_CMD_END_VALUE, NEX_DVC_CMD_END_VALUE, NEX_DVC_CMD_END_VALUE

/**
 * The minimum response size, in bytes.
 * i.e: NEX_DVC_CMD_START_LENGTH + NEX_DVC_CMD_END_LENGTH.
 */
#define NEX_DVC_CMD_ACK_LENGTH (NEX_DVC_CMD_START_LENGTH + NEX_DVC_CMD_END_LENGTH)

/**
 * The maximum number of bytes a event can return.
 */
#define NEX_DVC_EVT_MAX_RESPONSE_LENGTH 9

/**
 * The maximum length of a page name.
 */
#define NEX_DVC_PAGE_MAX_NAME_LENGTH 14

/**
 * The maximum length of a component name.
 */
#define NEX_DVC_COMPONENT_MAX_NAME_LENGTH 14

/**
 * Max reference length: (max page name length + "." + max component name length).
 */
#define NEX_DVC_REFERENCE_MAX_LENGTH (NEX_DVC_PAGE_MAX_NAME_LENGTH + 1 + NEX_DVC_COMPONENT_MAX_NAME_LENGTH)

/**
 * Character used to separate the page name from the component name.
 * e.g.: page1.component2
 */
#define NEX_DVC_CMD_REFERENCE_SEPARATOR '.'

/**
 * Character used to separate the params from the command.
 * e.g.: addt 1,0,320ÿÿÿ
 */
#define NEX_DVC_CMD_PARAMS_SEPARATOR ' '

/**
 * The max size, in bytes, that can be sent using the
 * "Transaprent Data" mode.
 * Size: (all commands + terminations + data).
 */
#define NEX_DVC_TRANSPARENT_DATA_MAX_DATA_SIZE 1023

/**
 * Time, in milliseconds, that a device needs
 * to enter the "Transparent Data" mode.
 */
#define NEX_DVC_TRANSPARENT_DATA_WAIT_TIME_MS 5

#ifdef __cplusplus
}
#endif

#endif //__NEXTION_CONSTANTS_H__