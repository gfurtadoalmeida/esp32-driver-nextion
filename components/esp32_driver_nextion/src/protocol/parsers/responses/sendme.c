#include "esp32_driver_nextion/base/codes.h"
#include "protocol/parsers/responses/sendme.h"

bool parser_rsp_sendme_can_parse(const parser_t *, const uint8_t data_id)
{
    return data_id == NEX_DVC_RSP_SENDME || NEX_DVC_CODE_IS_ACK_RESPONSE(data_id);
}

int parser_rsp_sendme_need_more_bytes(const parser_t *, const uint8_t *data, size_t length)
{
    if (data[0] == NEX_DVC_RSP_SENDME)
    {
        return 5 - length;
    }

    return 4 - length;
}

bool parser_rsp_sendme_parse(const parser_t *parser, const uint8_t *data, size_t)
{
    *((uint8_t *)parser->result_buffer) = data[1];

    return true;
}