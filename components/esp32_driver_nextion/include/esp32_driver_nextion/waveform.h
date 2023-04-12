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
     * @brief Resumes default waveform refreshing (refresh on data point add).
     * @param handle Nextion context pointer.
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_waveform_start_refesh(nextion_t *handle);

    /**
     * @brief Stops default waveform refreshing (will not refresh when data point added).
     * @param handle Nextion context pointer.
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_waveform_stop_refesh(nextion_t *handle);

    /**
     * @brief Add a single value to a waveform channel.
     *
     * @param handle Nextion context pointer.
     * @param waveform_id Waveform id.
     * @param channel_id Channel id to add data on.
     * @param value Value to be added.
     *
     * @return NEX_OK or NEX_DVC_ERR_INVALID_WAVEFORM.
     */
    nex_err_t nextion_waveform_add_value(nextion_t *handle,
                                         uint8_t waveform_id,
                                         uint8_t channel_id,
                                         uint8_t value);

    /**
     * @brief Clears a single waveform channel.
     *
     * @param handle Nextion context pointer.
     * @param waveform_id Waveform id.
     * @param channel_id Channel id to add data on.
     *
     * @return NEX_OK or NEX_DVC_ERR_INVALID_WAVEFORM.
     */
    nex_err_t nextion_waveform_clear_channel(nextion_t *handle,
                                             uint8_t waveform_id,
                                             uint8_t channel_id);

    /**
     * @brief Clears all waveform channels.
     * @param handle Nextion context pointer.
     * @param waveform_id Waveform id.
     * @return NEX_OK or NEX_DVC_ERR_INVALID_WAVEFORM.
     */
    nex_err_t nextion_waveform_clear(nextion_t *handle, uint8_t waveform_id);

    /**
     * @brief Begins the waveform data streaming.
     *
     * @details When in this mode, the device will "hang" until all
     * data is sent; no event or other commands will be processed.
     *
     * @param handle Nextion context pointer.
     * @param waveform_id Waveform id.
     * @param channel_id Channel id to add data on.
     * @param value_count How many values will be written. "value_count < (NEX_DVC_TRANSPARENT_DATA_MAX_DATA_SIZE - 20)"
     *
     * @return NEX_OK if success, otherwise any NEX_DVC_ERR_* value.
     */
    nex_err_t nextion_waveform_stream_begin(nextion_t *handle,
                                            uint8_t waveform_id,
                                            uint8_t channel_id,
                                            size_t value_count);

    /**
     * @brief Writes a value onto the waveform stream.
     *
     * @param handle Nextion context pointer.
     * @param value Value to be written.
     *
     * @return NEX_OK if success, otherwise NEX_FAIL.
     */
    nex_err_t nextion_waveform_stream_write(nextion_t *handle, uint8_t value);

    /**
     * @brief Ends the waveform streaming.
     * @param handle Nextion context pointer.
     * @return NEX_OK if success, otherwise NEX_FAIL.
     */
    nex_err_t nextion_waveform_stream_end(nextion_t *handle);

#ifdef __cplusplus
}
#endif
#endif