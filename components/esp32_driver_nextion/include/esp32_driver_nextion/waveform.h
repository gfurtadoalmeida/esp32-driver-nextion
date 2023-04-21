#ifndef __ESP32_DRIVER_NEXTION_WAVEFORM_H__
#define __ESP32_DRIVER_NEXTION_WAVEFORM_H__

#include <stdint.h>
#include <stddef.h>
#include "base/codes.h"
#include "base/types.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Start default waveform refreshing (refresh on data point add).
     * @param[in] handle Nextion context pointer.
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_waveform_start_refesh(nextion_t *handle);

    /**
     * @brief Stop default waveform refreshing (will not refresh when data point added).
     * @param[in] handle Nextion context pointer.
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_waveform_stop_refesh(nextion_t *handle);

    /**
     * @brief Add a single value to a waveform channel.
     * @param[in] handle Nextion context pointer.
     * @param[in] waveform_id Waveform id.
     * @param[in] channel_id Channel id to add data on.
     * @param[in] value Value to be added.
     * @return NEX_OK or NEX_DVC_ERR_INVALID_WAVEFORM.
     */
    nex_err_t nextion_waveform_add_value(nextion_t *handle,
                                         uint8_t waveform_id,
                                         uint8_t channel_id,
                                         uint8_t value);

    /**
     * @brief Clear a single waveform channel.
     * @param[in] handle Nextion context pointer.
     * @param[in] waveform_id Waveform id.
     * @param[in] channel_id Channel id to add data on.
     * @return NEX_OK or NEX_DVC_ERR_INVALID_WAVEFORM.
     */
    nex_err_t nextion_waveform_clear_channel(nextion_t *handle,
                                             uint8_t waveform_id,
                                             uint8_t channel_id);

    /**
     * @brief Clear all waveform channels.
     * @param[in] handle Nextion context pointer.
     * @param[in] waveform_id Waveform id.
     * @return NEX_OK or NEX_DVC_ERR_INVALID_WAVEFORM.
     */
    nex_err_t nextion_waveform_clear(nextion_t *handle, uint8_t waveform_id);

    /**
     * @brief Begin the waveform data streaming.
     * @note When in this mode, the device will "hang" until all
     * data is sent; no event or other commands will be processed.
     * @param[in] handle Nextion context pointer.
     * @param[in] waveform_id Waveform id.
     * @param[in] channel_id Channel id to add data on.
     * @param[in] value_count How many values will be written. "value_count < (NEX_DVC_TRANSPARENT_DATA_MAX_DATA_SIZE - 20)"
     * @return NEX_OK if success, otherwise any NEX_DVC_ERR_* value.
     */
    nex_err_t nextion_waveform_stream_begin(nextion_t *handle,
                                            uint8_t waveform_id,
                                            uint8_t channel_id,
                                            size_t value_count);

    /**
     * @brief Write a value onto the waveform stream.
     * @param[in] handle Nextion context pointer.
     * @param[in] value Value to be written.
     * @return NEX_OK if success, otherwise NEX_FAIL.
     */
    nex_err_t nextion_waveform_stream_write(nextion_t *handle, uint8_t value);

    /**
     * @brief End the waveform streaming.
     * @param[in] handle Nextion context pointer.
     * @return NEX_OK if success, otherwise NEX_FAIL.
     */
    nex_err_t nextion_waveform_stream_end(nextion_t *handle);

#ifdef __cplusplus
}
#endif
#endif