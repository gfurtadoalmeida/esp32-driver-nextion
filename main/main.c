#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp32_driver_nextion/nextion.h"
#include "esp32_driver_nextion/page.h"
#include "esp32_driver_nextion/component.h"

static const char TAG[] = "nextion";

static TaskHandle_t task_handle_user_interface;

static void callback_touch_event(nextion_on_touch_event_t event);
static void process_callback_queue(void *pvParameters);

void app_main(void)
{
    // Initialize UART.
    nextion_t *nextion_handle = nextion_driver_install(UART_NUM_2,
                                                       115200,
                                                       GPIO_NUM_17,
                                                       GPIO_NUM_16);

    // Do basic configuration.
    nextion_init(nextion_handle);

    // Set a callback for touch events.
    nextion_event_callback_set_on_touch(nextion_handle,
                                        callback_touch_event);

    // Go to page with id 0.
    nextion_page_set(nextion_handle, "0");

    // Start a task that will handle touch notifications.
    xTaskCreate(process_callback_queue,
                "user_interface",
                2048,
                (void *)nextion_handle,
                5,
                &task_handle_user_interface);

    ESP_LOGI(TAG, "waiting for button to be pressed");

    vTaskDelay(portMAX_DELAY);

    // Will never reach here.
    // It is just to show how to delete the driver.

    vTaskDelete(task_handle_user_interface);

    // Free resources.
    nextion_driver_delete(nextion_handle);
}

static void callback_touch_event(nextion_on_touch_event_t event)
{
    if (event.page_id == 0 && event.component_id == 5 && event.state == NEXTION_TOUCH_RELEASED)
    {
        ESP_LOGI(TAG, "button pressed");

        xTaskNotify(task_handle_user_interface,
                    event.component_id,
                    eSetValueWithOverwrite);
    }
}

[[noreturn]] static void process_callback_queue(void *pvParameters)
{
    const uint8_t MAX_TEXT_LENGTH = 50;

    nextion_t *nextion_handle = (nextion_t *)pvParameters;
    char text_buffer[MAX_TEXT_LENGTH];
    size_t text_length = MAX_TEXT_LENGTH;
    int32_t number;

    for (;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Get the text value from a component.
        nextion_component_get_text(nextion_handle,
                                   "value_text",
                                   text_buffer,
                                   &text_length);

        // Get the integer value from a component.
        nextion_component_get_value(nextion_handle,
                                    "value_number",
                                    &number);

        ESP_LOGI(TAG, "text: %s", text_buffer);
        ESP_LOGI(TAG, "number: %lu", number);
    }
}