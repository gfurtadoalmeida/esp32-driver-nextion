#include "esp32_driver_nextion/nextion.h"
#include "esp32_driver_nextion/page.h"
#include "protocol/parsers/responses/ack.h"
#include "protocol/parsers/responses/sendme.h"
#include "protocol/protocol.h"
#include "assertion.h"

nex_err_t nextion_page_get(nextion_t *handle, uint8_t *page_id)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK((page_id != NULL), "page_id error(NULL)", NEX_FAIL)

    parser_t parser = PARSER_SENDME(page_id, sizeof(uint8_t));

    nex_err_t code = nextion_protocol_send_instruction(handle, "sendme", 6, &parser);

    if (code == NEX_DVC_RSP_SENDME)
    {
        return NEX_OK;
    }

    return code;
}

nex_err_t nextion_page_set(nextion_t *handle, const char *page_name_or_id)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    formated_instruction_t instruction = FORMAT_INSTRUNCTION("page %s", page_name_or_id);
    parser_t parser = PARSER_ACK();

    return nextion_protocol_send_instruction(handle, instruction.text, instruction.length, &parser);
}

nex_err_t nextion_page_refresh(nextion_t *handle)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    parser_t parser = PARSER_ACK();

    return nextion_protocol_send_instruction(handle, "ref 0", 5, &parser);
}