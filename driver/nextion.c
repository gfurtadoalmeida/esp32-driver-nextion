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
#include "nextion.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define NEX_UART_BUFFER_SIZE (CONFIG_NEX_UART_BUFFER_MULT * CONFIG_NEX_RESP_MSG_MAX_LENGTH)
#define NEX_EVENT_QUEUE_WAIT_TIME (pdMS_TO_TICKS(CONFIG_NEX_EVENT_QUEUE_WAIT_TIME_MS))
#define NEX_RESP_WAIT_TIME (pdMS_TO_TICKS(CONFIG_NEX_RESP_WAIT_TIME_MS))

#define NEX_CHECK(a, str, ret_val)                                        \
    if (!(a))                                                             \
    {                                                                     \
        ESP_LOGE(NEXTION_TAG, "%s(%d): %s", __FUNCTION__, __LINE__, str); \
        return (ret_val);                                                 \
    }

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
    bool _get_event_object(uint8_t code, uint8_t *buffer, int length, nextion_event_t *event);

    // Global variables.
    static const char *NEXTION_TAG = "nextion";
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
        ESP_ERROR_CHECK(uart_driver_install(uart_num, NEX_UART_BUFFER_SIZE, 0, CONFIG_NEX_UART_QUEUE_SIZE, &p_nextion_driver_obj.uart_queue, 0));

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

        if (xTaskCreate(_uart_event_task, "nex_uart_event_task", 2048, NULL, 8, &p_nextion_driver_obj.uart_task) != pdPASS)
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
        nex_err_t code = nextion_send_command("bkcmd=3\0");

        if (!NEX_DVC_CODE_IS_SUCCESS(code))
        {
            ESP_LOGE(NEXTION_TAG, "could not send 'bkcmd=3'");

            return NEX_FAIL;
        }

        // Touch coordinate events are not supported.
        // Reason: the device sends the message so fast
        // that, sometimes 2 messages are sent as one.
        // As it's not soo important, I'm cutting it out.
        code = nextion_send_command("sendxy=0\0");

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

            memcpy(text, response + NEX_DVC_CMD_START_LENGTH, length - NEX_DVC_CMD_END_LENGTH);

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
    //   4. Wait for a notification on the task handle.
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
        uint8_t *buffer = (uint8_t *)malloc(CONFIG_NEX_RESP_MSG_MAX_LENGTH);

        for (;;)
        {
            if (xQueueReceive(uart_queue, (void *)&event, (portTickType)portMAX_DELAY))
            {
                bzero(buffer, CONFIG_NEX_RESP_MSG_MAX_LENGTH);

                switch (event.type)
                {
                case UART_DATA:
                    if (uart_read_bytes(uart_num, buffer, event.size, portMAX_DELAY) >= NEX_DVC_CMD_ACK_LENGTH)
                    {
                        int length = event.size;
                        uint8_t code = buffer[0];

                        // Is it well-formed; has the endings?
                        if ((buffer[length - 1] == NEX_DVC_CMD_END_VALUE && buffer[length - 2] == NEX_DVC_CMD_END_VALUE && buffer[length - 3] == NEX_DVC_CMD_END_VALUE))
                        {
                            if (NEX_DVC_CODE_IS_EVENT(code, length))
                            {
                                if (event_enabled)
                                {
                                    if (_get_event_object(code, buffer, length, &nextion_event))
                                    {
                                        if (xQueueSend(event_queue, &nextion_event, NEX_EVENT_QUEUE_WAIT_TIME) != pdTRUE)
                                        {
                                            ESP_LOGW(NEXTION_TAG, "could not send event object to queue");
                                        }
                                    }
                                    else
                                    {
                                        ESP_LOGE(NEXTION_TAG, "could not get event object");
                                    }
                                }
                            }
                            else
                            {
                                memcpy(p_nextion_driver_obj.command_response, buffer, length);

                                p_nextion_driver_obj.command_response_length = length;

                                xTaskNotifyGive(p_nextion_driver_obj.command_task);
                            }
                        }
                        else
                        {
                            p_nextion_driver_obj.command_response_length = -1;

                            // In case of error, free the task.
                            // We don't know if it's blocked or not, so just free it.
                            xTaskNotifyGive(p_nextion_driver_obj.command_task);

                            ESP_LOGW(NEXTION_TAG, "message size error(no endings)");
                        }
                    }
                    else
                    {
                        ESP_LOGW(NEXTION_TAG, "message size error(<NEX_DVC_SERIAL_ACK_LENGTH)");
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

    bool _get_event_object(uint8_t code, uint8_t *buffer, int length, nextion_event_t *event)
    {
        NEX_CHECK((buffer != NULL), "buffer error(NULL)", false);
        NEX_CHECK((length >= NEX_DVC_CMD_ACK_LENGTH), "length error(<NEX_DVC_SERIAL_ACK_LENGTH>)", false);
        NEX_CHECK((event != NULL), "event error(NULL)", false);

        bool is_ok = true;

        switch (code)
        {
        case NEX_DVC_EVT_HARDWARE_START_RESET:
            event->type = NEXTION_DEVICE_STATE;
            event->device_state = NEXTION_DEVICE_STARTED;
            break;

        case NEX_DVC_EVT_TOUCH_OCCURRED:
            if (length == 7)
            {
                event->type = NEXTION_TOUCH_STATE;
                event->touch.page_id = buffer[1];
                event->touch.component_id = buffer[2];

                if (buffer[3] == 1U)
                {
                    event->touch.state = NEXTION_TOUCH_PRESSED;
                }
                else
                {
                    event->touch.state = NEXTION_TOUCH_RELEASED;
                }
            }
            else
            {
                is_ok = false;

                ESP_LOGW(NEXTION_TAG, "touch event with invalid length (%d<7)", length);
            }

            break;

        case NEX_DVC_EVT_TOUCH_COORDINATE_AWAKE:
        case NEX_DVC_EVT_TOUCH_COORDINATE_ASLEEP:
        {
            is_ok = false;

            ESP_LOGE(NEXTION_TAG, "touch coord event is not supported");
        }
        break;

        case NEX_DVC_EVT_HARDWARE_SLEEP_AUTOMATIC:
            event->type = NEXTION_DEVICE_STATE;
            event->device_state = NEXTION_DEVICE_AUTO_SLEEP;
            break;

        case NEX_DVC_EVT_HARDWARE_WAKE_AUTOMATIC:
            event->type = NEXTION_DEVICE_STATE;
            event->device_state = NEXTION_DEVICE_AUTO_WAKE;
            break;

        case NEX_DVC_EVT_HARDWARE_READY:
            event->type = NEXTION_DEVICE_STATE;
            event->device_state = NEXTION_DEVICE_READY;
            break;

        case NEX_DVC_EVT_HARDWARE_UPGRADE:
            event->type = NEXTION_DEVICE_STATE;
            event->device_state = NEXTION_DEVICE_UPGRADING;
            break;

        case NEX_DVC_EVT_TRANSPARENT_DATA_FINISHED:
            event->type = NEXTION_DEVICE_STATE;
            event->device_state = NEXTION_DEVICE_TRANSP_DATA_FINISHED;
            break;

        case NEX_DVC_EVT_TRANSPARENT_DATA_READY:
            event->type = NEXTION_DEVICE_STATE;
            event->device_state = NEXTION_DEVICE_TRANSP_DATA_READY;
            break;

        default:
            ESP_LOGW(NEXTION_TAG, "event with unknown code %u", code);

            is_ok = false;
        }

        return is_ok;
    }

#ifdef __cplusplus
}
#endif

#endif //__NEXTION_C__