#ifndef __NEXTION_OP_COMPONENT_H__
#define __NEXTION_OP_COMPONENT_H__

#include <stdint.h>
#include "codes.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Sends a command that retrieves a null-terminated string.
     *
     * @details It's the caller responsibility to allocate a buffer big enough to
     * hold the text returned.
     *
     * @param command A null-terminated string with the command to be sent.
     * @param[in] text Location where the retrieved text will be stored. Must take the null-terminator into account.
     * @param[i] text_length Location where the text length will be stored.
     *
     * @return NEX_OK or NEX_DVC_ERR_INVALID_VARIABLE_OR_ATTRIBUTE.
     */
    nex_err_t nextion_get_text(const char *command, char *text, int *text_length);

    /**
     * @brief Sends a command that retrieves a signed number.
     *
     * @param command A null-terminated string with the command to be sent.
     * @param[in] number Location where the retrieved number will be stored.
     *
     * @return NEX_OK or NEX_DVC_ERR_INVALID_VARIABLE_OR_ATTRIBUTE.
     */
    nex_err_t nextion_get_number(const char *command, int32_t *number);

    /**
     * @brief Refreshes a component, bringing it to front.
     * @param command A null-terminated string with the component's name or id.
     * @return NEX_OK or NEX_DVC_ERR_INVALID_COMPONENT.
     */
    nex_err_t nextion_component_refresh(char *component_name_or_id);

    /**
     * @brief Set a component visibility.
     *
     * @param command A null-terminated string with the component's name or id.
     * @param visible_or_not True make it visible; false make it invisible.
     *
     * @return NEX_OK or NEX_DVC_ERR_INVALID_COMPONENT.
     */
    nex_err_t nextion_component_set_visibility(char *component_name_or_id, bool visible_or_not);

    /**
     * @brief Set all components visibility.
     * @note Only for the current page.
     * @param visible_or_not True make them visible; false make them invisible.
     * @return NEX_OK or NEX_DVC_ERR_INVALID_COMPONENT.
     */
    nex_err_t nextion_component_set_visibility_all(bool visible_or_not);

    /**
     * @brief Set if a component can respond to touch events or not.
     *
     * @param command A null-terminated string with the component's name or id.
     * @param touchable_or_not True make it touchable; false make it untouchable.
     *
     * @return NEX_OK or NEX_DVC_ERR_INVALID_COMPONENT.
     */
    nex_err_t nextion_component_set_touchable(char *component_name_or_id, bool touchable_or_not);

    /**
     * @brief Set if all components can respond to touch events or not.
     * @note Only for the current page.
     * @param touchable_or_not True make them touchable; false make them untouchable.
     * @return NEX_OK or NEX_DVC_ERR_INVALID_COMPONENT.
     */
    nex_err_t nextion_component_set_touchable_all(bool touchable_or_not);

#ifdef __cplusplus
}
#endif

#endif //__NEXTION_OP_COMPONENT_H__