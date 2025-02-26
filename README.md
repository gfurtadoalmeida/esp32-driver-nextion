# ESP32 - Nextion Display Driver

[![GitHub Release Status][git-bagdge-release]][git-release] [![Bugs][sonar-badge-bugs]][sonar-home] [![Code Smells][sonar-badge-smells]][sonar-home] [![Security Rating][sonar-badge-security]][sonar-home] [![Quality Gate Status][sonar-badge-quality]][sonar-home]  

ESP32 driver for [Nextion](https://nextion.tech/) HMI displays.

## Characteristics

* ESP-IDF: [v5.4](https://docs.espressif.com/projects/esp-idf/en/v5.3/esp32/index.html)
* Written in **C** using just the [ESP-IDF Framework](https://github.com/espressif/esp-idf).
* Testable: 80+ tests.
* Standard eventing: receive events using `esp_event`.
* Multi-display: you can connect to as many displays as you want.
* Configurable: using [menuconfig](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/kconfig.html).

## Documentation

Everything is at the [docs](/docs) folder.

## Code Size

Build options:

* Compile optimized for size (`CONFIG_COMPILER_OPTIMIZATION_SIZE=y`).
* Error logging (`CONFIG_LOG_DEFAULT_LEVEL_ERROR=y`).

| DRAM (bss,data) | Flash (code,rodata) |
|:-:|:-:|
| 510 B | 2.72 KB |

## Example: Reading Component Values After Button Pressed

```cpp
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
```

## Contributing

To contribute to this project make sure to read our [CONTRIBUTING.md](/docs/CONTRIBUTING.md) file.

## To Do

* [ ] Have only one touch event insted of one with coordinates and other without coordinates.
* [ ] Enhance protocol handling with:
  * [ ] Automatic baud rate detection on `nextion_init`.
  * [ ] CRC support.
* [ ] Commands:
  * [ ] Expressions that return value
  * [ ] click
  * [ ] covx
  * [ ] cov
  * [ ] touch_j
  * [ ] substr
  * [ ] com_star
  * [ ] com_stop
  * [ ] randset
  * [ ] code_c
  * [ ] prints
  * [ ] print
  * [ ] printh
  * [ ] doevents
  * [ ] strlen
  * [ ] btlen
  * [ ] repo
  * [ ] cfgpio
  * [ ] ucopy
  * [ ] move
  * [ ] play
  * [ ] twfile
  * [ ] delfile
  * [ ] refile
  * [ ] findfile
  * [ ] rdfile
  * [ ] setlayer
  * [ ] newdir
  * [ ] deldir
  * [ ] redir
  * [ ] finddir
  * [ ] udelete
  * [ ] crcrest
  * [ ] crcputs
  * [ ] crcputh
  * [ ] crcputu
  * [ ] spstr
  * [ ] newfile
  * [ ] picq
  * [ ] bauds
  * [ ] spax
  * [ ] spay
  * [ ] thc
  * [ ] thdra
  * [ ] delay
  * [ ] wup
  * [ ] rtc0-6
  * [ ] pio0-7
  * [ ] pwm4-7
  * [ ] pwmf
  * [ ] eql
  * [ ] eqm
  * [ ] eqh
  * [ ] eq0-9
  * [ ] volume
  * [ ] audio0-1
  * [ ] lowpower

[git-bagdge-release]: https://github.com/gfurtadoalmeida/esp32-driver-nextion/actions/workflows/release.yml/badge.svg
[git-release]: https://github.com/gfurtadoalmeida/esp32-driver-nextion/releases
[sonar-badge-bugs]: https://sonarcloud.io/api/project_badges/measure?project=esp32_driver_nextion&metric=bugs
[sonar-badge-quality]: https://sonarcloud.io/api/project_badges/measure?project=esp32_driver_nextion&metric=alert_status
[sonar-badge-security]: https://sonarcloud.io/api/project_badges/measure?project=esp32_driver_nextion&metric=security_rating
[sonar-badge-smells]: https://sonarcloud.io/api/project_badges/measure?project=esp32_driver_nextion&metric=code_smells
[sonar-home]: https://sonarcloud.io/project/overview?id=esp32_driver_nextion
