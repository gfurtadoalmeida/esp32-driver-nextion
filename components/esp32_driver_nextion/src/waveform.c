#include "esp32_driver_nextion/base/constants.h"
#include "esp32_driver_nextion/waveform.h"
#include "protocol/protocol.h"
#include "assertion.h"

nex_err_t nextion_waveform_start_refesh(nextion_t *handle)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    return nextion_protocol_send_instruction_ack(handle, "ref_star");
}

nex_err_t nextion_waveform_stop_refesh(nextion_t *handle)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    return nextion_protocol_send_instruction_ack(handle, "ref_stop");
}

nex_err_t nextion_waveform_add_value(nextion_t *handle,
                                     uint8_t waveform_id,
                                     uint8_t channel_id,
                                     uint8_t value)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    nex_err_t code = nextion_protocol_send_instruction_ack(handle, "add %d,%d,%d", waveform_id, channel_id, value);

    // This operation will only return data in case of failure.

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

    return nextion_protocol_send_instruction_ack(handle, "cle %d,%d", waveform_id, channel_id);
}

nex_err_t nextion_waveform_clear(nextion_t *handle, uint8_t waveform_id)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    return nextion_waveform_clear_channel(handle, waveform_id, 255);
}

nex_err_t nextion_waveform_stream_begin(nextion_t *handle,
                                        uint8_t waveform_id,
                                        uint8_t channel_id,
                                        size_t value_count)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK((value_count < (NEX_DVC_TRANSPARENT_DATA_MAX_DATA_SIZE - 20)), "value_count error(>=NEX_DVC_TRANSPARENT_DATA_MAX_DATA_SIZE-20)", NEX_FAIL)

    return nextion_protocol_send_instruction_tdm_start(handle, "addt %d,%d,%d", waveform_id, channel_id, value_count);
}

nex_err_t nextion_waveform_stream_write(const nextion_t *handle, uint8_t value)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    return nextion_protocol_send_raw_byte(handle, value);
}
