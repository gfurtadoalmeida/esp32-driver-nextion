#include "esp32_driver_nextion/base/codes.h"
#include "esp32_driver_nextion/base/events.h"
#include "protocol/parsers/events/device.h"
#include "protocol/parsers/events/tdm_stop.h"
#include "protocol/parsers/events/touch_coord.h"
#include "protocol/parsers/events/touch.h"
#include "protocol/event.h"

bool try_get_event_parser(uint8_t event_id, void *result_buffer, size_t result_buffer_length, event_parser_t *parser)
{
    parser->base.result_buffer = result_buffer;
    parser->base.result_buffer_length = result_buffer_length;

    switch (event_id)
    {
    case NEX_DVC_EVT_TOUCH_OCCURRED:
        parser->event_id = NEXTION_EVENT_TOUCHED;
        parser->base.can_parse = parser_evt_touch_can_parse;
        parser->base.need_more_bytes = parser_evt_touch_need_more_bytes;
        parser->base.parse = parser_evt_touch_parse;
        parser->required_buffer_size = sizeof(nextion_on_touch_event_t);
        return true;

    case NEX_DVC_EVT_TOUCH_COORDINATE_AWAKE:
    case NEX_DVC_EVT_TOUCH_COORDINATE_ASLEEP:
        parser->event_id = NEXTION_EVENT_TOUCHED_COORD;
        parser->base.can_parse = parser_evt_touch_coord_can_parse;
        parser->base.need_more_bytes = parser_evt_touch_coord_need_more_bytes;
        parser->base.parse = parser_evt_touch_coord_parse;
        parser->required_buffer_size = sizeof(nextion_on_touch_coord_event_t);
        return true;

    case NEX_DVC_EVT_HARDWARE_START_RESET:
    case NEX_DVC_EVT_HARDWARE_AUTO_SLEEP:
    case NEX_DVC_EVT_HARDWARE_AUTO_WAKE:
    case NEX_DVC_EVT_HARDWARE_READY:
    case NEX_DVC_EVT_HARDWARE_UPGRADE:
        parser->event_id = NEXTION_EVENT_STATE_CHANGED;
        parser->base.can_parse = parser_evt_device_can_parse;
        parser->base.need_more_bytes = parser_evt_device_need_more_bytes;
        parser->base.parse = parser_evt_device_parse;
        parser->required_buffer_size = sizeof(nextion_on_device_event_t);
        return true;

    default:
        return false;
    }
}