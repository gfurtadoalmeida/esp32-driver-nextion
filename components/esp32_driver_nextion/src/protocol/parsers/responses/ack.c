#include "esp32_driver_nextion/base/codes.h"
#include "protocol/parsers/responses/ack.h"

bool parser_rsp_ack_can_parse(const parser_t *parser, const uint8_t data_id)
{
    return NEX_DVC_CODE_IS_ACK_RESPONSE(data_id);
}

int parser_rsp_ack_need_more_bytes(const parser_t *parser, const uint8_t *data, size_t length)
{
    return 4 - length;
}

bool parser_rsp_ack_parse(const parser_t *parser, const uint8_t *data, size_t length)
{
    return true;
}
