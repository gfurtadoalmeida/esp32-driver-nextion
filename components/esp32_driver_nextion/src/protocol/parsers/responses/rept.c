#include <string.h>
#include "protocol/parsers/responses/rept.h"

bool parser_rsp_rept_can_parse(const parser_t *, const uint8_t)
{
    // The "rept" returns raw data; there is no identifier.

    return true;
}

int parser_rsp_rept_need_more_bytes(const parser_t *parser, const uint8_t *, size_t length)
{
    return parser->result_buffer_length - length;
}

bool parser_rsp_rept_parse(const parser_t *parser, const uint8_t *data, size_t)
{
    memcpy(parser->result_buffer, data, parser->result_buffer_length);

    return true;
}
