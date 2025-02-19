#include "esp32_driver_nextion/base/codes.h"
#include "protocol/parsers/events/tdm_stop.h"

bool parser_evt_tdm_stop_can_parse(const parser_t *, const uint8_t data_id)
{
    return data_id == NEX_DVC_EVT_TRANSPARENT_DATA_FINISHED;
}

int parser_evt_tdm_stop_need_more_bytes(const parser_t *, const uint8_t *, size_t length)
{
    return 4 - length;
}

bool parser_evt_tdm_stop_parse(const parser_t *, const uint8_t *, size_t )
{
    return true;
}