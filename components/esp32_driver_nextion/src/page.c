#include "esp32_driver_nextion/nextion.h"
#include "esp32_driver_nextion/page.h"
#include "assertion.h"

nex_err_t nextion_page_get(nextion_t *handle, uint8_t *page_id)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK((page_id != NULL), "page_id error(NULL)", NEX_FAIL)

    uint8_t buffer[5];
    size_t length = 5;

    if (nextion_command_send_get_bytes(handle, buffer, &length, "sendme") != NEX_OK)
    {
        return NEX_FAIL;
    }

    if (length == NEX_DVC_CMD_ACK_LENGTH)
    {
        return buffer[0];
    }

    if (length == 5 && buffer[0] == NEX_DVC_RSP_SENDME_RESULT)
    {
        *page_id = buffer[1];

        return NEX_OK;
    }

    return NEX_FAIL;
}

nex_err_t nextion_page_set(nextion_t *handle, const char *page_name_or_id)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    return nextion_command_send(handle, "page %s", page_name_or_id);
}

nex_err_t nextion_page_refresh(nextion_t *handle)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    return nextion_command_send(handle, "ref 0");
}