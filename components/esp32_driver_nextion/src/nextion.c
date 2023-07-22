#include <malloc.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp32_driver_nextion/nextion.h"
#include "esp32_driver_nextion/system.h"
#include "assertion.h"
#include "config.h"

#define CMP_CHECK_SEND_COMMAND_HANDLE_STATE(handle)                                \
    CMP_CHECK_HANDLE(handle, NEX_FAIL)                                             \
    CMP_CHECK((handle->is_installed), "driver error(not installed)", NEX_FAIL)     \
    CMP_CHECK((handle->is_initialized), "driver error(not initialized)", NEX_FAIL) \
    CMP_CHECK((handle->in_transparent_data_mode == false), "state error(in transparent data mode)", NEX_FAIL)

static bool nextion_core_command_sync_acquire(nextion_t *handle, TickType_t timeout);
static void nextion_core_command_sync_release(nextion_t *handle);
static bool nextion_core_event_dispatch(nextion_t *handle, const uint8_t *buffer, const size_t buffer_length);
static bool nextion_core_event_process(nextion_t *handle);
static void nextion_core_uart_task(void *pvParameters);
static int32_t nextion_core_uart_read_as_byte(const nextion_t *handle, uint8_t *buffer, size_t length);
static nex_err_t nextion_core_uart_read_as_simple_result(nextion_t *handle);
static bool nextion_core_uart_write_as_byte(const nextion_t *handle, const char *bytes, size_t length);
static bool nextion_core_uart_write_as_command(nextion_t *handle, const char *format, va_list args);

/**
 * @struct nextion_t
 * @brief Holds control data for a context.
 */
struct nextion_t
{
    char command_format_buffer[CONFIG_NEX_UART_TRANS_COMMAND_FORMAT_BUFFER_SIZE]; /*!< Buffer used for formating commands. */
    event_callback_on_touch event_callback_on_touch;                              /*!< Callbacks for 'on touch' events. */
    event_callback_on_touch_coord event_callback_on_touch_coord;                  /*!< Callbacks for 'on touch with coordinates' events. */
    event_callback_on_device event_callback_on_device;                            /*!< Callbacks for 'on device' events. */
    SemaphoreHandle_t command_sync;                                               /*!< Mutex used command control. */
    QueueHandle_t uart_queue;                                                     /*!< Queue used for UART event. */
    TaskHandle_t uart_task;                                                       /*!< Task used for UART queue handling. */
    size_t transparent_data_mode_size;                                            /*!< How many bytes are expected to be written while in "Transparent Data Mode". */
    uart_port_t uart_num;                                                         /*!< UART port number. */
    bool is_installed;                                                            /*!< If the driver was installed. */
    bool is_initialized;                                                          /*!< If the driver was initialized. */
    bool in_transparent_data_mode;                                                /*!< If it is in Transparent Data mode. */
};

