#include <stdarg.h>
#include "esp32_driver_nextion/base/codes.h"
#include "protocol/parsers/responses/ack.h"
#include "protocol/parsers/responses/number.h"
#include "protocol/parsers/responses/text.h"
#include "protocol/parsers/responses/tdm_start.h"
#include "protocol/protocol.h"

nex_err_t nextion_protocol_send_instruction_ack(nextion_t *handle, const char *instruction, ...)
{
    va_list args;
    va_start(args, instruction);

    formated_instruction_t formated_instruction;

    if (!nextion_protocol_format_instruction_variadic(handle, &formated_instruction, instruction, args))
    {
        return NEX_FAIL;
    }

    va_end(args);

    parser_t parser = PARSER_ACK();

    return nextion_protocol_send_instruction(handle, formated_instruction.text, formated_instruction.length, &parser);
}

nex_err_t nextion_protocol_send_instruction_get_text(nextion_t *handle,
                                                     char *buffer,
                                                     size_t buffer_length,
                                                     const char *instruction,
                                                     ...)
{
    va_list args;
    va_start(args, instruction);

    formated_instruction_t formated_instruction;

    if (!nextion_protocol_format_instruction_variadic(handle, &formated_instruction, instruction, args))
    {
        return NEX_FAIL;
    }

    va_end(args);

    parser_t parser = PARSER_TEXT(buffer, buffer_length);

    nex_err_t code = nextion_protocol_send_instruction(handle, formated_instruction.text, formated_instruction.length, &parser);

    if (code == NEX_DVC_RSP_GET_TEXT)
    {
        return NEX_OK;
    }

    return code;
}

nex_err_t nextion_protocol_send_instruction_get_number(nextion_t *handle,
                                                       int32_t *number,
                                                       const char *instruction,
                                                       ...)
{
    va_list args;
    va_start(args, instruction);

    formated_instruction_t formated_instruction;

    if (!nextion_protocol_format_instruction_variadic(handle, &formated_instruction, instruction, args))
    {
        return NEX_FAIL;
    }

    va_end(args);

    parser_t parser = PARSER_NUMBER(number, sizeof(int32_t));

    nex_err_t code = nextion_protocol_send_instruction(handle, formated_instruction.text, formated_instruction.length, &parser);

    if (code == NEX_DVC_RSP_GET_NUMBER)
    {
        return NEX_OK;
    }

    return code;
}

nex_err_t nextion_protocol_send_instruction_tdm_start(nextion_t *handle, const char *instruction, ...)
{
    va_list args;
    va_start(args, instruction);

    formated_instruction_t formated_instruction;

    if (!nextion_protocol_format_instruction_variadic(handle, &formated_instruction, instruction, args))
    {
        return NEX_FAIL;
    }

    va_end(args);

    parser_t parser = PARSER_TDM_START();

    nex_err_t code = nextion_protocol_send_instruction(handle, formated_instruction.text, formated_instruction.length, &parser);

    if (code == NEX_DVC_RSP_TRANSPARENT_DATA_READY)
    {
        return NEX_OK;
    }

    return code;
}