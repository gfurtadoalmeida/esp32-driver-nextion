#include <string.h>
#include "common_infra.h"
#include "nextion/nextion.h"
#include "nextion/op_component.h"

#ifdef __cplusplus
extern "C"
{
#endif

    nex_err_t nextion_component_refresh(const char *component_name_or_id)
    {
        const int name_length = strlen(component_name_or_id);

        return NEX_SEND_COMMAND(4 + name_length, "ref %s", component_name_or_id);
    }

    nex_err_t nextion_component_set_visibility(const char *component_name_or_id, bool visible_or_not)
    {
        const int name_length = strlen(component_name_or_id);

        return NEX_SEND_COMMAND(6 + name_length, "vis %s,%d", component_name_or_id, visible_or_not);
    }

    nex_err_t nextion_component_set_visibility_all(bool visible_or_not)
    {
        return NEX_SEND_COMMAND(9, "vis 255,%d", visible_or_not);
    }

    nex_err_t nextion_component_set_touchable(const char *component_name_or_id, bool touchable_or_not)
    {
        const int name_length = strlen(component_name_or_id);

        return NEX_SEND_COMMAND(6 + name_length, "tsw %s,%d", component_name_or_id, touchable_or_not);
    }

    nex_err_t nextion_component_set_touchable_all(bool touchable_or_not)
    {
        return NEX_SEND_COMMAND(9, "tsw 255,%d", touchable_or_not);
    }

#ifdef __cplusplus
}
#endif