nextion_t *nextion_driver_install(uart_port_t uart_num, uint32_t baud_rate, gpio_num_t tx_io_num, gpio_num_t rx_io_num)
{
    CMP_CHECK((baud_rate >= NEX_SERIAL_BAUD_RATE_MIN || baud_rate <= NEX_SERIAL_BAUD_RATE_MAX), "baud_rate error", NULL)

    CMP_LOGI("installing driver on uart %d with baud rate %lu", uart_num, baud_rate);

    const uart_config_t uart_config = {
        .baud_rate = (int)baud_rate,
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

    nextion_t *driver = (nextion_t *)calloc(1, sizeof(nextion_t));
    driver->uart_num = uart_num;
    driver->is_installed = true;
    driver->is_initialized = false;
    driver->in_transparent_data_mode = false;
    driver->command_sync = xSemaphoreCreateBinary();

    ESP_ERROR_CHECK(uart_driver_install(uart_num,
                                        CONFIG_NEX_UART_RECV_BUFFER_SIZE, // Receive buffer size.
                                        0,                                // Transmit buffer size.
                                        10,                               // Queue size.
                                        &driver->uart_queue,              // Queue pointer.
                                        0));                              // Allocation flags.

    if (xTaskCreate(&nextion_core_uart_task,
                    "nextion",
                    2048,
                    (void *)driver,
                    CONFIG_NEX_UART_TASK_PRIORITY,
                    &driver->uart_task) != pdPASS)
    {
        CMP_LOGE("failed creating UART event task");

        abort();
    }

    CMP_LOGI("driver installed");

    return driver;
}

bool nextion_driver_delete(nextion_t *handle)
{
    CMP_CHECK((handle != NULL), "handle error(NULL)", false)

    if (!handle->is_installed)
    {
        return true;
    }

    nextion_core_command_sync_release(handle);

    CMP_LOGI("deleting driver");

    vTaskDelete(handle->uart_task);

    // Will also free the queue.
    ESP_ERROR_CHECK(uart_driver_delete(handle->uart_num));

    vSemaphoreDelete(handle->command_sync);

    free(handle);

    handle = NULL;

    CMP_LOGI("driver deleted");

    return true;
}

bool nextion_event_callback_set_on_touch(nextion_t *handle, event_callback_on_touch callback)
{
    CMP_CHECK_HANDLE(handle, false)

    handle->event_callback_on_touch = callback;

    return true;
}

bool nextion_event_callback_set_on_touch_coord(nextion_t *handle, event_callback_on_touch_coord callback)
{
    CMP_CHECK_HANDLE(handle, false)

    handle->event_callback_on_touch_coord = callback;

    return true;
}

bool nextion_event_callback_set_on_device(nextion_t *handle, event_callback_on_device callback)
{
    CMP_CHECK_HANDLE(handle, false)

    handle->event_callback_on_device = callback;

    return true;
}

nex_err_t nextion_init(nextion_t *handle)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    // As "nextion_command_send" validates if the driver is
    // initialized, we need to cheat here.
    handle->is_initialized = true;

    // Set up the semaphore.
    nextion_core_command_sync_release(handle);

    // Resume the UART task.
    vTaskResume(handle->uart_task);

    // As "bkcmd" is not set, we cannot garantee what will come.
    // Just try to wake up, as the device cannot receive commands
    // when sleeping. Any failure will come when setting "bkcmd".
    nextion_system_wakeup(handle);

    // All logic relies on receiving responses at all times.
    if (nextion_command_send(handle, "bkcmd=3") != NEX_OK)
    {
        handle->is_initialized = false;

        vTaskSuspend(handle->uart_task);

        CMP_LOGE("failed initializing display");

        return NEX_FAIL;
    }

    CMP_LOGI("driver initialized");

    return NEX_OK;
}

nex_err_t nextion_command_send_get_bytes(nextion_t *handle, uint8_t *buffer, size_t *length, const char *command, ...)
{
    CMP_CHECK_SEND_COMMAND_HANDLE_STATE(handle)
    CMP_CHECK((command != NULL), "command error(NULL)", NEX_FAIL)
    CMP_CHECK((nextion_core_command_sync_acquire(handle, pdMS_TO_TICKS(CONFIG_NEX_UART_MUTEX_WAIT_TIME_MS))), "sync error(not acquired)", NEX_FAIL)

    nex_err_t code = NEX_OK;
    va_list args;
    va_start(args, command);

    if (!nextion_core_uart_write_as_command(handle, command, args))
    {
        CMP_LOGE("failed sending command");

        code = NEX_FAIL;
    }
    else
    {
        *length = (int)nextion_core_uart_read_as_byte(handle, buffer, *length);

        if (*length == -1)
        {
            code = NEX_TIMEOUT;
        }
    }

    nextion_core_command_sync_release(handle);

    va_end(args);

    return code;
}

nex_err_t nextion_command_send_variadic(nextion_t *handle, const char *command, va_list args)
{
    CMP_CHECK_SEND_COMMAND_HANDLE_STATE(handle)
    CMP_CHECK((command != NULL), "command error(NULL)", NEX_FAIL)
    CMP_CHECK((nextion_core_command_sync_acquire(handle, pdMS_TO_TICKS(CONFIG_NEX_UART_MUTEX_WAIT_TIME_MS))), "sync error(not acquired)", NEX_FAIL)

    nex_err_t code = NEX_DVC_INSTRUCTION_FAIL;

    if (!nextion_core_uart_write_as_command(handle, command, args))
    {
        CMP_LOGE("failed sending command");
    }
    else
    {
        code = nextion_core_uart_read_as_simple_result(handle);
    }

    nextion_core_command_sync_release(handle);

    if (code == NEX_DVC_INSTRUCTION_FAIL)
    {
        CMP_LOGW("device returned failure");

        return NEX_FAIL;
    }

    if (code == NEX_DVC_INSTRUCTION_OK)
    {
        return NEX_OK;
    }

    return code;
}

