#include <malloc.h>
#include <stdarg.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp32_driver_nextion/base/events.h"
#include "esp32_driver_nextion/nextion.h"
#include "esp32_driver_nextion/system.h"
#include "protocol/parsers/responses/ack.h"
#include "protocol/protocol.h"
#include "protocol/event.h"
#include "assertion.h"
#include "config.h"

#define CMP_CHECK_SEND_COMMAND_HANDLE_STATE(handle)                            \
    CMP_CHECK_HANDLE(handle, NEX_FAIL)                                         \
    CMP_CHECK((handle->is_installed), "driver error(not installed)", NEX_FAIL) \
    CMP_CHECK((handle->is_initialized), "driver error(not initialized)", NEX_FAIL)

#define PROCESS_SYNC_TAKE(handle, timeout) (xSemaphoreTake(handle->command_sync, timeout) == pdTRUE)
#define PROCESS_SYNC_GIVE(handle) xSemaphoreGive(handle->command_sync)

static nex_err_t nextion_core_process_response(nextion_t *handle, const parser_t *parser);
static void nextion_core_process_events(nextion_t *handle);
static void nextion_core_uart_task(void *pvParameters);

/**
 * @struct nextion_t
 * @brief Holds control data for a context.
 */
struct nextion_t
{
    uint8_t uart_buffer[64];                                                 /*!< Buffer to receive UART data. */
    uint8_t format_buffer[CONFIG_NEX_UART_TRANS_COMMAND_FORMAT_BUFFER_SIZE]; /*!< Buffer to format instructions. */
    uint8_t event_buffer[EVENT_PARSE_BUFFER_SIZE];                           /*!< Buffer to parse events. */
    SemaphoreHandle_t command_sync;                                          /*!< Mutex used command control. */
    QueueHandle_t uart_queue;                                                /*!< Queue used for UART event. */
    TaskHandle_t uart_task;                                                  /*!< Task used for UART queue handling. */
    uart_port_t uart_num;                                                    /*!< UART port number. */
    bool is_installed;                                                       /*!< If the driver was installed. */
    bool is_initialized;                                                     /*!< If the driver was initialized. */
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

nex_err_t nextion_init(nextion_t *handle)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    // As "nextion_command_send" validates if the driver is
    // initialized, we need to cheat here.
    handle->is_initialized = true;

    // Set up the semaphore.
    PROCESS_SYNC_GIVE(handle);

    // Resume the UART task.
    vTaskResume(handle->uart_task);

    // Just try to wake up, as the device cannot
    // receive commands when sleeping.
    if (nextion_system_wakeup(handle) != NEX_OK)
    {
        handle->is_initialized = false;

        vTaskSuspend(handle->uart_task);

        CMP_LOGE("failed waking up device");

        return NEX_FAIL;
    }

    CMP_LOGI("driver initialized");

    return NEX_OK;
}

