#include "esp32_driver_nextion/component.h"
#include "protocol/protocol.h"
#include "assertion.h"

nex_err_t nextion_component_refresh(nextion_t *handle, const char *component_name_or_id)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK((component_name_or_id != NULL), "component_name_or_id error(NULL)", NEX_FAIL)

    return nextion_protocol_send_instruction_ack(handle, "ref %s", component_name_or_id);
}

nex_err_t nextion_component_set_visibility(nextion_t *handle, const char *component_name_or_id, bool is_visible)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK((component_name_or_id != NULL), "component_name_or_id error(NULL)", NEX_FAIL)

    return nextion_protocol_send_instruction_ack(handle, "vis %s,%d", component_name_or_id, is_visible);
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

    return nextion_protocol_send_instruction_ack(handle, "tsw %s,%d", component_name_or_id, is_touchable);
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

    return nextion_protocol_send_instruction_get_text(handle,
                                                      buffer,
                                                      buffer_length,
                                                      "get %s.%s", component_name, property_name);
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

    return nextion_protocol_send_instruction_get_number(handle, number, "get %s.%s", component_name, property_name);
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

    return nextion_protocol_send_instruction_ack(handle, "%s.%s=\"%s\"", component_name, property_name, text);
}

nex_err_t nextion_component_set_property_number(nextion_t *handle,
                                                const char *component_name,
                                                const char *property_name,
                                                int32_t number)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK((component_name != NULL), "component_name error(NULL)", NEX_FAIL)
    CMP_CHECK((property_name != NULL), "property_name error(NULL)", NEX_FAIL)

    return nextion_protocol_send_instruction_ack(handle, "%s.%s=%ld", component_name, property_name, number);
}