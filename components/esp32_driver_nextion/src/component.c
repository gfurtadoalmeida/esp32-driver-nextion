#include "esp32_driver_nextion/nextion.h"
#include "esp32_driver_nextion/system.h"
#include "esp32_driver_nextion/component.h"
#include "assertion.h"

nex_err_t nextion_component_refresh(nextion_t *handle, const char *component_name_or_id)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK((component_name_or_id != NULL), "component_name_or_id error(NULL)", NEX_FAIL)

    return nextion_command_send(handle, "ref %s", component_name_or_id);
}

nex_err_t nextion_component_set_visibility(nextion_t *handle, const char *component_name_or_id, bool is_visible)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK((component_name_or_id != NULL), "component_name_or_id error(NULL)", NEX_FAIL)

    return nextion_command_send(handle, "vis %s,%d", component_name_or_id, is_visible);
}

nex_err_t nextion_component_set_visibility_all(nextion_t *handle, bool is_visible)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    return nextion_command_send(handle, "vis 255,%d", is_visible);
}

nex_err_t nextion_component_set_touchable(nextion_t *handle, const char *component_name_or_id, bool is_touchable)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK((component_name_or_id != NULL), "component_name_or_id error(NULL)", NEX_FAIL)

    return nextion_command_send(handle, "tsw %s,%d", component_name_or_id, is_touchable);
}

nex_err_t nextion_component_set_touchable_all(nextion_t *handle, bool is_touchable)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    return nextion_command_send(handle, "tsw 255,%d", is_touchable);
}

nex_err_t nextion_component_get_text(nextion_t *handle,
                                     const char *component_name,
                                     char *buffer,
                                     size_t *expected_length)
{
    return nextion_component_get_property_text(handle, component_name, "txt", buffer, expected_length);
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

nex_err_t nextion_component_set_text(nextion_t *handle, const char *component_name, char *text)
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
                                              size_t *expected_length)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK((component_name != NULL), "component_name error(NULL)", NEX_FAIL)
    CMP_CHECK((property_name != NULL), "property_name error(NULL)", NEX_FAIL)
    CMP_CHECK((buffer != NULL), "buffer error(NULL)", NEX_FAIL)
    CMP_CHECK((expected_length != NULL), "expected_length error(NULL)", NEX_FAIL)

    size_t command_length = 10 + NEX_DVC_COMPONENT_MAX_NAME_LENGTH;
    char command[10 + NEX_DVC_COMPONENT_MAX_NAME_LENGTH];

    snprintf(command, command_length, "get %s.%s", component_name, property_name);

    return nextion_system_get_text(handle, command, buffer, expected_length);
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

    size_t command_length = 10 + NEX_DVC_COMPONENT_MAX_NAME_LENGTH;
    char command[10 + NEX_DVC_COMPONENT_MAX_NAME_LENGTH];

    snprintf(command, command_length, "get %s.%s", component_name, property_name);

    return nextion_system_get_number(handle, command, number);
}

nex_err_t nextion_component_set_property_text(nextion_t *handle,
                                              const char *component_name,
                                              const char *property_name,
                                              char *text)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK((component_name != NULL), "component_name error(NULL)", NEX_FAIL)
    CMP_CHECK((property_name != NULL), "property_name error(NULL)", NEX_FAIL)
    CMP_CHECK((text != NULL), "text error(NULL)", NEX_FAIL)

    return nextion_command_send(handle, "%s.%s=\"%s\"", component_name, property_name, text);
}

nex_err_t nextion_component_set_property_number(nextion_t *handle,
                                                const char *component_name,
                                                const char *property_name,
                                                int32_t number)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK((component_name != NULL), "component_name error(NULL)", NEX_FAIL)
    CMP_CHECK((property_name != NULL), "property_name error(NULL)", NEX_FAIL)

    return nextion_command_send(handle, "%s.%s=%d", component_name, property_name, number);
}