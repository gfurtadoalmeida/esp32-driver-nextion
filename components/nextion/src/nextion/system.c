#include <malloc.h>
#include <string.h>
#include "common_infra.h"
#include "nextion/nextion.h"
#include "nextion/system.h"

#ifdef __cplusplus
extern "C"
{
#endif

    nex_err_t nextion_system_get_text(nextion_handle_t handle,
                                      const char *command,
                                      char *buffer,
                                      size_t *expected_length)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);
        NEX_CHECK((command != NULL), "command error(NULL)", NEX_FAIL);
        NEX_CHECK((buffer != NULL), "text error(NULL)", NEX_FAIL);
        NEX_CHECK((expected_length != NULL), "expected length error(NULL)", NEX_FAIL);

        size_t length = *expected_length + NEX_DVC_CMD_ACK_LENGTH;
        uint8_t *response = (uint8_t *)malloc(length);

        if (nextion_command_send_get_bytes(handle, response, &length, command) != NEX_OK)
        {
            free(response);

            return NEX_FAIL;
        }

        if (length == NEX_DVC_CMD_ACK_LENGTH)
        {
            // In case of error it will send the basic ACK response.

            nex_err_t code = response[0];

            free(response);

            return code;
        }

        if (length > NEX_DVC_CMD_ACK_LENGTH && response[0] == NEX_DVC_RSP_GET_STRING)
        {
            length -= NEX_DVC_CMD_ACK_LENGTH;

            buffer[length] = '\0';

            memcpy(buffer, response + NEX_DVC_CMD_START_LENGTH, length);

            *expected_length = length;

            free(response);

            return NEX_OK;
        }

        free(response);

        return NEX_FAIL;
    }

    nex_err_t nextion_system_get_number(nextion_handle_t handle,
                                        const char *command,
                                        int32_t *number)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);
        NEX_CHECK((command != NULL), "command error(NULL)", NEX_FAIL);
        NEX_CHECK((number != NULL), "number error(NULL)", NEX_FAIL);

        uint8_t buffer[8];
        size_t length = 8;

        if (nextion_command_send_get_bytes(handle, buffer, &length, command) != NEX_OK)
        {
            return NEX_FAIL;
        }

        if (length == NEX_DVC_CMD_ACK_LENGTH)
        {
            // In case of error it will send the basic ACK response.

            return buffer[0];
        }

        if (length == 8 && buffer[0] == NEX_DVC_RSP_GET_NUMBER)
        {
            // Number: 4 bytes and signed = int32_t.
            // Sent in little endian format.

            *number = (int32_t)(((uint32_t)buffer[4] << 24) | ((uint32_t)buffer[3] << 16) | ((uint32_t)buffer[2] << 8) | (uint32_t)buffer[1]);

            return NEX_OK;
        }

        return NEX_FAIL;
    }

    nex_err_t nextion_system_reset(nextion_handle_t handle)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);

        nex_err_t code = nextion_command_send(handle, "rest");

        if (code == NEX_OK || code == NEX_TIMEOUT)
        {
            return NEX_OK;
        }

        return NEX_FAIL;
    }

    nex_err_t nextion_system_get_brightness(nextion_handle_t handle, bool persisted, uint8_t *percentage)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);
        NEX_CHECK((percentage != NULL), "percentage error(NULL)", NEX_FAIL);

        int32_t value = 0;
        nex_err_t result;

        if (persisted)
        {
            result = nextion_system_get_number(handle, "get dims", &value);
        }
        else
        {
            result = nextion_system_get_number(handle, "get dim", &value);
        }

        *percentage = (uint8_t)value;

        return result;
    }

    nex_err_t nextion_system_set_brightness(nextion_handle_t handle, uint8_t percentage, bool persist)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);

        if (percentage > 100)
        {
            percentage = 100;
        }

        if (persist)
        {
            return nextion_command_send(handle, "dims=%d", percentage);
        }
        else
        {
            return nextion_command_send(handle, "dim=%d", percentage);
        }
    }

    nex_err_t nextion_system_get_sleep_no_touch(nextion_handle_t handle, uint16_t *seconds)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);
        NEX_CHECK((seconds != NULL), "seconds error(NULL)", NEX_FAIL);

        int32_t value = 0;
        nex_err_t code = nextion_system_get_number(handle, "get thsp", &value);

        *seconds = value;

        return code;
    }

    nex_err_t nextion_system_set_sleep_no_touch(nextion_handle_t handle, uint16_t seconds)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);

        return nextion_command_send(handle, "thsp=%d", seconds);
    }

    nex_err_t nextion_system_set_wake_on_touch(nextion_handle_t handle, bool wake_on_touch)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);

        return nextion_command_send(handle, "thup=%d", wake_on_touch);
    }

#ifdef __cplusplus
}
#endif
