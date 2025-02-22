#include <string.h>
#include "esp32_driver_nextion/base/codes.h"
#include "protocol/parsers/responses/text.h"
#include "assertion.h"

static int find_text_end(const uint8_t *data, size_t length);

bool parser_rsp_text_can_parse(const parser_t *, const uint8_t data_id)
{
    return data_id == NEX_DVC_RSP_GET_TEXT || NEX_DVC_CODE_IS_ACK_RESPONSE(data_id);
}

int parser_rsp_text_need_more_bytes(const parser_t *, const uint8_t *data, size_t length)
{
    if (length < 4)
    {
        return 4 - length;
    }

    if (data[0] == NEX_DVC_RSP_GET_TEXT && find_text_end(data, length) == -1)
    {
        return 1;
    }

    return 0;
}

bool parser_rsp_text_parse(const parser_t *parser, const uint8_t *data, size_t length)
{
    const uint8_t *string_start_address = data + 1; // First byte is an id.
    int string_end_pos = find_text_end(data, length);
    int string_size = string_end_pos - 1; // Remove the id.

    // Need an extra byte for the char termination.
    CMP_CHECK((parser->result_buffer_length >= string_size + 1), "out_length error(insufficient)", -1);

    memcpy(parser->result_buffer, string_start_address, string_size);

    ((char *)parser->result_buffer)[string_size] = '\0';

    return true;
}

static int find_text_end(const uint8_t *data, size_t length)
{
    int ends_found = 0;

    // Ignore data[0] as it is the id.
    for (size_t i = 1; i < length; i++)
    {
        if (data[i] == 0xFF)
        {
            ends_found++;

            if (ends_found == 3)
            {
                return i - 2;
            }
        }
    }

    return -1;
}