#include "common_infra.h"
#include "config.h"
#include "nextion/base/kernel.h"
#include "nextion/nextion-esp32.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        uart_port_t uart_num; // UART port number.
        bool is_installed;    // If the driver is installed.
    } nextion_driver_t;

    nex_kernel_err_t esp32_nextion_kernel_comm_read(void *caller_object, uint8_t *byte);
    nex_kernel_err_t esp32_nextion_kernel_comm_write(void *caller_object, const uint8_t *buffer, size_t length);

    nex_kernel_err_t (*nextion_kernel_comm_read)(void *, uint8_t *) = &esp32_nextion_kernel_comm_read;
    nex_kernel_err_t (*nextion_kernel_comm_write)(void *, const uint8_t *, size_t) = &esp32_nextion_kernel_comm_write;

    nextion_handle_t nextion_driver_install(uart_port_t uart_num, int baud_rate, int tx_io_num, int rx_io_num)
    {
        NEX_CHECK((baud_rate >= NEX_SERIAL_BAUD_RATE_MIN || baud_rate <= NEX_SERIAL_BAUD_RATE_MAX), "baud_rate error", NULL);

        NEX_LOGI("installing driver on uart: %d", uart_num);

        const uart_config_t uart_config = {
            .baud_rate = baud_rate,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};

        // Do not change the UART initialization order.
        // This order was gotten from: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/uart.html
        // Trying to follow the examples on the github site (https://github.com/espressif/esp-idf/tree/master/examples/peripherals/uart)
        // will lead to error.
        ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));
        ESP_ERROR_CHECK(uart_set_pin(uart_num, tx_io_num, rx_io_num, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
        ESP_ERROR_CHECK(uart_driver_install(uart_num, CONFIG_NEX_UART_RECV_BUFFER_SIZE, 0, CONFIG_NEX_UART_QUEUE_SIZE, NULL, 0));

        nextion_driver_t *handle = (nextion_driver_t *)malloc(sizeof(nextion_driver_t));
        handle->uart_num = uart_num;
        handle->is_installed = true;

        NEX_LOGI("driver installed");

        return nextion_create(handle);
    }

    bool nextion_driver_delete(nextion_handle_t handle)
    {
        NEX_CHECK((handle != NULL), "handle error(NULL)", false);

        NEX_LOGI("deleting driver");

        nextion_driver_t *driver = (nextion_driver_t *)nextion_caller_object_get(handle);

        ESP_ERROR_CHECK(uart_driver_delete(driver->uart_num));

        free(driver);

        nextion_free(handle);

        handle = NULL;

        NEX_LOGI("driver deleted");

        return true;
    }

    nex_kernel_err_t esp32_nextion_kernel_comm_read(void *caller_object, uint8_t *byte)
    {
        int bytes_count = uart_read_bytes(((nextion_driver_t *)caller_object)->uart_num,
                                          byte,
                                          1,
                                          pdMS_TO_TICKS(CONFIG_NEX_RESP_WAIT_TIME_MS));
        if (bytes_count > 0)
            return NEX_KERNEL_OK;

        if (bytes_count == 0)
            return NEX_KERNEL_TIMEOUT;

        return NEX_KERNEL_FAIL;
    }

    nex_kernel_err_t esp32_nextion_kernel_comm_write(void *caller_object, const uint8_t *buffer, size_t length)
    {
        return uart_write_bytes(((nextion_driver_t *)caller_object)->uart_num, (void *)buffer, length) > -1 ? NEX_KERNEL_OK : NEX_KERNEL_FAIL;
    }

#ifdef __cplusplus
}
#endif