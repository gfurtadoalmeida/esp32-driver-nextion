#include "esp32_driver_nextion/base/codes.h"
#include "esp32_driver_nextion/base/events.h"
#include "protocol/parsers/events/device.h"

bool parser_evt_device_can_parse(const parser_t *, const uint8_t data_id)
{
    return data_id == NEX_DVC_EVT_HARDWARE_START_RESET ||
           data_id == NEX_DVC_EVT_HARDWARE_AUTO_SLEEP ||
           data_id == NEX_DVC_EVT_HARDWARE_AUTO_WAKE ||
           data_id == NEX_DVC_EVT_HARDWARE_READY ||
           data_id == NEX_DVC_EVT_HARDWARE_UPGRADE;
}

int parser_evt_device_need_more_bytes(const parser_t *, const uint8_t *data, size_t length)
{
    if (data[0] == NEX_DVC_EVT_HARDWARE_START_RESET)
    {
        return 6 - length;
    }

    return 4 - length;
}

bool parser_evt_device_parse(const parser_t *parser, const uint8_t *data, size_t)
{
    nextion_on_device_event_t *event = (nextion_on_device_event_t *)parser->result_buffer;

    event->state = data[0];

    return true;
}
