#include "esp32_driver_nextion/base/events.h"
#include "protocol/protocol.h"
#include "common_infra_test.h"

static void callback_page_changed(TaskHandle_t task_handle,
                                  esp_event_base_t event_base,
                                  nextion_event_t event_id,
                                  const nextion_page_changed_event_t *event);

TEST_CASE("Receive page changed", "[events]")
{
    TaskHandle_t current_task = xTaskGetCurrentTaskHandle();

    xTaskNotifyStateClear(current_task);

    esp_event_handler_register(NEXTION_EVENT, NEXTION_EVENT_PAGE_CHANGED, (esp_event_handler_t)callback_page_changed, current_task);

    nextion_protocol_send_instruction(handle, "click b1,1", 10, NULL);

    uint32_t notification_value = 0;

    bool success = xTaskNotifyWait(0, 0xFFFFFFFF, &notification_value, pdMS_TO_TICKS(5000)) == pdTRUE && ((notification_value & 0x01) != 0);

    esp_event_handler_unregister(NEXTION_EVENT, NEXTION_EVENT_PAGE_CHANGED, (esp_event_handler_t)callback_page_changed);

    if (!success)
    {
        FAIL_TEST("Did not receive page changed event");
    }
}

void callback_page_changed(TaskHandle_t task_handle,
                           esp_event_base_t event_base,
                           nextion_event_t event_id,
                           const nextion_page_changed_event_t *event)
{
    xTaskNotify(task_handle, 0x01, eSetBits);
}
