#include <malloc.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "nextion/nextion.h"
#include "common_infra.h"
#include "config.h"

#ifdef __cplusplus
extern "C"
{
#endif
#define NEX_CHECK_SEND_COMMAND_HANDLE_STATE(handle)                                                                \
    NEX_CHECK_HANDLE(handle, NEX_FAIL);                                                                            \
    NEX_CHECK((handle->transparent_data_mode_active == false), "state error(in transparent data mode)", NEX_FAIL); \
    NEX_CHECK((handle->is_installed), "driver error(not installed)", NEX_FAIL);                                    \
    NEX_CHECK((handle->is_initialized), "driver error(not initialized)", NEX_FAIL);

    static bool nextion_core_event_process(nextion_handle_t handle, bool lock);
    static bool nextion_core_event_dispatch(nextion_handle_t handle, const uint8_t *buffer, const size_t buffer_length);
    static bool nextion_core_acquire_mutex(nextion_handle_t handle);
    static void nextion_core_release_mutex(nextion_handle_t handle);
    static nex_err_t nextion_core_read_from_uart_as_simple_result(nextion_handle_t handle);
    static int32_t nextion_core_read_from_uart_as_byte(nextion_handle_t handle, uint8_t *buffer, size_t length);
    static bool nextion_core_write_to_uart_as_command(nextion_handle_t handle, const char *format, va_list args);
    static bool nextion_core_write_to_uart_as_byte(nextion_handle_t handle, const char *bytes, size_t length);

    /**
     * @struct nextion_t
     * @brief Holds control data for a context.
     */
    struct nextion_t
    {
        SemaphoreHandle_t uart_mutex;                                          /*!< Mutex used for UART control. */
        nextion_event_callback_t event_callback;                               /*!< Callbacks for events. */
        size_t transparent_data_mode_size;                                     /*!< How many bytes are expected to be written while in "Transparent Data Mode". */
        uart_port_t uart_num;                                                  /*!< UART port number. */
        char command_format_buffer[NEX_UART_TRANS_COMMAND_FORMAT_BUFFER_SIZE]; /*!< Buffer used to format commands. */
        bool is_installed;                                                     /*!< If the driver was installed. */
        bool is_initialized;                                                   /*!< If the driver was initialized. */
        bool transparent_data_mode_active;                                     /*!< If it is in Transparent Data mode. */
    };

    nextion_handle_t nextion_driver_install(uart_port_t uart_num, uint32_t baud_rate, gpio_num_t tx_io_num, gpio_num_t rx_io_num)
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

        nextion_t *driver = (nextion_t *)malloc(sizeof(nextion_t));
        driver->uart_num = uart_num;
        driver->is_installed = true;
        driver->transparent_data_mode_active = false;
        driver->uart_mutex = xSemaphoreCreateMutex();

        NEX_LOGI("driver installed");

        return driver;
    }

    bool nextion_driver_delete(nextion_handle_t handle)
    {
        NEX_CHECK((handle != NULL), "handle error(NULL)", false);

        if (handle->is_installed)
        {
            return true;
        }

        NEX_LOGI("deleting driver");

        ESP_ERROR_CHECK(uart_driver_delete(handle->uart_num));

        free(handle);

        handle = NULL;

        NEX_LOGI("driver deleted");

        return true;
    }

    bool nextion_event_callback_set(nextion_handle_t handle, nextion_event_callback_t event_callback)
    {
        NEX_CHECK_HANDLE(handle, false);

        handle->event_callback = event_callback;

        return true;
    }

    nex_err_t nextion_init(nextion_handle_t handle)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);

        // All logic relies on receiving responses in all cases.

        if (nextion_command_send(handle, "bkcmd=3") != NEX_OK)
        {
            NEX_LOGE("failed initializing display");

            return NEX_FAIL;
        }

        handle->is_initialized = true;

        return NEX_OK;
    }

    nex_err_t nextion_command_send_get_bytes(nextion_handle_t handle, uint8_t *buffer, size_t *length, const char *command, ...)
    {
        NEX_CHECK_SEND_COMMAND_HANDLE_STATE(handle)
        NEX_CHECK((command != NULL), "command error(NULL)", NEX_FAIL);
        NEX_CHECK((nextion_core_acquire_mutex(handle)), "mutex error(not acquired)", NEX_FAIL);
        NEX_CHECK((nextion_core_event_process(handle, false)), "event process error(failed dispatching)", NEX_FAIL);

        nex_err_t code = NEX_OK;
        va_list args;
        va_start(args, command);

        if (!nextion_core_write_to_uart_as_command(handle, command, args))
        {
            NEX_LOGE("failed sending command");

            code = NEX_FAIL;
        }
        else
        {
            *length = nextion_core_read_from_uart_as_byte(handle, buffer, *length);

            if (*length == -1)
            {
                code = NEX_TIMEOUT;
            }
        }

        va_end(args);

        nextion_core_release_mutex(handle);

        return code;
    }

    nex_err_t nextion_command_send_variadic(nextion_handle_t handle, const char *command, va_list args)
    {
        NEX_CHECK_SEND_COMMAND_HANDLE_STATE(handle)
        NEX_CHECK((command != NULL), "command error(NULL)", NEX_FAIL);
        NEX_CHECK((nextion_core_acquire_mutex(handle)), "mutex error(not acquired)", NEX_FAIL);
        NEX_CHECK((nextion_core_event_process(handle, false)), "event process error(failed dispatching)", NEX_FAIL);

        nex_err_t code = NEX_DVC_INSTRUCTION_FAIL;

        if (!nextion_core_write_to_uart_as_command(handle, command, args))
        {
            NEX_LOGE("failed sending command");
        }
        else
        {
            code = nextion_core_read_from_uart_as_simple_result(handle);
        }

        nextion_core_release_mutex(handle);

        if (code == NEX_DVC_INSTRUCTION_FAIL)
        {
            return NEX_FAIL;
        }

        if (code == NEX_DVC_INSTRUCTION_OK)
        {
            return NEX_OK;
        }

        return code;
    }

    nex_err_t nextion_command_send(nextion_handle_t handle, const char *command, ...)
    {
        va_list args;
        va_start(args, command);

        nex_err_t result = nextion_command_send_variadic(handle, command, args);

        va_end(args);

        return result;
    }

    bool nextion_event_process(nextion_handle_t handle)
    {
        NEX_CHECK_HANDLE(handle, false);
        NEX_CHECK((handle->transparent_data_mode_active == false), "state error(in transparent data mode)", false);
        NEX_CHECK((handle->is_installed), "driver error(not installed)", false);
        NEX_CHECK((handle->is_initialized), "driver error(not initialized)", false);

        return nextion_core_event_process(handle, true);
    }

    nex_err_t nextion_transparent_data_mode_begin(nextion_handle_t handle,
                                                  size_t data_size,
                                                  const char *command,
                                                  ...)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);
        NEX_CHECK((command != NULL), "command error(NULL)", NEX_FAIL);
        NEX_CHECK((handle->transparent_data_mode_active == false), "state error(in transparent data mode)", NEX_FAIL);
        NEX_CHECK((data_size > 0), "data_size error(<1)", NEX_FAIL);
        NEX_CHECK((data_size < NEX_DVC_TRANSPARENT_DATA_MAX_DATA_SIZE), "data_size error(>NEX_DVC_TRANSPARENT_DATA_MAX_DATA_SIZE)", NEX_FAIL);

        va_list args;
        va_start(args, command);

        nex_err_t result = nextion_command_send_variadic(handle, command, args);

        va_end(args);

        if (result != NEX_DVC_RSP_TRANSPARENT_DATA_READY)
        {
            return result;
        }

        handle->transparent_data_mode_active = true;
        handle->transparent_data_mode_size = data_size;

        return NEX_OK;
    }

    nex_err_t nextion_transparent_data_mode_write(nextion_handle_t handle, uint8_t value)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);
        NEX_CHECK((handle->transparent_data_mode_active), "state error(not in transparent data mode)", NEX_FAIL);
        NEX_CHECK((handle->transparent_data_mode_size > 0), "state error(all data was written)", NEX_FAIL);

        const uint8_t buffer[1] = {value};

        if (!nextion_core_write_to_uart_as_byte(handle, (char *)buffer, 1))
        {
            NEX_LOGE("failed writing to the communication port");

            return NEX_FAIL;
        }

        handle->transparent_data_mode_size--;

        return NEX_OK;
    }

    nex_err_t nextion_transparent_data_mode_end(nextion_handle_t handle)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);
        NEX_CHECK((handle->transparent_data_mode_active), "state error(not in transparent data mode)", NEX_FAIL);
        NEX_CHECK((handle->transparent_data_mode_size == 0), "state error(not all data was written)", NEX_FAIL);

        uint8_t code = 0;
        uint8_t buffer[NEX_DVC_CMD_ACK_LENGTH];
        size_t bytes_read = nextion_core_read_from_uart_as_byte(handle, buffer, NEX_DVC_CMD_ACK_LENGTH);

        if (bytes_read < 1)
        {
            NEX_LOGE("failed reading response");

            return NEX_FAIL;
        }

        if (bytes_read < NEX_DVC_CMD_ACK_LENGTH)
        {
            NEX_LOGE("invalid response length, expected %d but received %d", NEX_DVC_CMD_ACK_LENGTH, bytes_read);

            return NEX_FAIL;
        }

        code = buffer[0];

        // Here we must have a response message indicating end.

        if (!NEX_DVC_CODE_IS_RESPONSE(code, bytes_read))
        {
            NEX_LOGE("response code not expected");

            return NEX_FAIL;
        }

        if (code != NEX_DVC_RSP_TRANSPARENT_DATA_FINISHED)
        {
            NEX_LOGE("response code is not data finished");

            return NEX_FAIL;
        }

        handle->transparent_data_mode_active = false;

        return NEX_OK;
    }

    /* ======================
     *     Core Methods
    ======================== */

    /**
     * @brief Dispatches an event to a callback.
     * @param handle Nextion context pointer.
     * @param buffer Buffer containing event data.
     * @param buffer_length Buffer length.
     * @return True if success, otherwise, false.
     */
    bool nextion_core_event_dispatch(nextion_handle_t handle, const uint8_t *buffer, const size_t buffer_length)
    {
        NEX_CHECK((buffer_length >= NEX_DVC_CMD_ACK_LENGTH), "buffer length error(<NEX_DVC_CMD_ACK_LENGTH)", false)

        const uint8_t code = buffer[0];

        switch (code)
        {
        case NEX_DVC_EVT_TOUCH_OCCURRED:
            if (buffer_length == 7 && handle->event_callback.on_touch != NULL)
            {
                handle->event_callback.on_touch(handle, buffer[1], buffer[2], buffer[3]);
            }
            break;

        case NEX_DVC_EVT_TOUCH_COORDINATE_AWAKE:
        case NEX_DVC_EVT_TOUCH_COORDINATE_ASLEEP:
            if (buffer_length == 9 && handle->event_callback.on_touch_coord != NULL)
            {
                // Coordinates: 2 bytes and unsigned = uint16_t.
                // Sent in big endian format.

                handle->event_callback.on_touch_coord(handle,
                                                      (uint16_t)(((uint16_t)buffer[1] << 8) | (uint16_t)buffer[2]),
                                                      (uint16_t)(((uint16_t)buffer[3] << 8) | (uint16_t)buffer[4]),
                                                      buffer[5],
                                                      code == NEX_DVC_EVT_TOUCH_COORDINATE_ASLEEP);
            }
            break;

        default:
            if (handle->event_callback.on_device != NULL)
            {
                handle->event_callback.on_device(handle, code);
            }
        }

        return true;
    }

    bool nextion_core_event_process(nextion_handle_t handle, bool lock)
    {
        if (lock)
        {
            if (!nextion_core_acquire_mutex(handle))
            {
                NEX_LOGE("failed acquiring lock");

                return false;
            }
        }

        uint8_t buffer[NEX_DVC_EVT_MAX_RESPONSE_LENGTH];
        uint32_t bytes_read = nextion_core_read_from_uart_as_byte(handle, buffer, NEX_DVC_EVT_MAX_RESPONSE_LENGTH);

        while (bytes_read > -1)
        {
            if (!NEX_DVC_CODE_IS_EVENT(buffer[0], bytes_read))
            {
                NEX_LOGW("response was not an event, some data might be corrupted");

                break;
            }

            if (!nextion_core_event_dispatch(handle, buffer, bytes_read))
            {
                NEX_LOGW("failure dispatching event");

                break;
            }

            bytes_read = nextion_core_read_from_uart_as_byte(handle, buffer, NEX_DVC_EVT_MAX_RESPONSE_LENGTH);
        };

        if (lock)
        {
            nextion_core_release_mutex(handle);
        }

        return true;
    }

    bool nextion_core_acquire_mutex(nextion_handle_t handle)
    {
        return xSemaphoreTake(handle->uart_mutex, pdMS_TO_TICKS(CONFIG_NEX_UART_MUTEX_WAIT_TIME_MS)) == pdTRUE;
    }

    void nextion_core_release_mutex(nextion_handle_t handle)
    {
        xSemaphoreGive(handle->uart_mutex);
    }

    nex_err_t nextion_core_read_from_uart_as_simple_result(nextion_handle_t handle)
    {
        uint8_t buffer[NEX_DVC_CMD_ACK_LENGTH];

        int bytes_read = nextion_core_read_from_uart_as_byte(handle, buffer, NEX_DVC_CMD_ACK_LENGTH);

        if (bytes_read < 1)
        {
            // Some commands only return data on failure.
            // Event processing will throw this too when no event response is found.
            // That's why this is a debug; too much noise.

            NEX_LOGD("response timed out");

            return NEX_TIMEOUT;
        }

        if (bytes_read != NEX_DVC_CMD_ACK_LENGTH)
        {
            NEX_LOGE("invalid response size, expected %d but received %d", NEX_DVC_CMD_ACK_LENGTH, bytes_read);

            return NEX_DVC_INSTRUCTION_FAIL;
        }

        return buffer[0];
    }

    int32_t nextion_core_read_from_uart_as_byte(nextion_handle_t handle, uint8_t *buffer, size_t length)
    {
        int bytes_read = uart_read_bytes(handle->uart_num, buffer, length, pdMS_TO_TICKS(CONFIG_NEX_UART_RECV_WAIT_TIME_MS));

        if (bytes_read < 1)
        {
            // Some commands only return data on failure.
            // Event processing will throw this too when no event response is found.
            // That's why this is a debug; too much noise.

            NEX_LOGD("response timed out");

            return -1;
        }

        return bytes_read;
    }

    bool nextion_core_write_to_uart_as_command(nextion_handle_t handle, const char *format, va_list args)
    {
        static const char END_SEQUENCE[NEX_DVC_CMD_END_LENGTH] = {NEX_DVC_CMD_END_SEQUENCE};

        int size = vsnprintf(handle->command_format_buffer, NEX_UART_TRANS_COMMAND_FORMAT_BUFFER_SIZE, format, args);

        uart_port_t uart = handle->uart_num;

        if (uart_write_bytes(uart, handle->command_format_buffer, size) < 0 || uart_write_bytes(uart, END_SEQUENCE, NEX_DVC_CMD_END_LENGTH) < 0)
        {
            NEX_LOGE("failed writing command");

            return false;
        }

        if (uart_wait_tx_done(uart, pdMS_TO_TICKS(CONFIG_NEX_UART_TRANS_WAIT_TIME_MS)) != ESP_OK)
        {
            NEX_LOGE("failed waiting transmission");

            return false;
        }

        return true;
    }

    bool nextion_core_write_to_uart_as_byte(nextion_handle_t handle, const char *bytes, size_t length)
    {
        uart_port_t uart = handle->uart_num;

        if (uart_write_bytes(uart, bytes, length) < 0)
        {
            NEX_LOGE("failed writing command");

            return false;
        }

        if (uart_wait_tx_done(uart, pdMS_TO_TICKS(CONFIG_NEX_UART_TRANS_WAIT_TIME_MS)) != ESP_OK)
        {
            NEX_LOGE("failed waiting transmission");

            return false;
        }

        return true;
    }

#ifdef __cplusplus
}
#endif