bool nextion_driver_delete(nextion_t *handle)
{
    CMP_CHECK((handle != NULL), "handle error(NULL)", false)

    if (!handle->is_installed)
    {
        return true;
    }

    PROCESS_SYNC_GIVE(handle);

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

//
// Protocol
//

bool nextion_protocol_format_instruction(nextion_t *handle, formated_instruction_t *formated_instruction, const char *instruction, ...)
{
    va_list args;
    va_start(args, instruction);

    bool result = nextion_protocol_format_instruction_variadic(handle, formated_instruction, instruction, args);

    va_end(args);

    return result;
}

bool nextion_protocol_format_instruction_variadic(nextion_t *handle,
                                                  formated_instruction_t *formated_instruction,
                                                  const char *instruction,
                                                  va_list args)
{
    int result = vsnprintf((char *)handle->format_buffer, sizeof(handle->format_buffer), instruction, args);

    if (result > sizeof(handle->format_buffer))
    {
        CMP_LOGE("format buffer insufficient: needed %d, has %d", result, sizeof(handle->format_buffer));

        return false;
    }

    formated_instruction->text = (char *)handle->format_buffer;
    formated_instruction->length = result;

    return true;
}

nex_err_t nextion_protocol_send_instruction(nextion_t *handle, const char *instruction, size_t instruction_length, const parser_t *parser)
{
    CMP_CHECK_SEND_COMMAND_HANDLE_STATE(handle)
    CMP_CHECK((instruction != NULL), "instruction error(NULL)", NEX_FAIL)
    CMP_CHECK((PROCESS_SYNC_TAKE(handle, pdMS_TO_TICKS(CONFIG_NEX_UART_MUTEX_WAIT_TIME_MS))), "sync error(not acquired)", NEX_FAIL)

    // Process any event remaining in the buffer
    // before sending a command, so we can be sure
    // that the data received is a command response.
    nextion_core_process_events(handle);

    const char END_SEQUENCE[NEX_DVC_CMD_END_LENGTH] = {NEX_DVC_CMD_END_SEQUENCE};

    nex_err_t code = NEX_DVC_INS_FAIL;

    if (uart_write_bytes(handle->uart_num, instruction, instruction_length) < 1 || uart_write_bytes(handle->uart_num, END_SEQUENCE, NEX_DVC_CMD_END_LENGTH) < 1)
    {
        CMP_LOGE("failed writing command");

        goto END;
    }

    if (uart_wait_tx_done(handle->uart_num, pdMS_TO_TICKS(CONFIG_NEX_UART_TRANS_WAIT_TIME_MS)) != ESP_OK)
    {
        CMP_LOGE("failed waiting transmission");

        goto END;
    }

    if (parser != NULL)
    {
        code = nextion_core_process_response(handle, parser);
    }
    else
    {
        code = NEX_DVC_INS_OK;
    }

END:
    PROCESS_SYNC_GIVE(handle);

    if (code == NEX_DVC_INS_FAIL)
    {
        CMP_LOGW("device returned failure");

        return NEX_FAIL;
    }

    if (code == NEX_DVC_INS_OK)
    {
        return NEX_OK;
    }

    return code;
}

nex_err_t nextion_protocol_send_raw_byte(const nextion_t *handle, uint8_t value)
{
    uart_port_t uart = handle->uart_num;

    if (uart_write_bytes(uart, (const void *)&value, 1) < 1)
    {
        CMP_LOGE("failed writing command");

        return NEX_FAIL;
    }

    if (uart_wait_tx_done(uart, pdMS_TO_TICKS(CONFIG_NEX_UART_TRANS_WAIT_TIME_MS)) != ESP_OK)
    {
        CMP_LOGE("failed waiting transmission");

        return NEX_FAIL;
    }

    return NEX_OK;
}

//
// Core
//

static nex_err_t nextion_core_process_response(nextion_t *handle, const parser_t *parser)
{
    if (parser == NULL)
    {
        return NEX_DVC_INS_OK;
    }

    uint8_t *buffer = handle->uart_buffer;

    int total_bytes_read = 0;
    int bytes_read = uart_read_bytes(handle->uart_num, buffer + total_bytes_read, 1, pdMS_TO_TICKS(CONFIG_NEX_UART_RECV_WAIT_TIME_MS));

    total_bytes_read += bytes_read;

    if (bytes_read == 0)
    {
        return NEX_DVC_INS_OK;
    }

    if (bytes_read == -1)
    {
        return NEX_TIMEOUT;
    }

    const uint8_t data_id = buffer[0];

    if (!parser->can_parse(parser, data_id))
    {
        CMP_LOGE("parser cannot parser response: %d", data_id);

        uart_flush(handle->uart_num);

        return NEX_DVC_INS_FAIL;
    }

    while (parser->need_more_bytes(parser, buffer, total_bytes_read) > 0)
    {
        total_bytes_read += uart_read_bytes(handle->uart_num, buffer + total_bytes_read, 1, pdMS_TO_TICKS(CONFIG_NEX_UART_RECV_WAIT_TIME_MS));
    }

    if (!parser->parse(parser, buffer, total_bytes_read))
    {
        CMP_LOGE("parser cannot parser response: %d", data_id);

        uart_flush(handle->uart_num);

        return NEX_DVC_INS_FAIL;
    }

    return data_id;
}

static void nextion_core_process_events(nextion_t *handle)
{
    uint8_t *buffer = handle->uart_buffer;

    do
    {
        int total_bytes_read = 0;
        int bytes_read = uart_read_bytes(handle->uart_num, buffer + total_bytes_read, 1, pdMS_TO_TICKS(CONFIG_NEX_UART_RECV_WAIT_TIME_MS));

        total_bytes_read += bytes_read;

        if (bytes_read < 1)
        {
            return;
        }

        const uint8_t data_id = buffer[0];
        event_parser_t event_parser;

        if (!try_get_event_parser(data_id, handle->event_buffer, sizeof(handle->event_buffer), &event_parser))
        {
            CMP_LOGE("parser not found for event: %d", data_id);

            uart_flush(handle->uart_num);

            return;
        }

        if (!event_parser.base.can_parse(&event_parser.base, data_id))
        {
            CMP_LOGE("parser found cannot parse event: %d", data_id);

            uart_flush(handle->uart_num);

            return;
        }

        while (event_parser.base.need_more_bytes(&event_parser.base, buffer, total_bytes_read) > 0)
        {
            total_bytes_read += uart_read_bytes(handle->uart_num, buffer + total_bytes_read, 1, pdMS_TO_TICKS(CONFIG_NEX_UART_RECV_WAIT_TIME_MS));
        }

        if (!event_parser.base.parse(&event_parser.base, buffer, total_bytes_read))
        {
            CMP_LOGE("parser cannot parser event: %d", data_id);

            uart_flush(handle->uart_num);

            return;
        }

        esp_event_post(NEXTION_EVENT, event_parser.event_id, handle->event_buffer, event_parser.required_buffer_size, portMAX_DELAY);
    } while (true);
}

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
            // was called.
            if (PROCESS_SYNC_TAKE(handle, portMAX_DELAY))
            {
                CMP_LOGD("processing events");

                nextion_core_process_events(handle);

                PROCESS_SYNC_GIVE(handle);
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