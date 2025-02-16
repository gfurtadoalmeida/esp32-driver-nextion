#include "esp32_driver_nextion/base/codes.h"
#include "esp32_driver_nextion/base/events.h"
#include "protocol/parsers/events/touch_coord.h"

bool parser_evt_touch_coord_can_parse(const parser_t *parser, const uint8_t data_id)
{
    return data_id == NEX_DVC_EVT_TOUCH_COORDINATE_AWAKE || data_id == NEX_DVC_EVT_TOUCH_COORDINATE_ASLEEP;
}

int parser_evt_touch_coord_need_more_bytes(const parser_t *parser, const uint8_t *data, size_t length)
{
    return 9 - length;
}

bool parser_evt_touch_coord_parse(const parser_t *parser, const uint8_t *data, size_t length)
{
    nextion_on_touch_coord_event_t *event = (nextion_on_touch_coord_event_t *)parser->result_buffer;

    event->x = (uint16_t)(((uint16_t)data[1] << 8) | (uint16_t)data[2]);
    event->y = (uint16_t)(((uint16_t)data[3] << 8) | (uint16_t)data[4]);
    event->state = data[5];
    event->exited_sleep = data[0] == NEX_DVC_EVT_TOUCH_COORDINATE_ASLEEP;

    return true;
}
