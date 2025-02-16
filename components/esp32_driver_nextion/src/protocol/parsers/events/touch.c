#include "esp32_driver_nextion/base/codes.h"
#include "esp32_driver_nextion/base/events.h"
#include "protocol/parsers/events/touch.h"

bool parser_evt_touch_can_parse(const parser_t *parser, const uint8_t data_id)
{
    return data_id == NEX_DVC_EVT_TOUCH_OCCURRED;
}

int parser_evt_touch_need_more_bytes(const parser_t *parser, const uint8_t *data, size_t length)
{
    return 7 - length;
}

bool parser_evt_touch_parse(const parser_t *parser, const uint8_t *data, size_t length)
{
    nextion_on_touch_event_t *event = (nextion_on_touch_event_t *)parser->result_buffer;

    event->page_id = data[1];
    event->component_id = data[2];
    event->state = data[3];

    return true;
}
