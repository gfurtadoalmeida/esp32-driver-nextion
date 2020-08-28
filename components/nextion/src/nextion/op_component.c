#include <string.h>
#include "common_infra.h"
#include "nextion/nextion.h"
#include "nextion/constants.h"
#include "nextion/op_component.h"

#ifdef __cplusplus
extern "C"
{
#endif

    nex_err_t nextion_get_text(const char *command, char *text, int *text_length)
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

    nex_err_t nextion_get_number(const char *command, int32_t *number)
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

    nex_err_t nextion_component_refresh(char *component_name_or_id)
    {
        return NEX_SEND_COMMAND(4 + NEX_DVC_COMPONENT_MAX_NAME_LENGTH, "ref %s", component_name_or_id);
    }

    nex_err_t nextion_component_set_visibility(char *component_name_or_id, bool visible_or_not)
    {
        return NEX_SEND_COMMAND(6 + NEX_DVC_COMPONENT_MAX_NAME_LENGTH, "vis %s,%d", component_name_or_id, visible_or_not);
    }

    nex_err_t nextion_component_set_visibility_all(bool visible_or_not)
    {
        return NEX_SEND_COMMAND(9, "vis 255,%d", visible_or_not);
    }

    nex_err_t nextion_component_set_touchable(char *component_name_or_id, bool touchable_or_not)
    {
        return NEX_SEND_COMMAND(6 + NEX_DVC_COMPONENT_MAX_NAME_LENGTH, "tsw %s,%d", component_name_or_id, touchable_or_not);
    }

    nex_err_t nextion_component_set_touchable_all(bool touchable_or_not)
    {
        return NEX_SEND_COMMAND(9, "tsw 255,%d", touchable_or_not);
    }

#ifdef __cplusplus
}
#endif