#ifndef __NEXTION_H__
#define __NEXTION_H__

#include "driver/uart.h"
#include "freertos/queue.h"
#include "codes.h"

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
     * @details Will send some commands to the display to configure it.
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
     * @brief Sends a command and waits for a simple ACK.
     * @param command A null-terminated string with the command to be sent.
     * @return NEX_FAIL if error, otherwise any "NEX_DVC_" from "nextion/codes.h"
     */
    nex_err_t nextion_send_command(const char *command);

    /**
     * @brief Sends a command and waits for a response. Do not free the buffer.
     * @note Do not free the buffer.
     * @param command A null-terminated string with the command to be sent.
     * @param response_buffer Pointer of a pointer for a uint8_t array.
     * @return -1 if error, otherwise bytes read.
     */
    int send_command_raw(const char *command, uint8_t **response_buffer);

#ifdef __cplusplus
}
#endif

#endif //__NEXTION_H__