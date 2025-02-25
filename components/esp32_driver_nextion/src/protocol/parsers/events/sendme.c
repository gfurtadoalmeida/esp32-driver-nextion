#include "esp32_driver_nextion/base/codes.h"
#include "esp32_driver_nextion/base/events.h"
#include "protocol/parsers/events/sendme.h"
#include "protocol/parsers/responses/sendme.h"

bool parser_evt_sendme_can_parse(const parser_t *parser, const uint8_t data_id)
{
    return parser_rsp_sendme_can_parse(parser, data_id);
}

int parser_evt_sendme_need_more_bytes(const parser_t *parser, const uint8_t *data, size_t length)
{
    return parser_rsp_sendme_need_more_bytes(parser, data, length);
}

bool parser_evt_sendme_parse(const parser_t *parser, const uint8_t *data, size_t)
{
    nextion_page_changed_event_t *event = (nextion_page_changed_event_t *)parser->result_buffer;

    event->page_id = parser_rsp_sendme_convert(data + 1);

    return true;
}