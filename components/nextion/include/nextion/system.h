#ifndef __NEXTION_SYSTEM_H__
#define __NEXTION_SYSTEM_H__

#include <stdint.h>
#include <stdbool.h>
#include "base/codes.h"
#include "base/types.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Sends a command that retrieves a null-terminated string.
     *
     * @details It's the caller responsibility to allocate a buffer big enough to
     * hold the text returned.
     *
     * @param handle Nextion context pointer.
     * @param command A null-terminated string with the command to be sent.
     * @param[in] buffer Location where the retrieved text will be stored. Must take the null-terminator into account.
     * @param[in] expected_length Expected text length that might be retrieved. Will be update with the retrieved text length.
     *
     * @return NEX_OK or NEX_DVC_ERR_INVALID_VARIABLE_OR_ATTRIBUTE.
     */
    nex_err_t nextion_system_get_text(nextion_handle_t handle,
                                      const char *command,
                                      char *buffer,
                                      size_t *expected_length);

    /**
     * @brief Sends a command that retrieves a signed number.
     *
     * @param handle Nextion context pointer.
     * @param command A null-terminated string with the command to be sent.
     * @param[in] number Location where the retrieved number will be stored.
     *
     * @return NEX_OK or NEX_DVC_ERR_INVALID_VARIABLE_OR_ATTRIBUTE.
     */
    nex_err_t nextion_system_get_number(nextion_handle_t handle,
                                        const char *command,
                                        int32_t *number);

    /**
     * @brief Resets the display, losing all volatile configurations.
     * @note It's mandatory, after (NEX_DVC_RESET_WAIT_TIME_MS) ms, to call
     * "nextion_init" after calling this function.
     * @param handle Nextion context pointer.
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_system_reset(nextion_handle_t handle);

    /**
     * @brief Enters in sleep mode.
     * @param handle Nextion context pointer.
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_system_sleep(nextion_handle_t handle);

    /**
     * @brief Exits sleep mode.
     * @param handle Nextion context pointer.
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_system_wakeup(nextion_handle_t handle);

    /**
     * @brief Get the display brightness.
     *
     * @param handle Nextion context pointer.
     * @param persisted If it will read the persisted or transient value.
     * @param percentage Brightness level, in percentage (0-100).
     *
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_system_get_brightness(nextion_handle_t handle, bool persisted, uint8_t *percentage);

    /**
     * @brief Set the display brightness.
     *
     * @param handle Nextion context pointer.
     * @param percentage Brightness level, in percentage (0-100).
     * @param persist If the value must persist between resets.
     *
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_system_set_brightness(nextion_handle_t handle, uint8_t percentage, bool persist);

    /**
     * @brief Get how long the display will be on after the last touch.
     *
     * @param handle Nextion context pointer.
     * @param seconds Seconds to wait. Range: 0 (disabled) to 65535 (18h 12m 15s).
     *
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_system_get_sleep_no_touch(nextion_handle_t handle, uint16_t *seconds);

    /**
     * @brief Set how long the display will be on after the last touch.
     * @note Not persisted through resets.
     *
     * @param handle Nextion context pointer.
     * @param seconds Seconds to wait. Range: 0 (disabled) to 65535 (18h 12m 15s).
     *
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_system_set_sleep_no_touch(nextion_handle_t handle, uint16_t seconds);

    /**
     * @brief Get how long the display will be on after the last serial command.
     *
     * @param handle Nextion context pointer.
     * @param seconds Seconds to wait. Range: 0 (disabled) to 65535 (18h 12m 15s).
     *
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_system_get_sleep_no_serial(nextion_handle_t handle, uint16_t *seconds);

    /**
     * @brief Set how long the display will be on after the last serial command.
     * @note Not persisted through resets.
     *
     * @param handle Nextion context pointer.
     * @param seconds Seconds to wait. Range: 0 (disabled) to 65535 (18h 12m 15s).
     *
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_system_set_sleep_no_serial(nextion_handle_t handle, uint16_t seconds);

    /**
     * @brief Set if the display will turn on when touched.
     * @note Not persisted through resets.
     *
     * @param handle Nextion context pointer.
     * @param wake_on_touch If will turn on or not when touched.
     *
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_system_set_wake_on_touch(nextion_handle_t handle, bool wake_on_touch);

    /**
     * @brief Set if the display will turn when receiving serial data.
     * @note Not persisted through resets.
     *
     * @param handle Nextion context pointer.
     * @param wake_on_serial If will turn on or not when serial data is received.
     *
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_system_set_wake_on_serial(nextion_handle_t handle, bool wake_on_serial);

    /**
     * @brief Set if the display will send x and y touch coordinates on every touch.
     * @note Not persisted through resets.
     *
     * @param handle Nextion context pointer.
     * @param wake_on_serial If will send the coordinates or not.
     *
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_system_set_send_xy(nextion_handle_t handle, bool send_xy);

#ifdef __cplusplus
}
#endif

#endif //__NEXTION_SYSTEM_H__