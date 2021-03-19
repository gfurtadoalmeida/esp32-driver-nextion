#include "common_infra.h"
#include "config.h"
#include "nextion/nextion-esp32.h"

#ifdef __cplusplus
extern "C"
{
#endif

    nex_comm_err_t _comm_init(nextion_comm_t *self);
    nex_comm_err_t _comm_free(nextion_comm_t *self);
    nex_comm_err_t _comm_read(nextion_comm_t *self, uint8_t *byte);
    nex_comm_err_t _comm_write(nextion_comm_t *self, const uint8_t *buffer, size_t buffer_length);

    static nextion_comm_vtable_t _vtable = {
        _comm_init,
        _comm_free,
        _comm_read,
        _comm_write};

    typedef struct
    {
        nextion_comm_t comm;
        uart_port_t uart_num; // UART port number.
        bool is_installed;    // If the driver is installed.
    } nextion_driver_t;

    nextion_handle_t nextion_driver_install(uart_port_t uart_num, int baud_rate, int tx_io_num, int rx_io_num)
    {
        NEX_CHECK((baud_rate >= NEX_SERIAL_BAUD_RATE_MIN || baud_rate <= NEX_SERIAL_BAUD_RATE_MAX), "baud_rate error", NULL);

        NEX_LOGI("installing driver on uart %d with baud rate %d", uart_num, baud_rate);

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
        ESP_ERROR_CHECK(uart_driver_install(uart_num, CONFIG_NEX_UART_RECV_BUFFER_SIZE, 0, 0, NULL, 0));

        nextion_driver_t *driver = (nextion_driver_t *)malloc(sizeof(nextion_driver_t));
        driver->uart_num = uart_num;
        driver->is_installed = true;
        driver->comm.vtable = &_vtable;

        NEX_LOGI("driver installed");

        return nextion_create((nextion_comm_t *)driver);
    }

    bool nextion_driver_delete(nextion_handle_t handle)
    {
        NEX_CHECK((handle != NULL), "handle error(NULL)", false);

        NEX_LOGI("deleting driver");

        nextion_driver_t *driver = (nextion_driver_t *)nextion_comm_get(handle);

        ESP_ERROR_CHECK(uart_driver_delete(driver->uart_num));

        nextion_free(handle);

        free(driver);

        handle = NULL;

        NEX_LOGI("driver deleted");

        return true;
    }

    nex_comm_err_t _comm_init(nextion_comm_t *self)
    {
        return NEX_COMM_OK;
    }

    nex_comm_err_t _comm_free(nextion_comm_t *self)
    {
        return NEX_COMM_OK;
    }

    nex_comm_err_t _comm_read(nextion_comm_t *self, uint8_t *byte)
    {
        int bytes_count = uart_read_bytes(((nextion_driver_t *)self)->uart_num,
                                          byte,
                                          1,
                                          pdMS_TO_TICKS(CONFIG_NEX_UART_RECV_WAIT_TIME_MS));
        if (bytes_count > 0)
            return NEX_COMM_OK;

        if (bytes_count == 0)
            return NEX_COMM_TIMEOUT;

        return NEX_COMM_FAIL;
    }

    nex_comm_err_t _comm_write(nextion_comm_t *self, const uint8_t *buffer, size_t buffer_length)
    {
        uart_port_t port = ((nextion_driver_t *)self)->uart_num;

        if (uart_write_bytes(port, (void *)buffer, buffer_length) > -1)
        {
            if (uart_wait_tx_done(port, pdMS_TO_TICKS(CONFIG_NEX_UART_TRANS_WAIT_TIME_MS)) != ESP_OK)
            {
                NEX_LOGE("Failure on waiting for transmission completion");

                return NEX_COMM_FAIL;
            }

            return NEX_COMM_OK;
        }

        NEX_LOGE("No bytes were written to the communication port");

        return NEX_COMM_FAIL;
    }

#ifdef __cplusplus
}
#endif