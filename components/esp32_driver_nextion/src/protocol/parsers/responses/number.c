#include "esp32_driver_nextion/base/codes.h"
#include "protocol/parsers/responses/number.h"

bool parser_rsp_number_can_parse(const parser_t *parser, const uint8_t data_id)
{
    return data_id == NEX_DVC_RSP_GET_NUMBER || NEX_DVC_CODE_IS_ACK_RESPONSE(data_id);
}

int parser_rsp_number_need_more_bytes(const parser_t *parser, const uint8_t *data, size_t length)
{
    if (data[0] == NEX_DVC_RSP_GET_NUMBER)
    {
        return 8 - length;
    }

    return 4 - length;
}

bool parser_rsp_number_parse(const parser_t *parser, const uint8_t *data, size_t length)
{
    *((int32_t *)parser->result_buffer) = parser_rsp_number_convert(data + 1);

    return true;
}

int32_t parser_rsp_number_convert(const uint8_t *data)
{
    // Little endian, 4 byte (32-bit) value.
    // Example: 0x01 0x02 0x03 0x04
    // 67305985 = (0x01 + 0x02 * 256 + 0x03 * 65536 + 0x04 * 16777216)

    uint32_t unsigned_value = ((uint32_t)data[0]) | ((uint32_t)data[1] << 8) | ((uint32_t)data[2] << 16) | ((uint32_t)data[3] << 24);

    return (int32_t)unsigned_value;
}
