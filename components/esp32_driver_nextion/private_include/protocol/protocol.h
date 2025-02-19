#ifndef __ESP32_DRIVER_NEXTION_PROTO_H__
#define __ESP32_DRIVER_NEXTION_PROTO_H__

#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "esp32_driver_nextion/base/codes.h"
#include "esp32_driver_nextion/base/types.h"
#include "protocol/parsers/parser.h"

#ifdef __cplusplus
extern "C"
{
#endif

    static char FORMAT_BUFFER[255];

    /**
     * @typedef formated_instruction_t
     * @brief A formated instruction ready to be sent.
     */
    typedef struct
    {
        const char *text; /** @brief Null-terminated text. */
        size_t length;    /** @brief Text length. */
    } formated_instruction_t;

    /**
     * @brief Create a formated instruction.
     */
#define FORMAT_INSTRUNCTION(instruction, ...) \
    {                                         \
        .text = FORMAT_BUFFER,                \
        .length = snprintf(FORMAT_BUFFER, sizeof(FORMAT_BUFFER), instruction, __VA_ARGS__)}

    /**
     * @brief Send a instruction to the device.
     * @param[in] handle Nextion context pointer.
     * @param[in] instruction A null-terminated string with the instruction to be sent.
     * @param[in] instruction_length Instruction length.
     * @param[in] parser Parser responsible for parsing the response. Can be NULL if not needed.
     * @return NEX_OK or NEX_FAIL | NEX_DVC_ERR_*
     */
    nex_err_t nextion_protocol_send_instruction(nextion_t *handle,
                                                const char *instruction,
                                                size_t instruction_length,
                                                const parser_t *parser);

    /**
     * @brief Send a raw byte to the device.
     * @param[in] handle Nextion context pointer.
     * @param[in] value Byte to be sent.
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_protocol_send_raw_byte(const nextion_t *handle, uint8_t value);

#ifdef __cplusplus
}
#endif
#endif