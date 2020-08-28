#include "common_infra.h"
#include "nextion/nextion.h"
#include "nextion/constants.h"
#include "nextion/op_waveform.h"

#ifdef __cplusplus
extern "C"
{
#endif

    nex_err_t nextion_waveform_start_refesh()
    {
        return NEX_SEND_COMMAND(8, "ref_star");
    }

    nex_err_t nextion_waveform_stop_refesh()
    {
        return NEX_SEND_COMMAND(8, "ref_stop");
    }

    nex_err_t nextion_waveform_add_value(uint8_t waveform_id, uint8_t channel_id, uint8_t value)
    {
        char buffer[16];

        snprintf(buffer, 16, "add %d,%d,%d", waveform_id, channel_id, value);

        uint8_t *response = NULL;

        // This operation does not respects the "bkcmd" value.
        // Will only return in case of failure.

        const int read = nextion_send_command_raw(buffer, &response, NEX_RESP_NO_BKCMD_WAIT_TIME);

        if (read == -1)
            return NEX_OK;

        return response[0];
    }

    nex_err_t nextion_waveform_clear_channel(uint8_t waveform_id, uint8_t channel_id)
    {
        return NEX_SEND_COMMAND(11, "cle %d,%d", waveform_id, channel_id);
    }

    nex_err_t nextion_waveform_clear(uint8_t waveform_id)
    {
        return NEX_SEND_COMMAND(11, "cle %d,255", waveform_id);
    }

#ifdef __cplusplus
}
#endif