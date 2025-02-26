#include "esp32_driver_nextion/base/constants.h"
#include "esp32_driver_nextion/eeprom.h"
#include "protocol/parsers/responses/number.h"
#include "protocol/parsers/responses/rept.h"
#include "protocol/protocol.h"
#include "assertion.h"

#define CMP_CHECK_EEPROM_ADDRESS(address) CMP_CHECK((address < NEX_DVC_EEPROM_SIZE), "address error(address > NEX_DVC_EEPROM_MAX_ADDRESS)", NEX_FAIL)
#define CMP_CHECK_EEPROM_END_ADDRESS(address) CMP_CHECK(((address) < NEX_DVC_EEPROM_SIZE), "address error(end address > NEX_DVC_EEPROM_MAX_ADDRESS)", NEX_FAIL)

nex_err_t nextion_eeprom_write_text(nextion_t *handle,
                                    uint16_t address,
                                    const char *text,
                                    size_t text_length)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK_EEPROM_ADDRESS(address)
    CMP_CHECK_EEPROM_END_ADDRESS(address + text_length)
    CMP_CHECK((text != NULL), "text error(NULL)", NEX_FAIL)

    return nextion_protocol_send_instruction_ack(handle, "wepo \"%s\",%d", text, address);
}

nex_err_t nextion_eeprom_write_number(nextion_t *handle,
                                      uint16_t address,
                                      int32_t value)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK_EEPROM_ADDRESS(address)
    CMP_CHECK_EEPROM_END_ADDRESS(address + 4)

    return nextion_protocol_send_instruction_ack(handle, "wepo %ld,%d", value, address);
}

nex_err_t nextion_eeprom_read_text(nextion_t *handle,
                                   uint16_t address,
                                   char *text,
                                   size_t text_length)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK_EEPROM_ADDRESS(address)
    CMP_CHECK_EEPROM_END_ADDRESS(address + text_length)

    text[text_length] = '\0';

    return nextion_eeprom_read_bytes(handle, address, (uint8_t *)text, text_length);
}

nex_err_t nextion_eeprom_read_number(nextion_t *handle,
                                     uint16_t address,
                                     int32_t *number)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK_EEPROM_ADDRESS(address)
    CMP_CHECK_EEPROM_END_ADDRESS(address + 4)

    uint8_t buffer[4];

    if (nextion_eeprom_read_bytes(handle, address, buffer, 4) != NEX_OK)
    {
        return NEX_FAIL;
    }

    *number = parser_rsp_number_convert(buffer);

    return NEX_OK;
}

nex_err_t nextion_eeprom_read_bytes(nextion_t *handle,
                                    uint16_t address,
                                    uint8_t *buffer,
                                    size_t buffer_length)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK_EEPROM_ADDRESS(address)
    CMP_CHECK_EEPROM_END_ADDRESS(address + buffer_length)

    if (buffer_length == 0)
    {
        return NEX_OK;
    }

    // It will always read exactly what it is asked,
    // adding zeros if no more data is available.
    // No return code sent.
    formated_instruction_t instruction;

    if (!nextion_protocol_format_instruction(handle, &instruction, "rept %d,%d", address, buffer_length))
    {
        return NEX_FAIL;
    }

    parser_t parser = PARSER_REPT(buffer, buffer_length);

    nextion_protocol_send_instruction(handle, instruction.text, instruction.length, &parser);

    return NEX_OK;
}

nex_err_t nextion_eeprom_stream_begin(nextion_t *handle, uint16_t address, size_t value_count)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK_EEPROM_ADDRESS(address)
    CMP_CHECK_EEPROM_END_ADDRESS(address + value_count)
    CMP_CHECK((value_count < (NEX_DVC_TRANSPARENT_DATA_MAX_DATA_SIZE - 20)), "value_count error(>=NEX_DVC_TRANSPARENT_DATA_MAX_DATA_SIZE-20)", NEX_FAIL)

    return nextion_protocol_send_instruction_tdm_start(handle, "wept %d,%d", address, value_count);
}

nex_err_t nextion_eeprom_stream_write(const nextion_t *handle, uint8_t value)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    return nextion_protocol_send_raw_byte(handle, value);
}
