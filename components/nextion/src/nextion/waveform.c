#include "common_infra.h"
#include "nextion/nextion.h"
#include "nextion/waveform.h"

#ifdef __cplusplus
extern "C"
{
#endif

    nex_err_t nextion_waveform_start_refesh(nextion_handle_t handle)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);

        return nextion_command_send(handle, "ref_star");
    }

    nex_err_t nextion_waveform_stop_refesh(nextion_handle_t handle)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);

        return nextion_command_send(handle, "ref_stop");
    }

    nex_err_t nextion_waveform_add_value(nextion_handle_t handle,
                                         uint8_t waveform_id,
                                         uint8_t channel_id,
                                         uint8_t value)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);

        nex_err_t code = nextion_command_send(handle, "add %d,%d,%d", waveform_id, channel_id, value);

        // This operation does not respects the "bkcmd" value.
        // Will only return in case of failure.

        if (code == NEX_TIMEOUT)
        {
            return NEX_OK;
        }

        return code;
    }

    nex_err_t nextion_waveform_clear_channel(nextion_handle_t handle,
                                             uint8_t waveform_id,
                                             uint8_t channel_id)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);

        return nextion_command_send(handle, "cle %d,%d", waveform_id, channel_id);
    }

    nex_err_t nextion_waveform_clear(nextion_handle_t handle, uint8_t waveform_id)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);

        return nextion_command_send(handle, "cle %d,255", waveform_id);
    }

    nex_err_t nextion_waveform_stream_begin(nextion_handle_t handle,
                                            uint8_t waveform_id,
                                            uint8_t channel_id,
                                            size_t value_count)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);
        NEX_CHECK((value_count < (NEX_DVC_TRANSPARENT_DATA_MAX_DATA_SIZE - 20)), "value_count error(>=NEX_DVC_TRANSPARENT_DATA_MAX_DATA_SIZE-20)", NEX_FAIL);

        return nextion_transparent_data_mode_begin(handle, value_count, "addt %d,%d,%zd", waveform_id, channel_id, value_count);
    }

    nex_err_t nextion_waveform_stream_write(nextion_handle_t handle, uint8_t value)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);

        return nextion_transparent_data_mode_write(handle, value);
    }

    nex_err_t nextion_waveform_stream_end(nextion_handle_t handle)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);

        return nextion_transparent_data_mode_end(handle);
    }
#ifdef __cplusplus
}
#endif