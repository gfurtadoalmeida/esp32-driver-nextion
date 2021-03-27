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
        NEX_CHECK_HANDLE(handle, NEX_FAIL);
        NEX_CHECK((component_name != NULL), "component_name error(NULL)", NEX_FAIL);
        NEX_CHECK((buffer != NULL), "text error(NULL)", NEX_FAIL);

        size_t command_length = 8 + NEX_DVC_COMPONENT_MAX_NAME_LENGTH;
        char command[8 + NEX_DVC_COMPONENT_MAX_NAME_LENGTH];

        snprintf(command, command_length, "get %s.txt", component_name);

        return nextion_system_get_text(handle, command, buffer, expected_length);
    }

    nex_err_t nextion_component_get_number(nextion_handle_t handle, const char *component_name, int32_t *number)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);
        NEX_CHECK((component_name != NULL), "component_name error(NULL)", NEX_FAIL);
        NEX_CHECK((number != NULL), "number error(NULL)", NEX_FAIL);

        size_t command_length = 8 + NEX_DVC_COMPONENT_MAX_NAME_LENGTH;
        char command[8 + NEX_DVC_COMPONENT_MAX_NAME_LENGTH];

        snprintf(command, command_length, "get %s.val", component_name);

        return nextion_system_get_number(handle, command, number);
    }

    nex_err_t nextion_component_get_boolean(nextion_handle_t handle, const char *component_name, bool *value)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);
        NEX_CHECK((component_name != NULL), "component_name error(NULL)", NEX_FAIL);
        NEX_CHECK((value != NULL), "value error(NULL)", NEX_FAIL);

        int32_t temp;
        nex_err_t code = nextion_component_get_number(handle, component_name, &temp);

        *value = (bool)temp;

        return code;
    }

    nex_err_t nextion_component_set_text(nextion_handle_t handle,
                                         const char *component_name,
                                         char *text)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);
        NEX_CHECK((component_name != NULL), "component_name error(NULL)", NEX_FAIL);
        NEX_CHECK((text != NULL), "text error(NULL)", NEX_FAIL);

        return nextion_command_send(handle, "%s.txt=\"%s\"", component_name, text);
    }

    nex_err_t nextion_component_set_number(nextion_handle_t handle, const char *component_name, int32_t number)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);
        NEX_CHECK((component_name != NULL), "component_name error(NULL)", NEX_FAIL);

        return nextion_command_send(handle, "%s.val=%d", component_name, number);
    }

    nex_err_t nextion_component_set_boolean(nextion_handle_t handle, const char *component_name, bool value)
    {
        NEX_CHECK_HANDLE(handle, NEX_FAIL);
        NEX_CHECK((component_name != NULL), "component_name error(NULL)", NEX_FAIL);

        return nextion_component_set_number(handle, component_name, (int32_t)value);
    }

#ifdef __cplusplus
}
#endif