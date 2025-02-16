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
     * @brief Install the Nextion driver and creates a Nextion context with the driver.
     * @note UART ISR handler will be attached to the same CPU core that this function is running on.
     * @note It will call "nextion_create".
     * @param[in] uart_num UART port number; any uart_port_t value.
     * @param[in] baud_rate UART baud rate, between NEX_UART_BAUD_RATE_MIN and NEX_UART_BAUD_RATE_MAX.
     * @param[in] tx_io_num UART TX pin GPIO number.
     * @param[in] rx_io_num UART RX pin GPIO number.
     * @return Pointer to a Nextion context or NULL.
     */
    nextion_t *nextion_driver_install(uart_port_t uart_num,
                                      uint32_t baud_rate,
                                      gpio_num_t tx_io_num,
                                      gpio_num_t rx_io_num);

    /**
     * @brief Initialize a Nextion context.
     * @note Will turn the display on.
     * @param[in] handle Nextion context pointer.
     * @return NEX_OK if success, otherwise NEX_FAIL.
     */
    nex_err_t nextion_init(nextion_t *handle);

    /**
     * @brief Delete a Nextion driver and context.
     * @note It will call "nextion_free".
     * @param[in] handle Nextion driver.
     * @return True if success, otherwise false.
     */
    bool nextion_driver_delete(nextion_t *handle);

#ifdef __cplusplus
}
#endif
#endif
