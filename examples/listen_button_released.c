// When building using VSCode tooling, the necessary
// variables are not injected in build time.
#include "../build/config/sdkconfig.h"

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "nextion/nextion.h"
#include "nextion/page.h"
#include "nextion/component.h"

const static char *TAG = "example";

static nextion_handle_t nextion_handle;
static TaskHandle_t task_interface_handle;
static QueueHandle_t queue_handle;

static void process_touch_event(nextion_on_touch_event_t event_data);
static void process_interface_queue(void *pvParameters);

void app_main(void)
{
    esp_log_level_set("*", ESP_LOG_DEBUG);

    nextion_handle = nextion_driver_install(UART_NUM_2, 9600, GPIO_NUM_17, GPIO_NUM_16);

    if (nextion_handle == NULL)
    {
        ESP_LOGE(TAG, "failed installing driver");
        return;
    }

    if (nextion_init(nextion_handle) != NEX_OK)
    {
        ESP_LOGE(TAG, "failed initializing device");
        return;
    }

    nextion_event_callback_t callback = {
        .on_touch = &process_touch_event,
        .on_touch_coord = NULL,
        .on_device = NULL};

    nextion_event_callback_set(nextion_handle, callback);

    if (nextion_page_set(nextion_handle, "0") != NEX_OK)
    {
        ESP_LOGE(TAG, "failed changing to page 0");
        return;
    }

    queue_handle = xQueueCreate(5, sizeof(nextion_on_touch_event_t));

    if (queue_handle == NULL)
    {
        ESP_LOGE(TAG, "failed creating queue");
        return;
    }

    if (xTaskCreate(process_interface_queue,
                    "interface",
                    2048,
                    NULL,
                    20,
                    &task_interface_handle) != pdPASS)
    {
        ESP_LOGE(TAG, "failed creating task");
        return;
    }

    ESP_LOGI(TAG, "waiting for button 3 'b0', from page 0, to be pressed");

    vTaskDelay(portMAX_DELAY);

    // Will never reach here.
    // It is just to show how to delete the driver.
    if (!nextion_driver_delete(nextion_handle))
    {
        ESP_LOGE(TAG, "failed deleting driver");
    }
}

void process_touch_event(nextion_on_touch_event_t event_data)
{
    xQueueSend(queue_handle, &event_data, pdMS_TO_TICKS(100));
}

void process_interface_queue(void *pvParameters)
{
    nextion_on_touch_event_t event_data;

    for (;;)
    {
        if (xQueueReceive(queue_handle, &event_data, portMAX_DELAY) == pdTRUE)
        {
            ESP_LOGI(TAG, "received an event");

            if (event_data.page_id == 0 && event_data.component_id == 3 && event_data.state == NEXTION_TOUCH_RELEASED)
            {
                char text[50];
                size_t text_length = 50;
                int32_t number;

                // Get the text.
                if (nextion_component_get_text(nextion_handle, "t0", text, &text_length) == NEX_OK)
                {
                    ESP_LOGI(TAG, "text: %s", text);
                }
                else
                {
                    ESP_LOGE(TAG, "failed getting text");
                }

                // Get the number.
                if (nextion_component_get_number(nextion_handle, "n0", &number) == NEX_OK)
                {
                    ESP_LOGI(TAG, "number: %d", number);
                }
                else
                {
                    ESP_LOGE(TAG, "failed getting number");
                }
            }
        }
    }
}