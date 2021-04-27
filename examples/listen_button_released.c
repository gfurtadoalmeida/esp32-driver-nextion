// When building using VSCode tooling, the necessary
// variables are not injected in build time.
#include "../build/config/sdkconfig.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "nextion/nextion.h"
#include "nextion/page.h"
#include "nextion/component.h"

const static char *TAG = "example";

static TaskHandle_t task_handle_user_interface;

static void callback_touch_event(nextion_on_touch_event_t event);
static void process_callback_queue(void *pvParameters);

// This example uses the "/hmi/listen_button_released.hmi".
// The baud rate must be 115200.

void app_main(void)
{
    esp_log_level_set("*", ESP_LOG_VERBOSE);

    nextion_handle_t nextion_handle = nextion_driver_install(UART_NUM_2,
                                                             115200,
                                                             GPIO_NUM_17,
                                                             GPIO_NUM_16);
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

    if (!nextion_event_callback_set_on_touch(nextion_handle, callback_touch_event))
    {
        ESP_LOGE(TAG, "failed registering 'on device' event callback");
        return;
    }

    if (nextion_page_set(nextion_handle, "0") != NEX_OK)
    {
        ESP_LOGE(TAG, "failed changing to page 0");
        return;
    }

    if (xTaskCreate(process_callback_queue,
                    "user_interface",
                    2048,
                    (void *)&nextion_handle,
                    5,
                    &task_handle_user_interface) != pdPASS)
    {
        ESP_LOGE(TAG, "failed creating task");
        return;
    }

    ESP_LOGI(TAG, "waiting for button 'OK' to be pressed");

    vTaskDelay(portMAX_DELAY);

    // Will never reach here.
    // It is just to show how to delete the driver.

    vTaskDelete(task_handle_user_interface);

    if (!nextion_driver_delete(nextion_handle))
    {
        ESP_LOGE(TAG, "failed deleting driver");
    }
}

void callback_touch_event(nextion_on_touch_event_t event)
{
    if (event.page_id == 0 && event.component_id == 5 && event.state == NEXTION_TOUCH_RELEASED)
    {
        ESP_LOGI(TAG, "button 'OK' pressed");

        xTaskNotify(task_handle_user_interface, event.component_id, eSetValueWithOverwrite);
    }
    else
    {
        ESP_LOGI(TAG, "received component %d from page %d with state %d", event.page_id, event.component_id, event.state);
    }
}

void process_callback_queue(void *pvParameters)
{
    nextion_handle_t nextion_handle = *(nextion_handle_t *)pvParameters;
    const uint8_t MAX_TEXT_LENGTH = 50;
    char text[MAX_TEXT_LENGTH];
    int32_t number;

    for (;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        size_t text_length = MAX_TEXT_LENGTH;

        ESP_LOGI(TAG, "getting data");

        // Get the text.
        if (nextion_component_get_text(nextion_handle, "value_text", text, &text_length) == NEX_OK)
        {
            ESP_LOGI(TAG, "text: %s", text);
        }
        else
        {
            ESP_LOGE(TAG, "failed getting text");
        }

        // Get the number.
        if (nextion_component_get_value(nextion_handle, "value_number", &number) == NEX_OK)
        {
            ESP_LOGI(TAG, "number: %d", number);
        }
        else
        {
            ESP_LOGE(TAG, "failed getting number");
        }
    }
}