#ifndef __ESP32_DRIVER_NEXTION_BASE_CODES_H__
#define __ESP32_DRIVER_NEXTION_BASE_CODES_H__

#include <stdint.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @typedef nex_err_t
     * @brief Response code.
     */
    typedef esp_err_t nex_err_t;

/**
 * @brief Success.
 */
#define NEX_OK ESP_OK

/**
 * @brief Failure.
 */
#define NEX_FAIL ESP_FAIL

/**
 * @brief Timeout.
 */
#define NEX_TIMEOUT 0x40

/* ========================================
 *         CODE VERIFICATION HELPERS
 * ======================================== */

/**
 * @brief Verify if a code represents a response.
 */
#define NEX_DVC_CODE_IS_ACK_RESPONSE(code) (code <= NEX_DVC_ERR_REFERENCE_NAME_TOO_LONG)

/* ========================================
 *               ERROR CODES
 *
 * All values below must have the same value as defined
 * in https://nextion.tech/instruction-set/#s7
 *
 * Keep the values in ascending order, it's easier to
 * know the ranges.
 *
 * Prefix meanings:
 *   NEX_DVC_INS_ = Instruction returns success, or fail due to malformed command.
 *   NEX_DVC_ERR_ = Instruction was well-formed but the operation failed.
 *   NEX_DVC_EVT_ = Event sent by the device.
 *   NEX_DVC_RSP_ = Instruction executed and returned data.
 * ======================================== */

/**
 * @brief Instruction failed.
 * @details Format: 0x00 0xFF 0xFF 0xFF
 */
#define NEX_DVC_INS_FAIL 0x00U

/**
 * @brief Instruction succeeded.
 * @details Format: 0x01 0xFF 0xFF 0xFF
 */
#define NEX_DVC_INS_OK 0x01U

/**
 * @brief Invalid component id or name.
 * @details Format: 0x02 0xFF 0xFF 0xFF
 */
#define NEX_DVC_ERR_INVALID_COMPONENT 0x02U

/**
 * @brief Invalid page id or name.
 * @details Format: 0x03 0xFF 0xFF 0xFF
 */
#define NEX_DVC_ERR_INVALID_PAGE 0x03U

/**
 * @brief Invalid picture id or name.
 * @details Format: 0x04 0xFF 0xFF 0xFF
 */
#define NEX_DVC_ERR_INVALID_PICTURE 0x04U

/**
 * @brief Invalid font id or name.
 * @details Format: 0x05 0xFF 0xFF 0xFF
 */
#define NEX_DVC_ERR_INVALID_FONT 0x05U

/**
 * @brief Invalid file.
 * @details Format: 0x06 0xFF 0xFF 0xFF
 * @note Device type: Intelligent
 */
#define NEX_DVC_ERR_INVALID_FILE 0x06U

/**
 * @brief Instruction CRC validation failed.
 * @details Format: 0x09 0xFF 0xFF 0xFF
 */
#define NEX_DVC_ERR_INVALID_CRC 0x09U

/**
 * @brief Invalid baud rate.
 * @details Format: 0x11 0xFF 0xFF 0xFF
 */
#define NEX_DVC_ERR_INVALID_BAUD_RATE 0x11U

/**
 * @brief Invalid waveform id or channel.
 * @details Format: 0x12 0xFF 0xFF 0xFF
 */
#define NEX_DVC_ERR_INVALID_WAVEFORM 0x12U

/**
 * @brief Invalid variable name or attribute.
 * @details Format: 0x1A 0xFF 0xFF 0xFF
 */
#define NEX_DVC_ERR_INVALID_VARIABLE_OR_ATTRIBUTE 0x1AU

/**
 * @brief Invalid variable operation.
 * @details Format: 0x1B 0xFF 0xFF 0xFF
 */
#define NEX_DVC_ERR_INVALID_VARIABLE_OPERATION 0x1BU

/**
 * @brief Invalid attribute assignment.
 * @details Format: 0x1C 0xFF 0xFF 0xFF
 */
#define NEX_DVC_ERR_INVALID_ATTRIBUTE_ASSIGNMENT 0x1CU

/**
 * @brief EEPROM operation failed.
 * @details Format: 0x1D 0xFF 0xFF 0xFF
 * @note Device type: Enhanced or Intelligent
 */
#define NEX_DVC_ERR_EEPROM_OPERATION_FAILED 0x1DU

/**
 * @brief Invalid number of parameters for a instruction.
 * @details Format: 0x1E 0xFF 0xFF 0xFF
 */
#define NEX_DVC_ERR_INVALID_INSTRUCTION_PARAMETERS_COUNT 0x1EU

/**
 * @brief IO operation failed.
 * @details Format: 0x1F 0xFF 0xFF 0xFF
 */
#define NEX_DVC_ERR_IO_OPERATION_FAILED 0x1FU

/**
 * @brief Invalid escape character.
 * @details Format: 0x20 0xFF 0xFF 0xFF
 */
#define NEX_DVC_ERR_INVALID_ESCAPE_CHARACTER 0x20U

