#ifndef __ESP32_DRIVER_NEXTION_COMPONENT_H__
#define __ESP32_DRIVER_NEXTION_COMPONENT_H__

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "base/codes.h"
#include "base/types.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Refresh a component, bringing it to front.
     * @param[in] handle Nextion context pointer.
     * @param[in] component_name_or_id A null-terminated string with the component's name or id.
     * @return NEX_OK or NEX_FAIL | NEX_DVC_ERR_INVALID_COMPONENT.
     */
    nex_err_t nextion_component_refresh(nextion_t *handle, const char *component_name_or_id);

    /**
     * @brief Set a component visibility.
     * @param[in] handle Nextion context pointer.
     * @param[in] component_name_or_id A null-terminated string with the component's name or id.
     * @param[in] is_visible True make it visible; false make it invisible.
     * @return NEX_OK or NEX_FAIL | NEX_DVC_ERR_INVALID_COMPONENT.
     */
    nex_err_t nextion_component_set_visibility(nextion_t *handle,
                                               const char *component_name_or_id,
                                               bool is_visible);

    /**
     * @brief Set all components visibility.
     * @note Only for the current page.
     * @param[in] handle Nextion context pointer.
     * @param[in] is_visible True make them visible; false make them invisible.
     * @return NEX_OK or NEX_DVC_ERR_INVALID_COMPONENT.
     */
    nex_err_t nextion_component_set_visibility_all(nextion_t *handle, bool is_visible);

    /**
     * @brief Set if a component will respond to touch events or not.
     * @param[in] handle Nextion context pointer.
     * @param[in] component_name_or_id A null-terminated string with the component's name or id.
     * @param[in] is_touchable True make it touchable; false make it untouchable.
     * @return NEX_OK or NEX_FAIL | NEX_DVC_ERR_INVALID_COMPONENT.
     */
    nex_err_t nextion_component_set_touchable(nextion_t *handle,
                                              const char *component_name_or_id,
                                              bool is_touchable);

    /**
     * @brief Set if all components will respond to touch events or not.
     * @note Only for the current page.
     * @param[in] handle Nextion context pointer.
     * @param[in] is_touchable True make them touchable; false make them untouchable.
     * @return NEX_OK or NEX_DVC_ERR_INVALID_COMPONENT.
     */
    nex_err_t nextion_component_set_touchable_all(nextion_t *handle, bool is_touchable);

    /**
     * @brief Get a component ".txt" value.
     * @note Shorthand for "nextion_component_get_property_text" using "txt" property.
     * @note It's the caller responsibility to allocate a buffer big enough to
     * hold the text returned.
     * @param[in] handle Nextion context pointer.
     * @param[in] component_name A null-terminated string with the component name.
     * @param[out] buffer Location where the retrieved text will be stored. Must take the null-terminator into account.
     * @param[in] buffer_length Buffer length.
     *
     * @return NEX_OK or NEX_FAIL | NEX_DVC_ERR_INVALID_VARIABLE_OR_ATTRIBUTE | NEX_DVC_ERR_INVALID_COMPONENT.
     */
    nex_err_t nextion_component_get_text(nextion_t *handle,
                                         const char *component_name,
                                         char *buffer,
                                         size_t buffer_length);

    /**
     * @brief Get a component ".val" value.
     * @note Shorthand for "nextion_component_get_property_number" using "val" property.
     * @param[in] handle Nextion context pointer.
     * @param[in] component_name A null-terminated string with the component name.
     * @param[out] number Location where the retrieved number will be stored.
     * @return NEX_OK or NEX_FAIL | NEX_DVC_ERR_INVALID_VARIABLE_OR_ATTRIBUTE | NEX_DVC_ERR_INVALID_COMPONENT.
     */
    nex_err_t nextion_component_get_value(nextion_t *handle,
                                          const char *component_name,
                                          int32_t *number);

    /**
     * @brief Get a component ".val" value converted to boolean.
     * @note Shorthand for "nextion_component_get_property_number" using "val" property
     * and converting to boolean.
     * @param[in] handle Nextion context pointer.
     * @param[in] component_name A null-terminated string with the component name.
     * @param[out] value Location where the retrieved boolean will be stored.
     * @return NEX_OK or NEX_FAIL | NEX_DVC_ERR_INVALID_VARIABLE_OR_ATTRIBUTE | NEX_DVC_ERR_INVALID_COMPONENT.
     */
    nex_err_t nextion_component_get_boolean(nextion_t *handle,
                                            const char *component_name,
                                            bool *value);

    /**
     * @brief Set a component ".txt" value.
     * @note Shorthand for "nextion_component_set_property_text" using "txt" property.
     * @param[in] handle Nextion context pointer.
     * @param[in] component_name A null-terminated string with the component name.
     * @param[in] text A null-terminated string with the text.
     * @return NEX_OK or NEX_FAIL | NEX_DVC_ERR_INVALID_VARIABLE_OR_ATTRIBUTE | NEX_DVC_ERR_INVALID_COMPONENT.
     */
    nex_err_t nextion_component_set_text(nextion_t *handle,
                                         const char *component_name,
                                         const char *text);

    /**
     * @brief Set a component ".val" value.
     * @note Shorthand for "nextion_component_set_property_number" using "val" property.
     * @param[in] handle Nextion context pointer.
     * @param[in] component_name A null-terminated string with the component name.
     * @param[in] number Value to be sent.
     * @return NEX_OK or NEX_FAIL | NEX_DVC_ERR_INVALID_VARIABLE_OR_ATTRIBUTE | NEX_DVC_ERR_INVALID_COMPONENT.
     */
    nex_err_t nextion_component_set_value(nextion_t *handle,
                                          const char *component_name,
                                          int32_t number);

    /**
     * @brief Set a component ".val" value, converted as boolean.
     * @note Shorthand for "nextion_component_set_property_number" using "val" property
     * and converting to boolean.
     * @param[in] handle Nextion context pointer.
     * @param[in] component_name A null-terminated string with the component name.
     * @param[in] value Value to be sent.
     * @return NEX_OK or NEX_FAIL | NEX_DVC_ERR_INVALID_VARIABLE_OR_ATTRIBUTE | NEX_DVC_ERR_INVALID_COMPONENT.
     */
    nex_err_t nextion_component_set_boolean(nextion_t *handle,
                                            const char *component_name,
                                            bool value);

    /**
     * @brief Get a text from a component property.
     * @note It's the caller responsibility to allocate a buffer big enough to
     * hold the text returned.
     * @param[in] handle Nextion context pointer.
     * @param[in] component_name A null-terminated string with the component name.
     * @param[in] property_name A null-terminated string with the property name to retrieve the text from.
     * @param[out] buffer Location where the retrieved text will be stored. Must take the null-terminator into account.
     * @param[in] buffer_length Buffer length.
     * @return NEX_OK or NEX_FAIL | NEX_DVC_ERR_INVALID_VARIABLE_OR_ATTRIBUTE | NEX_DVC_ERR_INVALID_COMPONENT.
     */
    nex_err_t nextion_component_get_property_text(nextion_t *handle,
                                                  const char *component_name,
                                                  const char *property_name,
                                                  char *buffer,
                                                  size_t expected_length);

    /**
     * @brief Get a number from a component property.
     * @param[in] handle Nextion context pointer.
     * @param[in] component_name A null-terminated string with the component name.
     * @param[out] property_name A null-terminated string with the property name to retrieve the number from.
     * @param[in] number Location where the retrieved number will be stored.
     * @return NEX_OK or NEX_FAIL | NEX_DVC_ERR_INVALID_VARIABLE_OR_ATTRIBUTE | NEX_DVC_ERR_INVALID_COMPONENT.
     */
    nex_err_t nextion_component_get_property_number(nextion_t *handle,
                                                    const char *component_name,
                                                    const char *property_name,
                                                    int32_t *number);

    /**
     * @brief Set a component property with text.
     * @param[in] handle Nextion context pointer.
     * @param[in] component_name A null-terminated string with the component name.
     * @param[in] property_name A null-terminated string with the property name to set the text.
     * @param[in] text A null-terminated string with the text.
     * @param text_length Text lenght.
     * @return NEX_OK or NEX_FAIL | NEX_DVC_ERR_INVALID_VARIABLE_OR_ATTRIBUTE | NEX_DVC_ERR_INVALID_COMPONENT.
     */
    nex_err_t nextion_component_set_property_text(nextion_t *handle,
                                                  const char *component_name,
                                                  const char *property_name,
                                                  const char *text);

    /**
     * @brief Set a component property with number.
     * @param[in] handle Nextion context pointer.
     * @param[in] component_name A null-terminated string with the component name.
     * @param[in] property_name A null-terminated string with the property name to set the value.
     * @param[in] number Value to be sent.
     * @return NEX_OK or NEX_FAIL | NEX_DVC_ERR_INVALID_VARIABLE_OR_ATTRIBUTE | NEX_DVC_ERR_INVALID_COMPONENT.
     */
    nex_err_t nextion_component_set_property_number(nextion_t *handle,
                                                    const char *component_name,
                                                    const char *property_name,
                                                    int32_t number);

#ifdef __cplusplus
}
#endif
#endif