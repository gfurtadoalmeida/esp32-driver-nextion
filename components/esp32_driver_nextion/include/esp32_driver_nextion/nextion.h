#ifndef __ESP32_DRIVER_NEXTION_NEXTION_H__
#define __ESP32_DRIVER_NEXTION_NEXTION_H__

#include <stdint.h>
#include "driver/gpio.h"
#include "driver/uart.h"
#include "base/constants.h"
#include "base/codes.h"
#include "base/types.h"
#include "base/events.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Installs the Nextion driver and creates a Nextion context with the driver.
     *
     * @details UART ISR handler will be attached to the same CPU core that this function is running on.
     *
     * @note It will call "nextion_create".
     *
     * @param uart_num UART port number; any uart_port_t value.
     * @param baud_rate UART baud rate, between NEX_UART_BAUD_RATE_MIN and NEX_UART_BAUD_RATE_MAX.
     * @param tx_io_num UART TX pin GPIO number.
     * @param rx_io_num UART RX pin GPIO number.
     *
     * @return Pointer to a Nextion context or NULL.
     */
    nextion_t *nextion_driver_install(uart_port_t uart_num,
                                      uint32_t baud_rate,
                                      gpio_num_t tx_io_num,
                                      gpio_num_t rx_io_num);
    /**
     * @brief Deletes a Nextion driver and context.
     * @note It will call "nextion_free".
     * @param handle Nextion driver.
     * @return True if success, otherwise false.
     */
    bool nextion_driver_delete(nextion_t *handle);

    /**
     * @brief Does the initializations necessary before any action can be done.
     * @note Will turn the display on.
     * @param handle Nextion context pointer.
     * @return NEX_OK if success, otherwise NEX_FAIL.
     */
    nex_err_t nextion_init(nextion_t *handle);

    /**
     * @brief Sends a command that waits for a simple response (ACK).
     *
     * @param handle Nextion context pointer.
     * @param command Command to be sent (null-terminated).
     * @param ... Command format arguments.
     *
     * @return NEX_OK if success, NEX_TIMEOUT if timeout or any NEX_DVC_ERR_* value.
     */
    nex_err_t nextion_command_send(nextion_t *handle, const char *command, ...);

    /**
     * @brief Sends a command that waits for a simple response (ACK). Variadic version.
     *
     * @param handle Nextion context pointer.
     * @param command Command to be sent (null-terminated).
     * @param args Command format arguments.
     *
     * @return NEX_OK if success, NEX_TIMEOUT if timeout or any NEX_DVC_ERR_* value.
     */
    nex_err_t nextion_command_send_variadic(nextion_t *handle, const char *command, va_list args);

    /**
     * @brief Sends a command that returns bytes.
     *
     * @param handle Nextion context pointer.
     * @param buffer Location where the bytes will be stored.
     * @param legth Buffer length. Will be updated with the retrieved bytes count.
     * @param command Command to be sent (null-terminated).
     * @param ... Command format arguments.
     *
     * @return NEX_OK if success, otherwise any NEX_DVC_ERR_* value.
     */
    nex_err_t nextion_command_send_get_bytes(nextion_t *handle, uint8_t *buffer, size_t *length, const char *command, ...);

    /**
     * @brief Sets a callback for when a component is touched; 'on touch' events.
     * @note Only the last registration will be called; you cannot register more then one callback.
     * @param handle Nextion context pointer.
     * @param callback Callback function.
     * @return True if success, otherwise false.
     */
    bool nextion_event_callback_set_on_touch(nextion_t *handle, event_callback_on_touch callback);

    /**
     * @brief Sets a callback for when something is touched and "sendxy=1"; 'on touch with coordinates' events.
     * @note Only the last registration will be called; you cannot register more then one callback.
     * @param handle Nextion context pointer.
     * @param callback Callback function.
     * @return True if success, otherwise false.
     */
    bool nextion_event_callback_set_on_touch_coord(nextion_t *handle, event_callback_on_touch_coord callback);

    /**
     * @brief Sets a callback for when a device event happens; 'on device' events.
     * @note Only the last registration will be called; you cannot register more then one callback.
     * @param handle Nextion context pointer.
     * @param callback Callback function.
     * @return True if success, otherwise false.
     */
    bool nextion_event_callback_set_on_device(nextion_t *handle, event_callback_on_device callback);

    /**
     * @brief Begins the "Transparent Data Mode".
     *
     * @details When in this mode, the device "hangs" until all
     * data is sent; no event or other commands will be processed.
     *
     * @param handle Nextion context pointer.
     * @param data_size How many bytes will be written. "(command_length + NEX_DVC_CMD_END_LENGTH + data_size) < NEX_DVC_TRANSPARENT_DATA_MAX_DATA_SIZE"
     * @param command Command that will start it.
     * @param ... Command format arguments.
     * @return NEX_OK if success, otherwise any NEX_DVC_ERR_* value.
     */
    nex_err_t nextion_transparent_data_mode_begin(nextion_t *handle,
                                                  size_t data_size,
                                                  const char *command,
                                                  ...);

    /**
     * @brief Writes a value onto the device serial buffer.
     *
     * @note Use only when in "Transparent Data Mode".
     *
     * @param handle Nextion context pointer.
     * @param byte Value to be written.
     *
     * @return NEX_OK if success, otherwise NEX_FAIL.
     */
    nex_err_t nextion_transparent_data_mode_write(nextion_t *handle, uint8_t value);

    /**
     * @brief Ends the "Transparent Data Mode".
     * @param handle Nextion context pointer.
     * @return NEX_OK if success, otherwise NEX_FAIL.
     */
    nex_err_t nextion_transparent_data_mode_end(nextion_t *handle);

#ifdef __cplusplus
}
#endif
#endif