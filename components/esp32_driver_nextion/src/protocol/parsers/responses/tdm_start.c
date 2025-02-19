#include "esp32_driver_nextion/base/codes.h"
#include "protocol/parsers/responses/tdm_start.h"

bool parser_rsp_tdm_start_can_parse(const parser_t *, const uint8_t data_id)
{
    return data_id == NEX_DVC_RSP_TRANSPARENT_DATA_READY || NEX_DVC_CODE_IS_ACK_RESPONSE(data_id);
}

int parser_rsp_tdm_start_need_more_bytes(const parser_t *, const uint8_t *, size_t length)
{
    return 4 - length;
}

bool parser_rsp_tdm_start_parse(const parser_t *, const uint8_t *, size_t)
{
    return true;
}