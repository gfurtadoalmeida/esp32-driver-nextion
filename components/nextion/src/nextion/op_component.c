#include <string.h>
#include "common_infra.h"
#include "nextion/nextion.h"
#include "nextion/op_system.h"
#include "nextion/op_component.h"

#ifdef __cplusplus
extern "C"
{
#endif

    nex_err_t nextion_component_refresh(const char *component_name_or_id)
    {
        NEX_CHECK((component_name_or_id != NULL), "component_name_or_id error(NULL)", NEX_FAIL);

        const int name_length = strlen(component_name_or_id);

        return NEX_SEND_COMMAND(4 + name_length, "ref %s", component_name_or_id);
    }

    nex_err_t nextion_component_set_visibility(const char *component_name_or_id, bool visible_or_not)
    {
        NEX_CHECK((component_name_or_id != NULL), "component_name_or_id error(NULL)", NEX_FAIL);

        const int name_length = strlen(component_name_or_id);

        return NEX_SEND_COMMAND(6 + name_length, "vis %s,%d", component_name_or_id, visible_or_not);
    }

    nex_err_t nextion_component_set_visibility_all(bool visible_or_not)
    {
        return NEX_SEND_COMMAND(9, "vis 255,%d", visible_or_not);
    }

    nex_err_t nextion_component_set_touchable(const char *component_name_or_id, bool touchable_or_not)
    {
        NEX_CHECK((component_name_or_id != NULL), "component_name_or_id error(NULL)", NEX_FAIL);

        const int name_length = strlen(component_name_or_id);

        return NEX_SEND_COMMAND(6 + name_length, "tsw %s,%d", component_name_or_id, touchable_or_not);
    }

    nex_err_t nextion_component_set_touchable_all(bool touchable_or_not)
    {
        return NEX_SEND_COMMAND(9, "tsw 255,%d", touchable_or_not);
    }

    nex_err_t nextion_component_get_text(const char *component_name, char *text, int *text_length)
    {
        NEX_CHECK((component_name != NULL), "component_name error(NULL)", NEX_FAIL);
        NEX_CHECK((text != NULL), "text error(NULL)", NEX_FAIL);
        NEX_CHECK((text_length != NULL), "text_length error(NULL)", NEX_FAIL);

        const int command_length = strlen(component_name) + 8;
        char command[command_length];

        sprintf(command, "get %s.txt", component_name);

        return nextion_system_get_text(command, text, text_length);
    }

    nex_err_t nextion_component_get_number(const char *component_name, int32_t *number)
    {
        NEX_CHECK((component_name != NULL), "component_name error(NULL)", NEX_FAIL);
        NEX_CHECK((number != NULL), "number error(NULL)", NEX_FAIL);

        const int command_length = strlen(component_name) + 8;
        char command[command_length];

        sprintf(command, "get %s.val", component_name);

        return nextion_system_get_number(command, number);
    }

    nex_err_t nextion_component_get_boolean(const char *component_name, bool *value)
    {
        NEX_CHECK((component_name != NULL), "component_name error(NULL)", NEX_FAIL);
        NEX_CHECK((value != NULL), "value error(NULL)", NEX_FAIL);

        int32_t temp;
        nex_err_t code = nextion_component_get_number(component_name, &temp);

        *value = (bool)temp;

        return code;
    }

    nex_err_t nextion_component_set_text(const char *component_name, char *text)
    {
        NEX_CHECK((component_name != NULL), "component_name error(NULL)", NEX_FAIL);
        NEX_CHECK((text != NULL), "text error(NULL)", NEX_FAIL);

        const int command_length = strlen(component_name) + strlen(text) + 8;

        return NEX_SEND_COMMAND(command_length, "%s.txt=\"%s\"", component_name, text);
    }

    nex_err_t nextion_component_set_number(const char *component_name, int32_t number)
    {
        NEX_CHECK((component_name != NULL), "component_name error(NULL)", NEX_FAIL);

        const int command_length = strlen(component_name) + 15; // int max digits = 10

        return NEX_SEND_COMMAND(command_length, "%s.val=%d", component_name, number);
    }

    nex_err_t nextion_component_set_boolean(const char *component_name, bool value)
    {
        NEX_CHECK((component_name != NULL), "component_name error(NULL)", NEX_FAIL);

        return nextion_component_set_number(component_name, (int32_t)value);
    }

#ifdef __cplusplus
}
#endif