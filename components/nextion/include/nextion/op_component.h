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
    nex_err_t nextion_refresh_component(char *component_name_or_id);

#ifdef __cplusplus
}
#endif

#endif //__NEXTION_OP_COMPONENT_H__