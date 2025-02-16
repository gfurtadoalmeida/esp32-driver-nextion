#include "esp32_driver_nextion/component.h"
#include "protocol/parsers/responses/ack.h"
#include "protocol/parsers/responses/text.h"
#include "protocol/parsers/responses/number.h"
#include "protocol/protocol.h"
#include "assertion.h"

nex_err_t nextion_component_refresh(nextion_t *handle, const char *component_name_or_id)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK((component_name_or_id != NULL), "component_name_or_id error(NULL)", NEX_FAIL)

    formated_instruction_t instruction = FORMAT_INSTRUNCTION("ref %s", component_name_or_id);
    parser_t parser = PARSER_ACK();

    return nextion_protocol_send_instruction(handle, instruction.text, instruction.length, &parser);
}

nex_err_t nextion_component_set_visibility(nextion_t *handle, const char *component_name_or_id, bool is_visible)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK((component_name_or_id != NULL), "component_name_or_id error(NULL)", NEX_FAIL)

    formated_instruction_t instruction = FORMAT_INSTRUNCTION("vis %s,%d", component_name_or_id, is_visible);
    parser_t parser = PARSER_ACK();

    return nextion_protocol_send_instruction(handle, instruction.text, instruction.length, &parser);
}

nex_err_t nextion_component_set_visibility_all(nextion_t *handle, bool is_visible)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    return nextion_component_set_visibility(handle, "255", is_visible);
}

nex_err_t nextion_component_set_touchable(nextion_t *handle, const char *component_name_or_id, bool is_touchable)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK((component_name_or_id != NULL), "component_name_or_id error(NULL)", NEX_FAIL)

    formated_instruction_t instruction = FORMAT_INSTRUNCTION("tsw %s,%d", component_name_or_id, is_touchable);
    parser_t parser = PARSER_ACK();

    return nextion_protocol_send_instruction(handle, instruction.text, instruction.length, &parser);
}

nex_err_t nextion_component_set_touchable_all(nextion_t *handle, bool is_touchable)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    return nextion_component_set_touchable(handle, "255", is_touchable);
}

nex_err_t nextion_component_get_text(nextion_t *handle,
                                     const char *component_name,
                                     char *buffer,
                                     size_t buffer_length)
{
    return nextion_component_get_property_text(handle, component_name, "txt", buffer, buffer_length);
}

nex_err_t nextion_component_get_value(nextion_t *handle, const char *component_name, int32_t *number)
{
    return nextion_component_get_property_number(handle, component_name, "val", number);
}

nex_err_t nextion_component_get_boolean(nextion_t *handle, const char *component_name, bool *value)
{
    int32_t temp = 0;
    nex_err_t code = nextion_component_get_property_number(handle, component_name, "val", &temp);

    *value = (bool)temp;

    return code;
}

nex_err_t nextion_component_set_text(nextion_t *handle, const char *component_name, const char *text)
{
    return nextion_component_set_property_text(handle, component_name, "txt", text);
}

nex_err_t nextion_component_set_value(nextion_t *handle, const char *component_name, int32_t number)
{
    return nextion_component_set_property_number(handle, component_name, "val", number);
}

nex_err_t nextion_component_set_boolean(nextion_t *handle, const char *component_name, bool value)
{
    return nextion_component_set_property_number(handle, component_name, "val", (int32_t)value);
}

nex_err_t nextion_component_get_property_text(nextion_t *handle,
                                              const char *component_name,
                                              const char *property_name,
                                              char *buffer,
                                              size_t buffer_length)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK((component_name != NULL), "component_name error(NULL)", NEX_FAIL)
    CMP_CHECK((property_name != NULL), "property_name error(NULL)", NEX_FAIL)
    CMP_CHECK((buffer != NULL), "buffer error(NULL)", NEX_FAIL)

    formated_instruction_t instruction = FORMAT_INSTRUNCTION("get %s.%s", component_name, property_name);
    parser_t parser = PARSER_TEXT(buffer, buffer_length);

    nex_err_t code = nextion_protocol_send_instruction(handle, instruction.text, instruction.length, &parser);

    if (code == NEX_DVC_RSP_GET_STRING)
    {
        return NEX_OK;
    }

    return code;
}

nex_err_t nextion_component_get_property_number(nextion_t *handle,
                                                const char *component_name,
                                                const char *property_name,
                                                int32_t *number)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK((component_name != NULL), "component_name error(NULL)", NEX_FAIL)
    CMP_CHECK((property_name != NULL), "property_name error(NULL)", NEX_FAIL)
    CMP_CHECK((number != NULL), "number error(NULL)", NEX_FAIL)

    formated_instruction_t instruction = FORMAT_INSTRUNCTION("get %s.%s", component_name, property_name);
    parser_t parser = PARSER_NUMBER(number, sizeof(int32_t));

    nex_err_t code = nextion_protocol_send_instruction(handle, instruction.text, instruction.length, &parser);

    if (code == NEX_DVC_RSP_GET_NUMBER)
    {
        return NEX_OK;
    }

    return code;
}

nex_err_t nextion_component_set_property_text(nextion_t *handle,
                                              const char *component_name,
                                              const char *property_name,
                                              const char *text)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK((component_name != NULL), "component_name error(NULL)", NEX_FAIL)
    CMP_CHECK((property_name != NULL), "property_name error(NULL)", NEX_FAIL)
    CMP_CHECK((text != NULL), "text error(NULL)", NEX_FAIL)

    formated_instruction_t instruction = FORMAT_INSTRUNCTION("%s.%s=\"%s\"", component_name, property_name, text);
    parser_t parser = PARSER_ACK();

    return nextion_protocol_send_instruction(handle, instruction.text, instruction.length, &parser);
}

nex_err_t nextion_component_set_property_number(nextion_t *handle,
                                                const char *component_name,
                                                const char *property_name,
                                                int32_t number)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK((component_name != NULL), "component_name error(NULL)", NEX_FAIL)
    CMP_CHECK((property_name != NULL), "property_name error(NULL)", NEX_FAIL)

    formated_instruction_t instruction = FORMAT_INSTRUNCTION("%s.%s=%ld", component_name, property_name, number);
    parser_t parser = PARSER_ACK();

    return nextion_protocol_send_instruction(handle, instruction.text, instruction.length, &parser);
}