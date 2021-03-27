#ifndef __NEXTION_CODES_H__
#define __NEXTION_CODES_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @typedef nex_err_t
     * @brief Response code.
     * @details Nextion return codes are unsigned byte.
     */
    typedef uint8_t nex_err_t;

/**
 * Success.
 */
#define NEX_OK 0x00U

/**
 * Failure.
 */
#define NEX_FAIL 0x01U

/**
 * Timeout.
 */
#define NEX_TIMEOUT 0x02U

/* ========================================
 *         CODE VERIFICATION HELPERS
 * ======================================== */

/**
 * Verify if a code means success.
 */
#define NEX_DVC_CODE_IS_SUCCESS(code) (code == NEX_DVC_INSTRUCTION_OK)

/**
 * Verify if a code means failure.
 */
#define NEX_DVC_CODE_IS_FAILURE(code) (code == NEX_DVC_INSTRUCTION_FAIL || (code >= NEX_DVC_ERR_INVALID_COMPONENT && code <= NEX_DVC_ERR_REFERENCE_NAME_TOO_LONG))

/**
 * Verify if a code represents an event.
 */
#define NEX_DVC_CODE_IS_EVENT(code, length) ((code >= NEX_DVC_EVT_TOUCH_OCCURRED && code != NEX_DVC_RSP_SENDME_RESULT && code != NEX_DVC_RSP_GET_STRING && code != NEX_DVC_RSP_GET_NUMBER && code != NEX_DVC_RSP_TRANSPARENT_DATA_READY && code != NEX_DVC_RSP_TRANSPARENT_DATA_FINISHED) || (code == NEX_DVC_EVT_HARDWARE_START_RESET && length == 6))

/**
 * Verify if a code represents a response.
 */
#define NEX_DVC_CODE_IS_RESPONSE(code, length) (!NEX_DVC_CODE_IS_EVENT(code, length))

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
 *   NEX_DVC_INSTRUCTION_ = Instruction returns success, or fail due to malformed command.
 *   NEX_DVC_ERR_         = Instruction was well-formed but the operation failed.
 *   NEX_DVC_EVT_         = Event sent by the device.
 *   NEX_DVC_RSP_         = Instruction executed and returned data.
 * ======================================== */

/**
 * Instruction failed.
 * Format: 0x00 0xFF 0xFF 0xFF
 */
#define NEX_DVC_INSTRUCTION_FAIL 0x00U

/**
 * Instruction succeeded.
 * Format: 0x01 0xFF 0xFF 0xFF
 */
#define NEX_DVC_INSTRUCTION_OK 0x01U

/**
 * Invalid component id or name.
 * Format: 0x02 0xFF 0xFF 0xFF
 */
#define NEX_DVC_ERR_INVALID_COMPONENT 0x02U

/**
 * Invalid page id or name.
 * Format: 0x03 0xFF 0xFF 0xFF
 */
#define NEX_DVC_ERR_INVALID_PAGE 0x03U

/**
 * Invalid picture id or name.
 * Format: 0x04 0xFF 0xFF 0xFF
 */
#define NEX_DVC_ERR_INVALID_PICTURE 0x04U

/**
 * Invalid font id or name.
 * Format: 0x05 0xFF 0xFF 0xFF
 */
#define NEX_DVC_ERR_INVALID_FONT 0x05U

/**
 * Invalid file.
 * Format: 0x06 0xFF 0xFF 0xFF
 * Device type: Intelligent
 */
#define NEX_DVC_ERR_INVALID_FILE 0x06U

/**
 * Instruction CRC validation failed.
 * Format: 0x09 0xFF 0xFF 0xFF
 */
#define NEX_DVC_ERR_INVALID_CRC 0x09U

/**
 * Invalid baud rate.
 * Format: 0x11 0xFF 0xFF 0xFF
 */
#define NEX_DVC_ERR_INVALID_BAUD_RATE 0x11U

/**
 * Invalid waveform id or channel.
 * Format: 0x12 0xFF 0xFF 0xFF
 */
#define NEX_DVC_ERR_INVALID_WAVEFORM 0x12U

/**
 * Invalid variable name or attribute.
 * Format: 0x1A 0xFF 0xFF 0xFF
 */
#define NEX_DVC_ERR_INVALID_VARIABLE_OR_ATTRIBUTE 0x1AU

/**
 * Invalid variable operation.
 * Format: 0x1B 0xFF 0xFF 0xFF
 */
#define NEX_DVC_ERR_INVALID_VARIABLE_OPERATION 0x1BU