nex_err_t nextion_command_send(nextion_t *handle, const char *command, ...)
{
    va_list args;
    va_start(args, command);

    nex_err_t result = nextion_command_send_variadic(handle, command, args);

    va_end(args);

    return result;
}

nex_err_t nextion_transparent_data_mode_begin(nextion_t *handle,
                                              size_t data_size,
                                              const char *command,
                                              ...)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK((command != NULL), "command error(NULL)", NEX_FAIL)
    CMP_CHECK((handle->in_transparent_data_mode == false), "state error(in transparent data mode)", NEX_FAIL)
    CMP_CHECK((data_size > 0), "data_size error(<1)", NEX_FAIL)
    CMP_CHECK((data_size < NEX_DVC_TRANSPARENT_DATA_MAX_DATA_SIZE), "data_size error(>NEX_DVC_TRANSPARENT_DATA_MAX_DATA_SIZE)", NEX_FAIL)

    va_list args;
    va_start(args, command);

    nex_err_t result = nextion_command_send_variadic(handle, command, args);

    va_end(args);

    if (result != NEX_DVC_RSP_TRANSPARENT_DATA_READY)
    {
        return result;
    }

    handle->in_transparent_data_mode = true;
    handle->transparent_data_mode_size = data_size;

    return NEX_OK;
}

nex_err_t nextion_transparent_data_mode_write(nextion_t *handle, uint8_t value)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK((handle->in_transparent_data_mode), "state error(not in transparent data mode)", NEX_FAIL)
    CMP_CHECK((handle->transparent_data_mode_size > 0), "state error(all data was written)", NEX_FAIL)

    const uint8_t buffer[1] = {value};

    if (!nextion_core_uart_write_as_byte(handle, (const char *)buffer, 1))
    {
        CMP_LOGE("failed writing to the communication port");

        return NEX_FAIL;
    }

    handle->transparent_data_mode_size--;

    return NEX_OK;
}

nex_err_t nextion_transparent_data_mode_end(nextion_t *handle)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK((handle->in_transparent_data_mode), "state error(not in transparent data mode)", NEX_FAIL)
    CMP_CHECK((handle->transparent_data_mode_size == 0), "state error(not all data was written)", NEX_FAIL)

    uint8_t code = 0;
    uint8_t buffer[NEX_DVC_CMD_ACK_LENGTH];
    size_t bytes_read = (size_t)nextion_core_uart_read_as_byte(handle, buffer, NEX_DVC_CMD_ACK_LENGTH);

    if (bytes_read < 1)
    {
        CMP_LOGE("failed reading response");

        return NEX_FAIL;
    }

    if (bytes_read < NEX_DVC_CMD_ACK_LENGTH)
    {
        CMP_LOGE("invalid response length, expected %d but received %d", NEX_DVC_CMD_ACK_LENGTH, bytes_read);

        return NEX_FAIL;
    }

    code = buffer[0];

    // Here we must have a response message indicating end.

    if (!NEX_DVC_CODE_IS_RESPONSE(code, bytes_read))
    {
        CMP_LOGE("response code not expected");

        return NEX_FAIL;
    }

    if (code != NEX_DVC_RSP_TRANSPARENT_DATA_FINISHED)
    {
        CMP_LOGE("response code is not data finished");

        return NEX_FAIL;
    }

    handle->in_transparent_data_mode = false;

    return NEX_OK;
}

/* ======================
 *     Core Methods
 *======================= */

static void nextion_core_uart_task(void *pvParameters)
{
    vTaskSuspend(NULL);

    nextion_t *handle = (nextion_t *)pvParameters;
    QueueHandle_t queue = handle->uart_queue;
    uart_port_t uart = handle->uart_num;

    CMP_LOGI("installed queue for UART %d", uart);

    uart_event_t event;

    for (;;)
    {
        if (xQueueReceive(queue, (void *)&event, portMAX_DELAY) == pdFALSE)
        {
            CMP_LOGE("failed receiving from UART queue");
            break;
        }

        switch (event.type)
        {
        case UART_DATA:
            CMP_LOGD("UART data size: %d", event.size);

            // If we can acquire a semaphore it means the event was sent by the device
            // automatically. It will only fail to acquire the semaphore when a command
            // was called. For commands and Transparent Data mode, we do nothing,
            // let them handle the bytes.

            if (!handle->in_transparent_data_mode && nextion_core_command_sync_acquire(handle, 0))
            {
                CMP_LOGD("processing events");

                nextion_core_event_process(handle);
                nextion_core_command_sync_release(handle);
            }
            break;
        case UART_FIFO_OVF:
            CMP_LOGW("UART hw fifo overflow");

            uart_flush_input(uart);

            xQueueReset(queue);
            break;
        case UART_BUFFER_FULL:
            CMP_LOGW("UART buffer full");

            uart_flush_input(uart);

            xQueueReset(queue);
            break;
        default:
            break;
        }
    }
}

