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

static void callback_touch_event(TaskHandle_t task_handle_user_interface,
                                 esp_event_base_t event_base,
                                 nextion_event_t event_id,
                                 const nextion_on_touch_event_t *event);

static void process_callback_queue(void *pvParameters);

void app_main(void)
{
    esp_event_loop_create_default();

    TaskHandle_t task_handle_user_interface;

    // Initialize UART.
    nextion_t *nextion_handle = nextion_driver_install(UART_NUM_2,
                                                       115200,
                                                       GPIO_NUM_17,
                                                       GPIO_NUM_16);

    // Do basic configuration.
    nextion_init(nextion_handle);

    // Go to page with id 0.
    nextion_page_set(nextion_handle, "0");

    // Start a task that will handle touch notifications.
    xTaskCreate(process_callback_queue,
                "user_interface",
                2048,
                (void *)nextion_handle,
                5,
                &task_handle_user_interface);

    // Set a callback for touch events.
    esp_event_handler_register(NEXTION_EVENT, NEXTION_EVENT_TOUCHED, (esp_event_handler_t)callback_touch_event, task_handle_user_interface);

    ESP_LOGI(TAG, "waiting for button to be pressed");

    vTaskDelay(portMAX_DELAY);

    // Will never reach here.
    // It is just to show how to delete the driver.

    esp_event_handler_unregister(NEXTION_EVENT, NEXTION_EVENT_TOUCHED, (esp_event_handler_t)callback_touch_event);

    vTaskDelete(task_handle_user_interface);

    // Free resources.
    nextion_driver_delete(nextion_handle);
}

static void callback_touch_event(TaskHandle_t task_handle_user_interface,
                                 esp_event_base_t event_base,
                                 nextion_event_t event_id,
                                 const nextion_on_touch_event_t *event)
{
    ESP_LOGI(TAG, "EVENTO: %d: page %d, cmp %d, state: %d", event_id, event->page_id, event->component_id, event->state);

    if (event->page_id == 0 && event->component_id == 3 && event->state == NEXTION_TOUCH_RELEASED)
    {
        ESP_LOGI(TAG, "button pressed");

        xTaskNotify(task_handle_user_interface,
                    event->component_id,
                    eSetValueWithOverwrite);
    }
}

[[noreturn]] static void process_callback_queue(void *pvParameters)
{
    const uint8_t MAX_TEXT_LENGTH = 50;

    nextion_t *nextion_handle = (nextion_t *)pvParameters;
    char text_buffer[MAX_TEXT_LENGTH];
    int32_t number;

    for (;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Get the text value from a component.
        nextion_component_get_text(nextion_handle,
                                   "t0",
                                   text_buffer,
                                   MAX_TEXT_LENGTH);

        // Get the integer value from a component.
        nextion_component_get_value(nextion_handle,
                                    "n0",
                                    &number);

        ESP_LOGI(TAG, "text: %s", text_buffer);
        ESP_LOGI(TAG, "number: %ld", number);
    }
}