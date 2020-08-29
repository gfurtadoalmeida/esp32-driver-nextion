#ifndef __NEXTION_OP_SYSTEM_H__
#define __NEXTION_OP_SYSTEM_H__

#include <stdint.h>
#include "codes.h"

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
     * @param command A null-terminated string with the command to be sent.
     * @param[in] text Location where the retrieved text will be stored. Must take the null-terminator into account.
     * @param[in] text_length Location where the text length will be stored.
     *
     * @return NEX_OK or NEX_DVC_ERR_INVALID_VARIABLE_OR_ATTRIBUTE.
     */
    nex_err_t nextion_system_get_text(const char *command, char *text, int *text_length);

    /**
     * @brief Sends a command that retrieves a signed number.
     *
     * @param command A null-terminated string with the command to be sent.
     * @param[in] number Location where the retrieved number will be stored.
     *
     * @return NEX_OK or NEX_DVC_ERR_INVALID_VARIABLE_OR_ATTRIBUTE.
     */
    nex_err_t nextion_system_get_number(const char *command, int32_t *number);

    /**
     * @brief Resets the display.
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_system_reset();

    /**
     * @brief Get the display backlight brightness.
     *
     * @param persisted If will read the persisted or transient value.
     * @param percentage Brightness level, in percentage (0-100).
     *
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_system_get_brightness(bool persisted, int *percentage);

    /**
     * @brief Set the display backlight brightness.

     * @param percentage Brightness level, in percentage (0-100).
     * @param persist If the value must persist between resets.

     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_system_set_brightness(int percentage, bool persist);

    /**
     * @brief Set how long the display backlight will be on after
     * the last touch.
     * @note Not persisted through resets.
     * @param seconds Seconds to wait. Max: 65535 (18h 12m 15s).
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_system_set_sleep_no_touch(int seconds);

    /**
     * @brief Set if the display backlight will turn on when touched.
     * @note Not persisted through resets.
     * @param wake_on_touch If will turn on or not when touched.
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_system_set_wake_on_touch(bool wake_on_touch);

#ifdef __cplusplus
}
#endif

#endif //__NEXTION_OP_SYSTEM_H__