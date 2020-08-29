#include <string.h>
#include "common_infra.h"
#include "nextion/nextion.h"
#include "nextion/constants.h"
#include "nextion/op_page.h"

#ifdef __cplusplus
extern "C"
{
#endif

    nex_err_t nextion_page_get(uint8_t *page_id)
    {
        NEX_CHECK((page_id != NULL), "page_id error(NULL)", NEX_FAIL);

        uint8_t *response = NULL;
        const int read = nextion_send_command_raw("sendme", &response, NEX_RESP_WAIT_TIME);

        if (read == 5 && response[0] == NEX_DVC_RSP_SENDME_RESULT)
        {
            *page_id = response[1];

            return NEX_OK;
        }

        return response[0];
    }

    nex_err_t nextion_page_set(uint8_t page_id)
    {
        return NEX_SEND_COMMAND(8, "page %d", page_id);
    }

    nex_err_t nextion_page_refresh()
    {
        return NEX_SEND_COMMAND(5, "ref 0");
    }

#ifdef __cplusplus
}
#endif