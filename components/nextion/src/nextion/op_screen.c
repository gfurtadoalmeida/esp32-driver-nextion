#include "common_infra.h"
#include "nextion/constants.h"
#include "nextion/nextion.h"
#include "nextion/op_screen.h"

#ifdef __cplusplus
extern "C"
{
#endif

    nex_err_t nextion_get_current_page(int *page_id)
    {
        uint8_t *response = NULL;
        const int read = send_command_raw("sendme", &response);

        if (read == 5 && response[0] == NEX_DVC_RSP_SENDME_RESULT)
        {
            *page_id = response[1];

            return NEX_OK;
        }

        return response[0];
    }

#ifdef __cplusplus
}
#endif