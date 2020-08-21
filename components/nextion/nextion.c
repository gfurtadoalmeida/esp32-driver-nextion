#ifndef __NEXTION_C__
#define __NEXTION_C__

// When building using VSCode tooling, the necessary
// variables are not injected in build time.
//#include "../../../build/config/sdkconfig.h"

#include <string.h>
#include "esp_log.h"
#include "nextion_config.h"
#include "nextion_constants.h"
#include "nextion_codes.h"
#include "nextion_common.h"
#include "nextion_parse.h"
#include "nextion.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define NEX_EVENT_QUEUE_WAIT_TIME (pdMS_TO_TICKS(CONFIG_NEX_EVENT_QUEUE_WAIT_TIME_MS))
#define NEX_RESP_WAIT_TIME (pdMS_TO_TICKS(CONFIG_NEX_RESP_WAIT_TIME_MS))

    typedef struct
    {
        uart_port_t uart_num;            // UART port number.
        QueueHandle_t uart_queue;        // UART queue.
        TaskHandle_t uart_task;          // UART task to process the "uart_queue" messages.
        SemaphoreHandle_t command_mutex; // Mutex to synchronize UART operations.
        TaskHandle_t command_task;       // Task handle used for task notification between "_send_command" and "uart_task".
        uint8_t *command_response;       // Buffer for the last command response data. Not user for events.
        int command_response_length;     // Size, in bytes, of the last command response data.
        QueueHandle_t event_queue;       // Event queue where device events will be sent.
        bool event_enabled;              // If it can send events.
        bool is_installed;               // If the driver is installed.
    } nextion_driver_obj_t;

    // Private methods declarations.
    int _send_command(const char *command, uint8_t **response_buffer);
    static void _uart_event_task(void *pvParameters);
    int _find_message_length(const uint8_t *buffer, int buffer_length);
    bool _assemble_event_object(uint8_t code, const uint8_t *buffer, int length, nextion_event_t *event);

    // Global variables.
    static nextion_driver_obj_t p_nextion_driver_obj;

    nex_err_t nextion_driver_install(uart_port_t uart_num, int baud_rate, int tx_io_num, int rx_io_num, int queue_size, QueueHandle_t *event_queue)
    {
        if (nextion_is_driver_installed())
            return NEX_OK;

        NEX_CHECK((baud_rate >= NEX_SERIAL_BAUD_RATE_MIN || baud_rate <= NEX_SERIAL_BAUD_RATE_MAX), "baud_rate error", NEX_FAIL);
        NEX_CHECK((queue_size > 0 || event_queue == NULL), "queue_size error(<1)", NEX_FAIL);

        ESP_LOGI(NEXTION_TAG, "installing driver on uart: %d", uart_num);

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
        ESP_ERROR_CHECK(uart_driver_install(uart_num, CONFIG_NEX_UART_RECV_BUFFER_SIZE, 0, CONFIG_NEX_UART_QUEUE_SIZE, &p_nextion_driver_obj.uart_queue, 0));

        if (event_queue != NULL)
        {
            QueueHandle_t queue_handle = xQueueCreate(queue_size, sizeof(nextion_event_t));

            if (queue_handle == 0)
            {
                ESP_LOGE(NEXTION_TAG, "could not create event queue");

                return NEX_FAIL;
            }

            p_nextion_driver_obj.event_queue = queue_handle;
            *event_queue = p_nextion_driver_obj.event_queue;

            p_nextion_driver_obj.event_enabled = true;
        }

        p_nextion_driver_obj.command_response = (uint8_t *)malloc(CONFIG_NEX_RESP_MSG_MAX_LENGTH);
        p_nextion_driver_obj.command_mutex = xSemaphoreCreateMutex();

        if (p_nextion_driver_obj.command_mutex == NULL)
        {
            ESP_LOGE(NEXTION_TAG, "could not create operation mutex");

            return NEX_FAIL;
        }

        if (xTaskCreate(_uart_event_task, "nex_uart_task", 2048, NULL, 8, &p_nextion_driver_obj.uart_task) != pdPASS)
        {
            ESP_LOGE(NEXTION_TAG, "could not create uart task");

            return NEX_FAIL;
        }

        p_nextion_driver_obj.uart_num = uart_num;
        p_nextion_driver_obj.is_installed = true;

        ESP_LOGI(NEXTION_TAG, "driver installed");

        return NEX_OK;
    }

    nex_err_t nextion_driver_init()
    {
        // The logic relies on receiving responses in all
        // cases, be it success or failure.
        nex_err_t code = nextion_send_command("bkcmd=3");

        if (!NEX_DVC_CODE_IS_SUCCESS(code))
        {
            ESP_LOGE(NEXTION_TAG, "could not send 'bkcmd=3'");

            return NEX_FAIL;
        }

        // Touch coordinate events are not supported.
        // Reason: the device sends the message so fast
        // that, sometimes 2 messages are sent as one.
        // As it's not soo important, I'm cutting it out.
        code = nextion_send_command("sendxy=0");

        if (!NEX_DVC_CODE_IS_SUCCESS(code))
        {
            ESP_LOGE(NEXTION_TAG, "could not send 'sendxy=0'");

            return NEX_FAIL;
        }

        return NEX_OK;
    }

    bool nextion_is_driver_installed()
    {
        return p_nextion_driver_obj.is_installed;
    }

    nex_err_t nextion_driver_delete()
    {
        if (!nextion_is_driver_installed())
            return NEX_OK;

        ESP_LOGI(NEXTION_TAG, "deleting driver");

        // Will free p_nextion_driver_obj.uart_queue
        ESP_ERROR_CHECK(uart_driver_delete(p_nextion_driver_obj.uart_num));

        vTaskDelete(p_nextion_driver_obj.uart_task);
        vSemaphoreDelete(p_nextion_driver_obj.command_mutex);

        free(p_nextion_driver_obj.command_response);

        if (p_nextion_driver_obj.event_enabled)
        {
            vQueueDelete(p_nextion_driver_obj.event_queue);

            p_nextion_driver_obj.event_queue = NULL;
            p_nextion_driver_obj.event_enabled = false;
        }

        p_nextion_driver_obj.uart_num = -1;
        p_nextion_driver_obj.uart_queue = NULL;
        p_nextion_driver_obj.uart_task = NULL;
        p_nextion_driver_obj.command_mutex = NULL;
        p_nextion_driver_obj.command_task = NULL;
        p_nextion_driver_obj.command_response = NULL;
        p_nextion_driver_obj.command_response_length = 0;
        p_nextion_driver_obj.is_installed = false;

        ESP_LOGI(NEXTION_TAG, "driver deleted");

        return NEX_OK;
    }

    nex_err_t nextion_send_command(const char *command)
    {
        NEX_CHECK((command != NULL), "command error(NULL)", NEX_FAIL);

        uint8_t *response = NULL;
        const int read = _send_command(command, &response);

        if (read == NEX_DVC_CMD_ACK_LENGTH)
        {
            return (nex_err_t)response[0];
        }

        return NEX_FAIL;
    }

    int nextion_get_text(const char *command, char *text)
    {
        NEX_CHECK((command != NULL), "command error(NULL)", -1);
        NEX_CHECK((text != NULL), "text error(NULL)", -1);

        uint8_t *response = NULL;
        const int read = _send_command(command, &response);

        if (read >= NEX_DVC_CMD_ACK_LENGTH && response[0] == NEX_DVC_RSP_GET_STRING)
        {
            const int length = read - NEX_DVC_CMD_ACK_LENGTH;

            text[length] = '\0';

            memcpy(text, response + NEX_DVC_CMD_START_LENGTH, length);

            return length;
        }

        return -1;
    }

    bool nextion_get_number(const char *command, int *number)
    {
        NEX_CHECK((command != NULL), "command error(NULL)", false);
        NEX_CHECK((number != NULL), "number error(NULL)", false);

        uint8_t *response = NULL;

        const int read = _send_command(command, &response);

        if (read == 8 && response[0] == NEX_DVC_RSP_GET_NUMBER)
        {
            *number = (int)(((uint32_t)response[4] << 24) | ((uint32_t)response[3] << 16) | ((uint32_t)response[2] << 8) | (uint32_t)response[1]);

            return true;
        }

        return false;
    }

    // Private Section
    // Only the methods below are allowed to read and write
    // the UART directly.
    //
    // How does a "send command" works?
    //  _send_command
    //   1. Acquire a mutex, to prevent multiple calls.
    //   2. Get the invoking task handle and save on the driver obj.
    //   3. Write to the UART.
    //   4. Wait for a notification on the task handle (or timeout).
    //  _uart_event_task
    //   5. Copy the received data to the global buffer.
    //   6. Set the received length on the driver obj.
    //   7. Notify the task.
    //  _send_command
    //   8. Upon notification, continue processing.
    //   9. Release the mutex.

    int _send_command(const char *command, uint8_t **response_buffer)
    {
        NEX_CHECK((command != NULL), "command error(NULL)", -1);
        NEX_CHECK((response_buffer != NULL), "response_buffer error(NULL)", -1);

        const uart_port_t uart_num = p_nextion_driver_obj.uart_num;
        const uint8_t ending[] = {NEX_DVC_CMD_END_SEQUENCE};

        xSemaphoreTake(p_nextion_driver_obj.command_mutex, portMAX_DELAY);

        uart_write_bytes(uart_num, command, strlen(command));
        uart_write_bytes(uart_num, (char *)ending, NEX_DVC_CMD_END_LENGTH);

        int read = -1;

        p_nextion_driver_obj.command_task = xTaskGetCurrentTaskHandle();

        if (ulTaskNotifyTake(pdTRUE, NEX_RESP_WAIT_TIME))
        {
            *response_buffer = p_nextion_driver_obj.command_response;
            read = p_nextion_driver_obj.command_response_length;
        }
        else
        {
            ESP_LOGE(NEXTION_TAG, "read timeout");
        }

        xSemaphoreGive(p_nextion_driver_obj.command_mutex);

        return read;
    }

    static void _uart_event_task(void *pvParameters)
    {
        const uart_port_t uart_num = p_nextion_driver_obj.uart_num;
        const QueueHandle_t uart_queue = p_nextion_driver_obj.uart_queue;
        const QueueHandle_t event_queue = p_nextion_driver_obj.event_queue;
        const bool event_enabled = p_nextion_driver_obj.event_enabled;

        uart_event_t event;
        nextion_event_t nextion_event;

        // Will hold 5 messages at most. In my testings the device sent only 2 ACK (4 bytes),
        // at the same time. This must not be a problem.
        const int buffer_length = 5 * CONFIG_NEX_RESP_MSG_MAX_LENGTH;
        uint8_t *buffer = (uint8_t *)malloc(buffer_length);

        int buffer_pos = 0;

        for (;;)
        {
            bzero(buffer, CONFIG_NEX_RESP_MSG_MAX_LENGTH);

            if (xQueueReceive(uart_queue, (void *)&event, (portTickType)portMAX_DELAY))
            {
                switch (event.type)
                {
                case UART_DATA:
                {
                    int space_remaining = buffer_length - buffer_pos;

                    // The buffer will act like a circular one.
                    // If we can't fill it, just zero and start from beggining.
                    if (event.size > space_remaining)
                    {
                        buffer_pos = 0;
                        space_remaining = buffer_length;

                        bzero(buffer, CONFIG_NEX_RESP_MSG_MAX_LENGTH);
                    }

                    // From here below this is the only way to access the buffer.
                    uint8_t *buffer_adv = buffer + buffer_pos;

                    uart_read_bytes(uart_num, buffer_adv, event.size, portMAX_DELAY);

                    int message_length = nextion_parse_find_message_length(buffer_adv, space_remaining);

                    if (message_length > -1)
                    {
                        uint8_t code = buffer_adv[0];

                        if (NEX_DVC_CODE_IS_EVENT(code, message_length))
                        {
                            if (event_enabled)
                            {
                                if (nextion_parse_assemble_event(code, buffer_adv, message_length, &nextion_event))
                                {
                                    if (xQueueSend(event_queue, &nextion_event, NEX_EVENT_QUEUE_WAIT_TIME) != pdTRUE)
                                    {
                                        ESP_LOGW(NEXTION_TAG, "could not send event to queue");
                                    }
                                }
                                else
                                {
                                    ESP_LOGE(NEXTION_TAG, "could not assemble event");
                                }
                            }
                        }
                        else
                        {
                            memcpy(p_nextion_driver_obj.command_response, buffer_adv, message_length);

                            p_nextion_driver_obj.command_response_length = message_length;

                            xTaskNotifyGive(p_nextion_driver_obj.command_task);
                        }

                        buffer_pos += message_length;
                    }
                    else if (space_remaining < NEX_DVC_CMD_ACK_LENGTH)
                    {
                        // If it haven't found a message and cannot fill a whole ACK,
                        // give up, zero the buffer and release the task.
                        // It means the buffer was not big enough or there is a bug anywhere.

                        p_nextion_driver_obj.command_response_length = -1;
                        buffer_pos = 0;

                        bzero(buffer, CONFIG_NEX_RESP_MSG_MAX_LENGTH);

                        xTaskNotifyGive(p_nextion_driver_obj.command_task);
                    }
                }
                break;

                case UART_FIFO_OVF:
                    ESP_LOGW(NEXTION_TAG, "hw fifo overflow, might need flow control");

                    uart_flush_input(uart_num);
                    xQueueReset(uart_queue);
                    break;

                case UART_BUFFER_FULL:
                    ESP_LOGW(NEXTION_TAG, "ring buffer full, might need size increase");

                    uart_flush_input(uart_num);
                    xQueueReset(uart_queue);
                    break;

                case UART_PARITY_ERR:
                    ESP_LOGW(NEXTION_TAG, "uart parity error");
                    break;

                case UART_FRAME_ERR:
                    ESP_LOGW(NEXTION_TAG, "uart frame error");
                    break;

                default:
                    ESP_LOGI(NEXTION_TAG, "uart event type: %d", event.type);
                    break;
                }
            }
        }

        free(buffer);
        buffer = NULL;
        vTaskDelete(NULL);
    }

#ifdef __cplusplus
}
#endif

#endif //__NEXTION_C__