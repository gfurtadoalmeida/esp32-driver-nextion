#ifndef __ESP32_DRIVER_NEXTION_ASSERTION_H__
#define __ESP32_DRIVER_NEXTION_ASSERTION_H__

#include "log.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Returns a given value if the condition passed is false.
 *
 * @param condition Condition to evaluate.
 * @param message Message to be logged if the condition is false.
 * @param return_value Value to be returned if the condition is false.
 *
 * @return If the condition is false the method will return what was
 * passed on @ref return_value parameter, otherwise the method will
 * continue.
 */
#define CMP_CHECK(condition, message, return_value)              \
    if (!(condition))                                            \
    {                                                            \
        CMP_LOGE("%s(%d): %s", __FUNCTION__, __LINE__, message); \
        return (return_value);                                   \
    }

/**
 * @brief Checks if a Nextion handle is valid.
 * @param handle Nextion handle.
 * @param return_value Value to be returned if the condition fails.
 * @return If the condition fails the method will return what was passed on
 * "return_value" parameter, otherwise the method will continue.
 */
#define CMP_CHECK_HANDLE(handle, return_value) CMP_CHECK((handle != NULL), "handle error(NULL)", return_value)

#ifdef __cplusplus
}
#endif
#endif
