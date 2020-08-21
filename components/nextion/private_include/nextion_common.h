#ifndef __NEXTION_COMMON_H__
#define __NEXTION_COMMON_H__

#include "esp_log.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * Tag used on ESP_LOG* macros.
     */
    static const char *NEXTION_TAG = "nextion";

/**
 * @brief Macro that forces a method to return a value
 * based on a failed condition.
 *
 * @param condition Condition to evaluate.
 * @param message Message to be written if the condition fails.
 * @param return_value Value to be returned if the condition fails.
 *
 * @return If the condition fails the method will return what was passed on
 * "return_value" parameter, otherwise the method will continue.
 */
#define NEX_CHECK(condition, message, return_value)                           \
    if (!(condition))                                                         \
    {                                                                         \
        ESP_LOGE(NEXTION_TAG, "%s(%d): %s", __FUNCTION__, __LINE__, message); \
        return (return_value);                                                \
    }

#ifdef __cplusplus
}
#endif

#endif //__NEXTION_COMMON_H__