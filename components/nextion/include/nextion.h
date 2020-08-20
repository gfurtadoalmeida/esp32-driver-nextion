#ifndef __NEXTION_H__
#define __NEXTION_H__

#include "driver/uart.h"
#include "freertos/queue.h"
#include "nextion_codes.h"
#include "nextion_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Installs the Nextion driver.
     *
     * @details UART ISR handler will be attached to the same CPU core that this function is running on.
     *
     * @param uart_num UART port number; any uart_port_t value.
     * @param baud_rate UART baud rate, between NEX_UART_BAUD_RATE_MIN and NEX_UART_BAUD_RATE_MAX.
     * @param tx_io_num UART TX pin GPIO number.
     * @param rx_io_num UART RX pin GPIO number.
     * @param queue_size Event queue size/depth.
     * @param[out] event_queue Event queue handle (out param). On success, a new queue handle is written here to provide
     * access to device events. If set to NULL, driver will not use an event queue.
     * @return NEX_OK when success or NEX_FAIL.
     */
    nex_err_t nextion_driver_install(uart_port_t uart_num, int baud_rate, int tx_io_num, int rx_io_num, int queue_size, QueueHandle_t *event_queue);

    /**
     * @brief Does the necessary initialization before any action can be done.
     *
     * @details Will send some commands to the display to configure it.
     *
     * @return NEX_OK when success or NEX_FAIL.
     */
    nex_err_t nextion_driver_init();

    /**
     * @brief Deletes the driver.
     * @return NEX_OK when success or NEX_FAIL.
     */
    nex_err_t nextion_driver_delete();

    /**
     * @brief Checks if the driver is installed.
     * @return True if installed, otherwise false.
     */
    bool nextion_is_driver_installed();

    /**
     * @brief Sends a command and wait for a simple ACK.
     *
     * @param command A null-terminated string with the command to be sent.

     * @return NEX_FAIL if error, otherwise any "NEX_DVC_" from "nextion_codes.h"
     */
    nex_err_t nextion_send_command(const char *command);

    /**
     * @brief Sends a command that retrieves a null-terminated string.
     *
     * @param command A null-terminated string with the command to be sent.
     * @param[in] text Location where the retrieved text will be stored. Must take the null-terminator into account.
     *
     * @return -1 if error, otherwise the text length without the null-terminator.
     */
    int nextion_get_text(const char *command, char *text);

    /**
     * @brief Sends a command that retrieves a signed number.
     *
     * @param command A null-terminated string with the command to be sent.
     * @param[in] number Location where the retrieved number will be stored.
     *
     * @return False if error, otherwise true.
     */
    bool nextion_get_number(const char *command, int *number);

#ifdef __cplusplus
}
#endif

#endif //__NEXTION_H__