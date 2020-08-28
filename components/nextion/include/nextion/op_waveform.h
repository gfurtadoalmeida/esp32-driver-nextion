#ifndef __NEXTION_OP_WAVEFORM_H__
#define __NEXTION_OP_WAVEFORM_H__

#include <stdint.h>
#include "codes.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Resumes default waveform refreshing (refresh on data point add).
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_waveform_start_refesh();

    /**
     * @brief Stops default waveform refreshing (will not refresh when data point added).
     * @return NEX_OK or NEX_DVC_ERR_INVALID_PAGE.
     */
    nex_err_t nextion_waveform_stop_refesh();

    /**
     * @brief Add a single value to a waveform channel.
     *
     * @param waveform_id Waveform id.
     * @param channel_id Channel id to add data on.
     * @param value Value to be added.
     *
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_waveform_add_value(uint8_t waveform_id, uint8_t channel_id, uint8_t value);

    /**
     * @brief Clears a single waveform channel.
     *
     * @param waveform_id Waveform id.
     * @param channel_id Channel id to add data on.
     *
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_waveform_clear_channel(uint8_t waveform_id, uint8_t channel_id);

    /**
     * @brief Clears all waveform channels.
     * @param waveform_id Waveform id.
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_waveform_clear(uint8_t waveform_id);
#ifdef __cplusplus
}
#endif

#endif //__NEXTION_OP_WAVEFORM_H__