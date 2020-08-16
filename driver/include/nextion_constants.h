#ifndef __NEXTION_CONSTANTS_H__
#define __NEXTION_CONSTANTS_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define NEX_SERIAL_BAUD_RATE_MIN 9600
#define NEX_SERIAL_BAUD_RATE_MAX 921600

/**
 * Character used to identify the end of a result.
 * All commands end with {0xFF, 0xFF, 0xFF}.
 * i.e: {255, 255, 255} or {ÿ, ÿ, ÿ}
 */
#define NEX_DVC_SERIAL_END_VALUE ((uint8_t)0xFF)

/**
 * Sequence that determines the end of a command and response.
 */
#define NEX_DVC_SERIAL_END_SEQUENCE NEX_DVC_SERIAL_END_VALUE, NEX_DVC_SERIAL_END_VALUE, NEX_DVC_SERIAL_END_VALUE

/**
 * Number of bytes used to identify a response type.
 */
#define NEX_DVC_SERIAL_START_LENGTH 1

/**
 * Number of bytes used to identify the end of a response type.
 */
#define NEX_DVC_SERIAL_END_LENGTH 3

/**
 * The minimum response size, in bytes.
 * i.e: NEX_DVC_SERIAL_START_LENGTH + NEX_DVC_SERIAL_END_LENGTH.
 */
#define NEX_DVC_SERIAL_ACK_LENGTH (NEX_DVC_SERIAL_START_LENGTH + NEX_DVC_SERIAL_END_LENGTH)

/**
 * The maximum number of bytes a event can return.
 */
#define NEX_DVC_MAX_EVT_RESPONSE_LENGTH 9

/**
 * The maximum length of a page name.
 */
#define NEX_DVC_MAX_PAGE_NAME_LENGTH 14

/**
 * The maximum length of a component name.
 */
#define NEX_DVC_MAX_COMPONENT_NAME_LENGTH 14

/**
 * Max reference length: (max page name length + "." + max component name length).
 */
#define NEX_DVC_MAX_REFERENCE_NAME_LENGTH (NEX_DVC_MAX_PAGE_NAME_LENGTH + 1 + NEX_DVC_MAX_PAGE_NAME_LENGTH)

/**
 * Character used to separate the page name from the component name.
 * e.g.: page1.component2
 */
#define NEX_DVC_REFERENCE_SEPARATOR '.'

/**
 * Character used to separate the params from the command.
 * e.g.: addt 1,0,320ÿÿÿ
 */
#define NEX_DVC_PARAMS_SEPARATOR ' '

/**
 * Data quantity limited by serial buffer.
 * Size: (all commands + terminations + data).
 */
#define NEX_DVC_MAX_DATA_BUFFER_SIZE 1024

/**
 * Time, in milliseconds, that a device needs
 * to enter the "Transparent Data" mode.
 */
#define NEX_DVC_TRANSPARENT_DATA_MODE_WAIT_TIME_MS 5

#ifdef __cplusplus
}
#endif

#endif //__NEXTION_CONSTANTS_H__