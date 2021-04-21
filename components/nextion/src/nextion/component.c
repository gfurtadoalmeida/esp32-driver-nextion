#include "common_infra.h"
#include "nextion/nextion.h"
#include "nextion/system.h"
#include "nextion/component.h"

#ifdef __cplusplus
extern "C"
{
#endif

    nex_err_t nextion_component_refresh(nextion_handle_t handle, const char *component_name_or_id)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);
        NEX_CHECK((component_name_or_id != NULL), "component_name_or_id error(NULL)", NEX_FAIL);

        return nextion_command_send(handle, "ref %s", component_name_or_id);
    }

    nex_err_t nextion_component_set_visibility(nextion_handle_t handle, const char *component_name_or_id, bool visible_or_not)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);
        NEX_CHECK((component_name_or_id != NULL), "component_name_or_id error(NULL)", NEX_FAIL);

        return nextion_command_send(handle, "vis %s,%d", component_name_or_id, visible_or_not);
    }

    nex_err_t nextion_component_set_visibility_all(nextion_handle_t handle, bool visible_or_not)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);

        return nextion_command_send(handle, "vis 255,%d", visible_or_not);
    }

    nex_err_t nextion_component_set_touchable(nextion_handle_t handle, const char *component_name_or_id, bool touchable_or_not)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);
        NEX_CHECK((component_name_or_id != NULL), "component_name_or_id error(NULL)", NEX_FAIL);

        return nextion_command_send(handle, "tsw %s,%d", component_name_or_id, touchable_or_not);
    }

    nex_err_t nextion_component_set_touchable_all(nextion_handle_t handle, bool touchable_or_not)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);

        return nextion_command_send(handle, "tsw 255,%d", touchable_or_not);
    }

    nex_err_t nextion_component_get_text(nextion_handle_t handle,
                                         const char *component_name,
                                         char *buffer,
                                         size_t *expected_length)
    {
        return nextion_component_get_property_text(handle, component_name, "txt", buffer, expected_length);
    }

    nex_err_t nextion_component_get_value(nextion_handle_t handle, const char *component_name, int32_t *number)
    {
        return nextion_component_get_property_number(handle, component_name, "val", number);
    }

    nex_err_t nextion_component_get_boolean(nextion_handle_t handle, const char *component_name, bool *value)
    {
        int32_t temp = 0;
        nex_err_t code = nextion_component_get_property_number(handle, component_name, "val", &temp);

        *value = (bool)temp;

        return code;
    }

    nex_err_t nextion_component_set_text(nextion_handle_t handle, const char *component_name, char *text)
    {
        return nextion_component_set_property_text(handle, component_name, "txt", text);
    }

    nex_err_t nextion_component_set_value(nextion_handle_t handle, const char *component_name, int32_t number)
    {
        return nextion_component_set_property_number(handle, component_name, "val", number);
    }

    nex_err_t nextion_component_set_boolean(nextion_handle_t handle, const char *component_name, bool value)
    {
        return nextion_component_set_property_number(handle, component_name, "val", (int32_t)value);
    }

    nex_err_t nextion_component_get_property_text(nextion_handle_t handle,
                                                  const char *component_name,
                                                  const char *property_name,
                                                  char *buffer,
                                                  size_t *expected_length)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);
        NEX_CHECK((component_name != NULL), "component_name error(NULL)", NEX_FAIL);
        NEX_CHECK((property_name != NULL), "property_name error(NULL)", NEX_FAIL);
        NEX_CHECK((buffer != NULL), "buffer error(NULL)", NEX_FAIL);
        NEX_CHECK((expected_length != NULL), "expected_length error(NULL)", NEX_FAIL);

        size_t command_length = 10 + NEX_DVC_COMPONENT_MAX_NAME_LENGTH;
        char command[10 + NEX_DVC_COMPONENT_MAX_NAME_LENGTH];

        snprintf(command, command_length, "get %s.%s", component_name, property_name);

        return nextion_system_get_text(handle, command, buffer, expected_length);
    }

    nex_err_t nextion_component_get_property_number(nextion_handle_t handle,
                                                    const char *component_name,
                                                    const char *property_name,
                                                    int32_t *number)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);
        NEX_CHECK((component_name != NULL), "component_name error(NULL)", NEX_FAIL);
        NEX_CHECK((property_name != NULL), "property_name error(NULL)", NEX_FAIL);
        NEX_CHECK((number != NULL), "number error(NULL)", NEX_FAIL);

        size_t command_length = 10 + NEX_DVC_COMPONENT_MAX_NAME_LENGTH;
        char command[10 + NEX_DVC_COMPONENT_MAX_NAME_LENGTH];

        snprintf(command, command_length, "get %s.%s", component_name, property_name);

        return nextion_system_get_number(handle, command, number);
    }

    nex_err_t nextion_component_set_property_text(nextion_handle_t handle,
                                                  const char *component_name,
                                                  const char *property_name,
                                                  char *text)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);
        NEX_CHECK((component_name != NULL), "component_name error(NULL)", NEX_FAIL);
        NEX_CHECK((property_name != NULL), "property_name error(NULL)", NEX_FAIL);
        NEX_CHECK((text != NULL), "text error(NULL)", NEX_FAIL);

        return nextion_command_send(handle, "%s.%s=\"%s\"", component_name, property_name, text);
    }

    nex_err_t nextion_component_set_property_number(nextion_handle_t handle,
                                                    const char *component_name,
                                                    const char *property_name,
                                                    int32_t number)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);
        NEX_CHECK((component_name != NULL), "component_name error(NULL)", NEX_FAIL);
        NEX_CHECK((property_name != NULL), "property_name error(NULL)", NEX_FAIL);

        return nextion_command_send(handle, "%s.%s=%d", component_name, property_name, number);
    }

#ifdef __cplusplus
}
#endif