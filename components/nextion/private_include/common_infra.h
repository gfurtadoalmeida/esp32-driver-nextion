#ifndef __NEXTION_ESP32_COMMON_INFRA_H__
#define __NEXTION_ESP32_COMMON_INFRA_H__

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
 * @brief Logs an error.
 * @param format Text to be logged. Supports formats.
 * @param ... Values used on the format.
 */
#define NEX_LOGE(format, ...) ESP_LOGE(NEXTION_TAG, format, ##__VA_ARGS__)

/**
 * @brief Logs a warning.
 * @param format Text to be logged. Supports formats.
 * @param ... Values used on the format.
 */
#define NEX_LOGW(format, ...) ESP_LOGW(NEXTION_TAG, format, ##__VA_ARGS__)

/**
 * @brief Logs an information.
 * @param format Text to be logged. Supports formats.
 * @param ... Values used on the format.
 */
#define NEX_LOGI(format, ...) ESP_LOGI(NEXTION_TAG, format, ##__VA_ARGS__)

/**
 * @brief Logs for debug.
 * @param format Text to be logged. Supports formats.
 * @param ... Values used on the format.
 */
#define NEX_LOGD(format, ...) ESP_LOGD(NEXTION_TAG, format, ##__VA_ARGS__)

/**
 * @brief Logs for verbose.
 * @param format Text to be logged. Supports formats.
 * @param ... Values used on the format.
 */
#define NEX_LOGV(format, ...) ESP_LOGV(NEXTION_TAG, format, ##__VA_ARGS__)

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
#define NEX_CHECK(condition, message, return_value)              \
    if (!(condition))                                            \
    {                                                            \
        NEX_LOGE("%s(%d): %s", __FUNCTION__, __LINE__, message); \
        return (return_value);                                   \
    }

#ifdef __cplusplus
}
#endif

#endif //__NEXTION_ESP32_COMMON_INFRA_H__