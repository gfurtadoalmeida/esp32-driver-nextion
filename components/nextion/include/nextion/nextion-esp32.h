#ifndef __NEXTION_ESP32_H__
#define __NEXTION_ESP32_H__

#include "driver/uart.h"
#include "freertos/queue.h"
#include "nextion/nextion.h"

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
    nextion_handle_t nextion_driver_install(uart_port_t uart_num,
                                            int baud_rate,
                                            int tx_io_num,
                                            int rx_io_num);
    /**
     * @brief Deletes a Nextion driver and context.
     * @note It will call "nextion_free".
     * @param handle Nextion driver.
     * @return True if success, otherwise false.
     */
    bool nextion_driver_delete(nextion_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif //__NEXTION_ESP32_H__