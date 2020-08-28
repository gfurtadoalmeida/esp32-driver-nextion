#include <string.h>
#include "common_infra.h"
#include "nextion/constants.h"
#include "nextion/nextion.h"
#include "nextion/op_system.h"

#ifdef __cplusplus
extern "C"
{
#endif

    nex_err_t nextion_system_get_text(const char *command, char *text, int *text_length)
    {
        NEX_CHECK((command != NULL), "command error(NULL)", NEX_FAIL);
        NEX_CHECK((text != NULL), "text error(NULL)", NEX_FAIL);

        uint8_t *response = NULL;
        const int read = nextion_send_command_raw(command, &response, NEX_RESP_WAIT_TIME);

        if (read >= NEX_DVC_CMD_ACK_LENGTH && response[0] == NEX_DVC_RSP_GET_STRING)
        {
            const int length = read - NEX_DVC_CMD_ACK_LENGTH;

            text[length] = '\0';

            memcpy(text, response + NEX_DVC_CMD_START_LENGTH, length);

            *text_length = length;

            return NEX_OK;
        }

        return response[0];
    }

    nex_err_t nextion_system_get_number(const char *command, int32_t *number)
    {
        NEX_CHECK((command != NULL), "command error(NULL)", NEX_FAIL);
        NEX_CHECK((number != NULL), "number error(NULL)", NEX_FAIL);

        uint8_t *response = NULL;
        const int read = nextion_send_command_raw(command, &response, NEX_RESP_WAIT_TIME);

        if (read == 8 && response[0] == NEX_DVC_RSP_GET_NUMBER)
        {
            // Number: 4 bytes and signed = int32_t.
            // Sent in little endian format.
            *number = (int32_t)(((uint32_t)response[4] << 24) | ((uint32_t)response[3] << 16) | ((uint32_t)response[2] << 8) | (uint32_t)response[1]);

            return NEX_OK;
        }

        return response[0];
    }

    nex_err_t nextion_system_reset()
    {
        return NEX_SEND_COMMAND(4, "rest");
    }

    nex_err_t nextion_system_get_brightness(bool persisted, int *percentage)
    {
        NEX_CHECK((percentage != NULL), "percentage error(NULL)", NEX_FAIL);

        if (persisted)
        {
            return nextion_system_get_number("get dims", percentage);
        }
        else
        {
            return nextion_system_get_number("get dim", percentage);
        }
    }

    nex_err_t nextion_system_set_brightness(int percentage, bool persist)
    {
        if (percentage > 100)
        {
            percentage = 100;
        }
        else if (percentage < 0)
        {
            percentage = 0;
        }

        if (persist)
        {
            return NEX_SEND_COMMAND(8, "dims=%d", percentage);
        }
        else
        {
            return NEX_SEND_COMMAND(7, "dim=%d", percentage);
        }
    }

    nex_err_t nextion_system_set_sleep_no_touch(int seconds)
    {
        if (seconds > UINT16_MAX)
        {
            seconds = UINT16_MAX;
        }
        else if (seconds < 0)
        {
            seconds = 0;
        }

        return NEX_SEND_COMMAND(10, "thsp=%d", seconds);
    }

    nex_err_t nextion_system_set_wake_on_touch(bool wake_on_touch)
    {
        return NEX_SEND_COMMAND(6, "thup=%d", wake_on_touch);
    }

#ifdef __cplusplus
}
#endif