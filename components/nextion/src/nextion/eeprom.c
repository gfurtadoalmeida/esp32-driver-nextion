#include "common_infra.h"
#include "nextion/nextion.h"
#include "nextion/eeprom.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define NEX_CHECK_EEPROM_ADDRESS(address) NEX_CHECK((address < NEX_DVC_EEPROM_SIZE), "address error(address > NEX_DVC_EEPROM_MAX_ADDRESS)", NEX_FAIL)
#define NEX_CHECK_EEPROM_END_ADDRESS(address) NEX_CHECK(((address) < NEX_DVC_EEPROM_SIZE), "address error(end address > NEX_DVC_EEPROM_MAX_ADDRESS)", NEX_FAIL)

    nex_err_t nextion_eeprom_write_text(nextion_handle_t handle,
                                        uint16_t address,
                                        const char *text,
                                        size_t text_length)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);
        NEX_CHECK_EEPROM_ADDRESS(address);
        NEX_CHECK_EEPROM_END_ADDRESS(address + text_length);
        NEX_CHECK((text != NULL), "text error(NULL)", NEX_FAIL);

        return nextion_command_send(handle, "wepo \"%s\",%d", text, address);
    }

    nex_err_t nextion_eeprom_write_number(nextion_handle_t handle,
                                          uint16_t address,
                                          int32_t value)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);
        NEX_CHECK_EEPROM_ADDRESS(address);
        NEX_CHECK_EEPROM_END_ADDRESS(address + 4);

        return nextion_command_send(handle, "wepo %d,%d", value, address);
    }

    nex_err_t nextion_eeprom_read_text(nextion_handle_t handle,
                                       uint16_t address,
                                       char *text,
                                       size_t text_length)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);
        NEX_CHECK_EEPROM_ADDRESS(address);
        NEX_CHECK_EEPROM_END_ADDRESS(address + text_length);

        text[text_length] = '\0';

        return nextion_eeprom_read_bytes(handle, address, (uint8_t *)text, text_length);
    }

    nex_err_t nextion_eeprom_read_number(nextion_handle_t handle,
                                         uint16_t address,
                                         int32_t *number)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);
        NEX_CHECK_EEPROM_ADDRESS(address);
        NEX_CHECK_EEPROM_END_ADDRESS(address + 4);

        uint8_t buffer[4];

        if (nextion_eeprom_read_bytes(handle, address, buffer, 4) != NEX_OK)
            return NEX_FAIL;

        // Number: 4 bytes and signed = int32_t.
        // Sent in little endian format.
        *number = (int32_t)(((uint32_t)buffer[3] << 24) | ((uint32_t)buffer[2] << 16) | ((uint32_t)buffer[1] << 8) | (uint32_t)buffer[0]);

        return NEX_OK;
    }

    nex_err_t nextion_eeprom_read_bytes(nextion_handle_t handle,
                                        uint16_t address,
                                        uint8_t *buffer,
                                        size_t buffer_length)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);
        NEX_CHECK_EEPROM_ADDRESS(address);
        NEX_CHECK_EEPROM_END_ADDRESS(address + buffer_length);

        size_t length = buffer_length;

        // It is no use updating "buffer_length" because it will always read
        // exactly what it is asked.

        return nextion_command_send_get_bytes(handle, buffer, &length, "rept %d,%zd", address, length);
    }

    nex_err_t nextion_eeprom_stream_begin(nextion_handle_t handle, uint16_t address, size_t value_count)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);
        NEX_CHECK_EEPROM_ADDRESS(address);
        NEX_CHECK_EEPROM_END_ADDRESS(address + value_count);
        NEX_CHECK((value_count < (NEX_DVC_TRANSPARENT_DATA_MAX_DATA_SIZE - 20)), "value_count error(>=NEX_DVC_TRANSPARENT_DATA_MAX_DATA_SIZE-20)", NEX_FAIL);

        return nextion_transparent_data_mode_begin(handle, value_count, "wept %d,%zd", address, value_count);
    }

    nex_err_t nextion_eeprom_stream_write(nextion_handle_t handle, uint8_t value)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);

        return nextion_transparent_data_mode_write(handle, value);
    }

    nex_err_t nextion_eeprom_stream_end(nextion_handle_t handle)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);

        return nextion_transparent_data_mode_end(handle);
    }

#ifdef __cplusplus
}
#endif