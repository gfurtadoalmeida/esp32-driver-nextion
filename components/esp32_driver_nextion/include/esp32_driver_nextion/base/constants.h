#ifndef __ESP32_DRIVER_NEXTION_BASE_CONST_H__
#define __ESP32_DRIVER_NEXTION_BASE_CONST_H__

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Minimum baud rate accepted.
 */
#define NEX_SERIAL_BAUD_RATE_MIN 2400U

/**
 * @brief Maximum baud rate accepted.
 */
#define NEX_SERIAL_BAUD_RATE_MAX 921600U

/**
 * @brief Number of bytes used to identify a response type.
 */
#define NEX_DVC_CMD_START_LENGTH 1U

/**
 * @brief Number of bytes used to identify the end of a response.
 */
#define NEX_DVC_CMD_END_LENGTH 3U

/**
 * @brief Character used to identify the end of a response.
 * @details All commands end with {0xFF, 0xFF, 0xFF}, i.e: {255, 255, 255} or {ÿ, ÿ, ÿ}
 */
#define NEX_DVC_CMD_END_VALUE ((uint8_t)0xFFU)

/**
 * @brief Sequence that determines the end of a command and response.
 */
#define NEX_DVC_CMD_END_SEQUENCE NEX_DVC_CMD_END_VALUE, NEX_DVC_CMD_END_VALUE, NEX_DVC_CMD_END_VALUE

/**
 * @brief Minimum response size, in bytes.
 * @details NEX_DVC_CMD_START_LENGTH + NEX_DVC_CMD_END_LENGTH.
 */
#define NEX_DVC_CMD_ACK_LENGTH (NEX_DVC_CMD_START_LENGTH + NEX_DVC_CMD_END_LENGTH)

/**
 * @brief Maximum number of bytes a event can return.
 */
#define NEX_DVC_EVT_MAX_RESPONSE_LENGTH 9U

/**
 * @brief Maximum length of a page name.
 */
#define NEX_DVC_PAGE_MAX_NAME_LENGTH 14U

/**
 * @brief Maximum length of a component name.
 */
#define NEX_DVC_COMPONENT_MAX_NAME_LENGTH 14U

/**
 * @brief Maximum reference length: (max page name length + "." + max component name length).
 */
#define NEX_DVC_REFERENCE_MAX_LENGTH (NEX_DVC_PAGE_MAX_NAME_LENGTH + 1 + NEX_DVC_COMPONENT_MAX_NAME_LENGTH)

/**
 * @brief Character used to separate a object from its attribute.
 * @example page1.component2 / comp1.txt
 */
#define NEX_DVC_CMD_ATTRIBUTE_SEPARATOR '.'

/**
 * @brief Character used to separate the params from the command.
 * @example addt 1,0,320ÿÿÿ
 */
#define NEX_DVC_CMD_PARAMS_DIVISOR ' '

/**
 * @brief Character used to separate params from other params.
 * @example wept 1,0,320ÿÿÿ
 */
#define NEX_DVC_CMD_PARAMS_SEPARATOR ','

/**
 * @brief Maximum size, in bytes, that can be sent using the
 * "Transaprent Data" mode.
 * @details size = (all commands + terminations + data)
 */
#define NEX_DVC_TRANSPARENT_DATA_MAX_DATA_SIZE 1024U

/**
 * @brief EEPROM size in bytes.
 */
#define NEX_DVC_EEPROM_SIZE 1024U

/**
 * @brief EEPROM maximum address.
 */
#define NEX_DVC_EEPROM_MAX_ADDRESS (NEX_DVC_EEPROM_SIZE - 1U)

/**
 * @brief Time, in milliseconds, that a device needs
 * to enter the "Transparent Data" mode.
 */
#define NEX_DVC_TRANSPARENT_DATA_WAIT_TIME_MS 5U

/**
 * @brief Time, in milliseconds, that a device will
 * be unavailable after being reset.
 */
#define NEX_DVC_RESET_WAIT_TIME_MS 100

#ifdef __cplusplus
}
#endif
#endif