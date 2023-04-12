#include "esp32_driver_nextion/nextion.h"
#include "esp32_driver_nextion/waveform.h"
#include "assertion.h"

nex_err_t nextion_waveform_start_refesh(nextion_t *handle)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    return nextion_command_send(handle, "ref_star");
}

nex_err_t nextion_waveform_stop_refesh(nextion_t *handle)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    return nextion_command_send(handle, "ref_stop");
}

nex_err_t nextion_waveform_add_value(nextion_t *handle,
                                     uint8_t waveform_id,
                                     uint8_t channel_id,
                                     uint8_t value)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    nex_err_t code = nextion_command_send(handle, "add %d,%d,%d", waveform_id, channel_id, value);

    // This operation does not respects the "bkcmd" value.
    // Will only return in case of failure.

    if (code == NEX_TIMEOUT)
    {
        return NEX_OK;
    }

    return code;
}

nex_err_t nextion_waveform_clear_channel(nextion_t *handle,
                                         uint8_t waveform_id,
                                         uint8_t channel_id)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    return nextion_command_send(handle, "cle %d,%d", waveform_id, channel_id);
}

nex_err_t nextion_waveform_clear(nextion_t *handle, uint8_t waveform_id)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    return nextion_command_send(handle, "cle %d,255", waveform_id);
}

nex_err_t nextion_waveform_stream_begin(nextion_t *handle,
                                        uint8_t waveform_id,
                                        uint8_t channel_id,
                                        size_t value_count)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK((value_count < (NEX_DVC_TRANSPARENT_DATA_MAX_DATA_SIZE - 20)), "value_count error(>=NEX_DVC_TRANSPARENT_DATA_MAX_DATA_SIZE-20)", NEX_FAIL)

    return nextion_transparent_data_mode_begin(handle,
                                               value_count,
                                               "addt %d,%d,%d",
                                               waveform_id,
                                               channel_id,
                                               value_count);
}

nex_err_t nextion_waveform_stream_write(nextion_t *handle, uint8_t value)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    return nextion_transparent_data_mode_write(handle, value);
}

nex_err_t nextion_waveform_stream_end(nextion_t *handle)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    return nextion_transparent_data_mode_end(handle);
}