/**
 * Invalid attribute assignment.
 * Format: 0x1C 0xFF 0xFF 0xFF
 */
#define NEX_DVC_ERR_INVALID_ATTRIBUTE_ASSIGNMENT 0x1CU

/**
 * EEPROM operation failed.
 * Format: 0x1D 0xFF 0xFF 0xFF
 * Device type: Enhanced or Intelligent
 */
#define NEX_DVC_ERR_EEPROM_OPERATION_FAILED 0x1DU

/**
 * Invalid number of parameters for a instruction.
 * Format: 0x1E 0xFF 0xFF 0xFF
 */
#define NEX_DVC_ERR_INVALID_INSTRUCTION_PARAMETERS_COUNT 0x1EU

/**
 * IO operation failed.
 * Format: 0x1F 0xFF 0xFF 0xFF
 */
#define NEX_DVC_ERR_IO_OPERATION_FAILED 0x1FU

/**
 * Invalid escape character.
 * Format: 0x20 0xFF 0xFF 0xFF
 */
#define NEX_DVC_ERR_INVALID_ESCAPE_CHARACTER 0x20U

/**
 * Reference name for a component is too long.
 * Format: 0x23 0xFF 0xFF 0xFF
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
 * Device started or reset.
 * Format: 0x00 0x00 0x00 0xFF 0xFF 0xFF
 */
#define NEX_DVC_EVT_HARDWARE_START_RESET 0x00U

/**
 * Touch occurred on a component.
 * Format: 0x65 {0x00} {0x02} {0x01} 0xFF 0xFF 0xFF
 * Composition:
 *   - {0x00}: page number
 *   - {0x02}: component id
 *   - {0x01}: event, where
 *     - 0x00: release
 *     - 0x01: press
 */
#define NEX_DVC_EVT_TOUCH_OCCURRED 0x65U

/**
 * "Sendme" command result.
 * Format: 0x66 {0x01} 0xFF 0xFF 0xFF
 * Composition:
 *   - {0x01}: page number
 */
#define NEX_DVC_RSP_SENDME_RESULT 0x66U

/**
 * Touch coordinate when the device is awake.
 * Format: 0x67 {0x00 0x7A} {0x00 0x1E} {0x01} 0xFF 0xFF 0xFF
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
 * Touch coordinate when the device was asleep (and now exited sleep mode).
 * Format: 0x68 {0x00 0x7A} {0x00 0x1E} {0x01} 0xFF 0xFF 0xFF
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
 * "get command" returning a string.
 * Format: 0x70 {0x61} {0x62} {0x31} {0x32} {0x33} 0xFF 0xFF 0xFF
 * Composition:
 *   - Each byte is a character
 */
#define NEX_DVC_RSP_GET_STRING 0x70U

/**
 * "get command" returning a number.
 * Format: 0x71 {0x01 0x02 0x03 0x04} 0xFF 0xFF 0xFF
 * Composition:
 *   - 4 byte 32-bit value in little endian order
 * Conversion:
 *   - (0x01 + 0x02 * 256 + 0x03 * 65536 + 0x04 * 16777216)
 */
#define NEX_DVC_RSP_GET_NUMBER 0x71U

/**
 * Device entered the sleep mode automatically.
 * Format: 0x86 0xFF 0xFF 0xFF
 */
#define NEX_DVC_EVT_HARDWARE_AUTO_SLEEP 0x86U

/**
 * Device left the sleep mode automatically.
 * Format: 0x87 0xFF 0xFF 0xFF
 */
#define NEX_DVC_EVT_HARDWARE_AUTO_WAKE 0x87U

/**
 * Device has powered up and initilized successfully.
 * Format: 0x88 0xFF 0xFF 0xFF
 */
#define NEX_DVC_EVT_HARDWARE_READY 0x88U

/**
 * A microSD was detected during power on and an update is in progress.
 * Format: 0x89 0xFF 0xFF 0xFF
 */
#define NEX_DVC_EVT_HARDWARE_UPGRADE 0x89U

/**
 * All requested bytes of a "Transparent Data" mode have been received.
 * The device is now leaving the "Transparent Data" mode.
 * Format: 0xFD 0xFF 0xFF 0xFF
 */
#define NEX_DVC_RSP_TRANSPARENT_DATA_FINISHED 0xFDU

/**
 * The device is able to receive a specified quantity of data in a "Transparent Data" mode.
 * Format: 0xFE 0xFF 0xFF 0xFF
 */
#define NEX_DVC_RSP_TRANSPARENT_DATA_READY 0xFEU

#ifdef __cplusplus
}
#endif

#endif //__NEXTION_CODES_H__