static bool nextion_core_event_process(nextion_t *handle)
{
    CMP_CHECK_HANDLE(handle, false)
    CMP_CHECK((handle->in_transparent_data_mode == false), "state error(in transparent data mode)", false)
    CMP_CHECK((handle->is_installed), "driver error(not installed)", false)
    CMP_CHECK((handle->is_initialized), "driver error(not initialized)", false)

    uint8_t buffer[NEX_DVC_EVT_MAX_RESPONSE_LENGTH];
    int bytes_read = (int)nextion_core_uart_read_as_byte(handle, buffer, NEX_DVC_EVT_MAX_RESPONSE_LENGTH);

    while (bytes_read > -1)
    {
        CMP_LOGD("parsed event %d with size %d", buffer[0], bytes_read);

        if (!NEX_DVC_CODE_IS_EVENT(buffer[0], bytes_read))
        {
            CMP_LOGW("response code %d was not an event, some data might be corrupted", buffer[0]);

            return false;
        }

        if (!nextion_core_event_dispatch(handle, buffer, bytes_read))
        {
            CMP_LOGW("failure dispatching event from event handler");

            return false;
        }

        bytes_read = (int)nextion_core_uart_read_as_byte(handle, buffer, NEX_DVC_EVT_MAX_RESPONSE_LENGTH);
    }

    return true;
}

/**
 * @brief Dispatches an event to a callback.
 * @param handle Nextion context pointer.
 * @param buffer Buffer containing event data.
 * @param buffer_length Buffer length.
 * @return True if success, otherwise, false.
 */
static bool nextion_core_event_dispatch(nextion_t *handle, const uint8_t *buffer, const size_t buffer_length)
{
    CMP_CHECK((buffer_length >= NEX_DVC_CMD_ACK_LENGTH), "buffer length error(<NEX_DVC_CMD_ACK_LENGTH)", false)

    const uint8_t code = buffer[0];

    CMP_LOGD("preparing event %d", code);

    switch (code)
    {
    case NEX_DVC_EVT_TOUCH_OCCURRED:
        if (buffer_length == 7 && handle->event_callback_on_touch != NULL)
        {
            nextion_on_touch_event_t event = {
                .handle = handle,
                .page_id = buffer[1],
                .component_id = buffer[2],
                .state = buffer[3]};

            CMP_LOGD("dispatching 'on touch' event");

            handle->event_callback_on_touch(event);
        }
        break;
    case NEX_DVC_EVT_TOUCH_COORDINATE_AWAKE:
    case NEX_DVC_EVT_TOUCH_COORDINATE_ASLEEP:
        if (handle->event_callback_on_touch_coord != NULL && buffer_length == 9)
        {
            // Coordinates: 2 bytes and unsigned = uint16_t.
            // Sent in big endian format.
            nextion_on_touch_coord_event_t event = {
                .handle = handle,
                .x = (uint16_t)(((uint16_t)buffer[1] << 8) | (uint16_t)buffer[2]),
                .y = (uint16_t)(((uint16_t)buffer[3] << 8) | (uint16_t)buffer[4]),
                .state = buffer[5],
                .exited_sleep = code == NEX_DVC_EVT_TOUCH_COORDINATE_ASLEEP};

            CMP_LOGD("dispatching 'on touch coord' event");

            handle->event_callback_on_touch_coord(event);
        }
        break;
    default:
        if (handle->event_callback_on_device != NULL)
        {
            nextion_on_device_event_t event = {
                .handle = handle,
                .state = code};

            CMP_LOGD("dispatching 'on device' event");

            handle->event_callback_on_device(event);
        }
    }

    return true;
}

static bool nextion_core_command_sync_acquire(nextion_t *handle, TickType_t timeout)
{
    return xSemaphoreTake(handle->command_sync, timeout) == pdTRUE;
}

static void nextion_core_command_sync_release(nextion_t *handle)
{
    xSemaphoreGive(handle->command_sync);
}