/**
 * @brief Reference name for a component is too long.
 * @details Format: 0x23 0xFF 0xFF 0xFF
 * Max length: 29
 * Composition:
 *   - Page: 14
 *   - Separator: 1 = "."
 *   - Component: 14
 */
#define NEX_DVC_ERR_REFERENCE_NAME_TOO_LONG 0x23U

/* ========================================
 *         EVENT AND RESPONSE CODES
 * ========================================*/

/**
 * @brief Device started or reset.
 * @details Format: 0x00 0x00 0x00 0xFF 0xFF 0xFF
 */
#define NEX_DVC_EVT_HARDWARE_START_RESET 0x00U

/**
 * @brief Touch occurred on a component.
 * @details Format: 0x65 {0x00} {0x02} {0x01} 0xFF 0xFF 0xFF
 * Composition:
 *   - {0x00}: page number
 *   - {0x02}: component id
 *   - {0x01}: event, where
 *     - 0x00: release
 *     - 0x01: press
 */
#define NEX_DVC_EVT_TOUCH_OCCURRED 0x65U

/**
 * @brief "Sendme" command result.
 * @remark Can act as an event when executing the
 * "Sendme" instruction on the display side.
 * @details Format: 0x66 {0x01} 0xFF 0xFF 0xFF
 * Composition:
 *   - {0x01}: page number
 */
#define NEX_DVC_RSP_SENDME 0x66U

/**
 * @brief Touch coordinate when the device is awake.
 * @details Format: 0x67 {0x00 0x7A} {0x00 0x1E} {0x01} 0xFF 0xFF 0xFF
 * Composition:
 *   - {0x00 0x7A}: X coordinate in big endian order
 *   - {0x00 0x1E}: Y coordinate in big endian order
 *   - {0x01}: event, where:
 *     - 0x00: release
 *     - 0x01: press
 * Conversion:
 *   - X: 0x00 * 256 + 0x7A
 *   - Y: 0x00 * 256 + 0x1E
 */
#define NEX_DVC_EVT_TOUCH_COORDINATE_AWAKE 0x67U

/**
 * @brief Touch coordinate when the device was asleep (and now exited sleep mode).
 * @details Format: 0x68 {0x00 0x7A} {0x00 0x1E} {0x01} 0xFF 0xFF 0xFF
 * Composition:
 *   - {0x00 0x7A}: X coordinate in big endian order
 *   - {0x00 0x1E}: Y coordinate in big endian order
 *   - {0x01}: event, where:
 *     - 0x00: release
 *     - 0x01: press
 * Conversion:
 *   - X: 0x00 * 256 + 0x7A
 *   - Y: 0x00 * 256 + 0x1E
 */
#define NEX_DVC_EVT_TOUCH_COORDINATE_ASLEEP 0x68U

/**
 * @brief "get command" returning a text.
 * @details Format: 0x70 {0x61} {0x62} {0x31} {0x32} {0x33} 0xFF 0xFF 0xFF
 * Composition:
 *   - Each byte is a character
 */
#define NEX_DVC_RSP_GET_TEXT 0x70U

/**
 * @brief "get command" returning a number.
 * @details Format: 0x71 {0x01 0x02 0x03 0x04} 0xFF 0xFF 0xFF
 * Composition:
 *   - 4 byte 32-bit value in little endian order
 * Conversion:
 *   - (0x01 + 0x02 * 256 + 0x03 * 65536 + 0x04 * 16777216)
 */
#define NEX_DVC_RSP_GET_NUMBER 0x71U

/**
 * @brief Device entered the sleep mode automatically.
 * @details Format: 0x86 0xFF 0xFF 0xFF
 */
#define NEX_DVC_EVT_HARDWARE_AUTO_SLEEP 0x86U

/**
 * @brief Device left the sleep mode automatically.
 * @details Format: 0x87 0xFF 0xFF 0xFF
 */
#define NEX_DVC_EVT_HARDWARE_AUTO_WAKE 0x87U

/**
 * @brief Device has powered up and initilized successfully.
 * @details Format: 0x88 0xFF 0xFF 0xFF
 */
#define NEX_DVC_EVT_HARDWARE_READY 0x88U

/**
 * @brief A microSD was detected during power on and an update is in progress.
 * @details Format: 0x89 0xFF 0xFF 0xFF
 */
#define NEX_DVC_EVT_HARDWARE_UPGRADE 0x89U

/**
 * @brief All requested bytes of a "Transparent Data" mode have been received.
 * @details Format: 0xFD 0xFF 0xFF 0xFF
 * @note The device is now leaving the "Transparent Data" mode.
 */
#define NEX_DVC_EVT_TRANSPARENT_DATA_FINISHED 0xFDU

/**
 * @brief The device is able to receive a specified quantity of data in a "Transparent Data" mode.
 * @details Format: 0xFE 0xFF 0xFF 0xFF
 */
#define NEX_DVC_RSP_TRANSPARENT_DATA_READY 0xFEU

#ifdef __cplusplus
}
#endif
#endif