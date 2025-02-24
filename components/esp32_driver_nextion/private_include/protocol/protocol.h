#ifndef __ESP32_DRIVER_NEXTION_PROTO_H__
#define __ESP32_DRIVER_NEXTION_PROTO_H__

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
     * @brief Format an instruction.
     * @param[in] handle Nextion context pointer.
     * @param[in] formated_instruction A null-terminated string with the instruction to be sent.
     * @param[in] instruction A null-terminated string to format.
     * @param[in] ... Format parameters.
     * @return NEX_OK or NEX_FAIL
     */
    bool nextion_protocol_format_instruction(nextion_t *handle,
                                             formated_instruction_t *formated_instruction,
                                             const char *instruction,
                                             ...);

    /**
     * @brief Format an instruction using a va_list.
     * @param[in] handle Nextion context pointer.
     * @param[in] formated_instruction A null-terminated string with the instruction to be sent.
     * @param[in] instruction A null-terminated string to format.
     * @param[in] args Format parameters.
     * @return NEX_OK or NEX_FAIL
     */
    bool nextion_protocol_format_instruction_variadic(nextion_t *handle,
                                                      formated_instruction_t *formated_instruction,
                                                      const char *instruction,
                                                      va_list args);
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
     * @brief Send an ACK instruction to the device, formating it before sending.
     * @remark An ACK instruction returns only success or failure.
     * @param[in] handle Nextion context pointer.
     * @param[in] instruction A null-terminated string to format.
     * @param[in] ... Format parameters.
     * @return NEX_OK or NEX_FAIL | NEX_DVC_ERR_*
     */
    nex_err_t nextion_protocol_send_instruction_ack(nextion_t *handle, const char *instruction, ...);

    /**
     * @brief Send an GET_TEXT instruction to the device, formating it before sending.
     * @remark An GET_TEXT instruction returns text or failure.
     * @param[in] handle Nextion context pointer.
     * @param[in] buffer Buffer to write the parsed text into.
     * @param[in] buffer_length Buffer length.
     * @param[in] instruction A null-terminated string to format.
     * @param[in] ... Format parameters.
     * @return NEX_OK or NEX_FAIL | NEX_DVC_ERR_*
     */
    nex_err_t nextion_protocol_send_instruction_get_text(nextion_t *handle,
                                                         char *buffer,
                                                         size_t buffer_length,
                                                         const char *instruction,
                                                         ...);

    /**
     * @brief Send an GET_NUMBER instruction to the device, formating it before sending.
     * @remark An GET_NUMBER instruction returns a number or failure.
     * @param[in] handle Nextion context pointer.
     * @param[in] number Buffer to write the parsed number into.
     * @param[in] instruction A null-terminated string to format.
     * @param[in] ... Format parameters.
     * @return NEX_OK or NEX_FAIL | NEX_DVC_ERR_*
     */
    nex_err_t nextion_protocol_send_instruction_get_number(nextion_t *handle,
                                                           int32_t *number,
                                                           const char *instruction,
                                                           ...);

    /**
     * @brief Send an TDM_START instruction to the device, formating it before sending.
     * @remark An TDM_START instruction starts the Transparent Data Mode.
     * @param[in] handle Nextion context pointer.
     * @param[in] instruction A null-terminated string to format.
     * @param[in] ... Format parameters.
     * @return NEX_OK or NEX_FAIL | NEX_DVC_ERR_*
     */
    nex_err_t nextion_protocol_send_instruction_tdm_start(nextion_t *handle,
                                                          const char *instruction,
                                                          ...);

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