static nex_err_t nextion_core_uart_read_as_simple_result(nextion_t *handle)
{
    uint8_t buffer[NEX_DVC_EVT_MAX_RESPONSE_LENGTH];

    int bytes_read = 0;

    do
    {
        bytes_read = (int)nextion_core_uart_read_as_byte(handle, buffer, NEX_DVC_EVT_MAX_RESPONSE_LENGTH);

        if (bytes_read == -1)
        {
            // Some commands only return data on failure.
            // Event processing will throw this too when no event response is found.
            // That's why this is a debug; too much noise.

            CMP_LOGD("response timed out");

            return NEX_TIMEOUT;
        }

        // Depending on how much device events we're receiving,
        // it may happen that an event response will be read.
        // Dispatch it and read the next response until ours
        // is found.

        if (NEX_DVC_CODE_IS_EVENT(buffer[0], bytes_read))
        {
            CMP_LOGW("parsed the event %d on command handler", buffer[0]);

            if (!nextion_core_event_dispatch(handle, buffer, bytes_read))
            {
                CMP_LOGW("failure dispatching event %d from command handler", buffer[0]);
            }

            continue;
        }

        break;
    } while (true);

    if (bytes_read != NEX_DVC_CMD_ACK_LENGTH)
    {
        CMP_LOGE("invalid response size, expected %d but received %d", NEX_DVC_CMD_ACK_LENGTH, bytes_read);

        return NEX_DVC_INSTRUCTION_FAIL;
    }

    return buffer[0];
}

static int32_t nextion_core_uart_read_as_byte(const nextion_t *handle, uint8_t *buffer, size_t length)
{
    uint8_t *movable_buffer = buffer;
    int ends_found = 0; // Some commands use NEX_DVC_CMD_END_LENGTH chars as end response.
    int bytes_read = 0;
    int result = 0;

    for (size_t i = 0; i < length; i++)
    {
        result = uart_read_bytes(handle->uart_num, movable_buffer, 1, pdMS_TO_TICKS(CONFIG_NEX_UART_RECV_WAIT_TIME_MS));

        if (result > 0) // We got something.
        {
            if (buffer[i] == NEX_DVC_CMD_END_VALUE)
            {
                ends_found++;
            }

            movable_buffer++;
            bytes_read++;

            // Stop when finding a command ending.
            // Not every read byte request will have a
            // command ending.
            if (ends_found == NEX_DVC_CMD_END_LENGTH)
            {
                break;
            }

            continue;
        }

        if (result == 0) // It's a timeout.
        {
            // If we have read something before the timeout,
            // we do not send an error. The "length" parameter
            // sometimes is an estimation; let the caller decide
            // if there is enough data or not.

            if (bytes_read > 0)
            {
                return bytes_read;
            }

            // Some commands only return data on failure.
            // Event processing will throw this too when no event response is found.
            // That's why this is a debug; too much noise.

            CMP_LOGD("response timed out");

            return -1;
        }

        if (result == -1) // UART error.
        {
            CMP_LOGE("failed reading UART");
            return -1;
        }
    }

    return bytes_read;
}

static bool nextion_core_uart_write_as_command(nextion_t *handle, const char *format, va_list args)
{
    const char END_SEQUENCE[NEX_DVC_CMD_END_LENGTH] = {NEX_DVC_CMD_END_SEQUENCE};

    int size = vsnprintf(handle->command_format_buffer, CONFIG_NEX_UART_TRANS_COMMAND_FORMAT_BUFFER_SIZE, format, args);

    uart_port_t uart = handle->uart_num;

    if (uart_write_bytes(uart, handle->command_format_buffer, size) < 0 || uart_write_bytes(uart, END_SEQUENCE, NEX_DVC_CMD_END_LENGTH) < 0)
    {
        CMP_LOGE("failed writing command");

        return false;
    }

    if (uart_wait_tx_done(uart, pdMS_TO_TICKS(CONFIG_NEX_UART_TRANS_WAIT_TIME_MS)) != ESP_OK)
    {
        CMP_LOGE("failed waiting transmission");

        return false;
    }

    return true;
}

static bool nextion_core_uart_write_as_byte(const nextion_t *handle, const char *bytes, size_t length)
{
    uart_port_t uart = handle->uart_num;

    if (uart_write_bytes(uart, bytes, length) < 0)
    {
        CMP_LOGE("failed writing command");

        return false;
    }

    if (uart_wait_tx_done(uart, pdMS_TO_TICKS(CONFIG_NEX_UART_TRANS_WAIT_TIME_MS)) != ESP_OK)
    {
        CMP_LOGE("failed waiting transmission");

        return false;
    }

    return true;
}
