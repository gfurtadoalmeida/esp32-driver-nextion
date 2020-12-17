// When building using VSCode tooling, the necessary
// variables are not injected in build time.
//#include "../build/config/sdkconfig.h"

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "nextion/nextion-esp32.h"
#include "nextion/page.h"
#include "nextion/component.h"

const static char *TAG = "example";

static nextion_handle_t nextion_handle;
static TaskHandle_t task_interface_handle;

void process_touch_event(const nextion_handle_t handle,
                         uint8_t page_id,
                         uint8_t component_id,
                         nextion_touch_state_t state)
{
    if (page_id == 0 && component_id == 3 && state == NEXTION_TOUCH_RELEASED)
    {
        char city[50];
        int32_t temperature;

        // Get the city name.
        if (nextion_component_get_text(handle, "t0", city, 50, NULL) == NEX_OK)
        {
            ESP_LOGI(TAG, "City: %s", city);
        }
        else
        {
            ESP_LOGE(TAG, "could not get city");
        }

        // Get the temperature.
        if (nextion_component_get_number(handle, "n0", &temperature) == NEX_OK)
        {
            ESP_LOGI(TAG, "Temperature: %d", temperature);
        }
        else
        {
            ESP_LOGE(TAG, "could not get temperature");
        }
    }
}

void do_interface_logic()
{
    do
    {
        nextion_event_process(nextion_handle);

        ESP_LOGI(TAG, "waiting for button 3 'b0' from page 0 to be pressed.");

        vTaskDelay(pdMS_TO_TICKS(300));
    } while (true);
}

void app_main(void)
{
    nextion_handle = nextion_driver_install(UART_NUM_2, 115200, GPIO_NUM_17, GPIO_NUM_16);

    if (nextion_handle == NULL)
    {
        ESP_LOGE(TAG, "could not install nextion driver");
        return;
    }

    if (nextion_init(nextion_handle) != NEX_OK)
    {
        ESP_LOGE(TAG, "could not init nextion device");
        return;
    }

    nextion_event_callback_t callback;
    callback.on_touch = &process_touch_event;

    nextion_event_callback_set(nextion_handle, callback);

    nex_err_t code = nextion_page_set(nextion_handle, "0");

    if (!NEX_DVC_CODE_IS_SUCCESS(code))
    {
        ESP_LOGE(TAG, "could not change to page 0");
        return;
    }

    if (xTaskCreate(do_interface_logic,
                    "do_interface_logic",
                    4096,
                    NULL,
                    20,
                    &task_interface_handle) != pdPASS)
    {
        ESP_LOGE(TAG, "could not create task");
        return;
    }

    vTaskSuspend(NULL);

    if (!nextion_driver_delete(nextion_handle))
    {
        ESP_LOGE(TAG, "could not delete nextion driver");
    }
}