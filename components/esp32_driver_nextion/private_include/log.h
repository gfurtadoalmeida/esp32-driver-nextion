#ifndef __ESP32_DRIVER_NEXTION_LOG_H__
#define __ESP32_DRIVER_NEXTION_LOG_H__

#include "esp_log.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /** @brief Description text used for logging. */
    static const char *TAG = "NEXTION";

/**
 * @brief Log a verbose message.
 * @param format String format.
 * @param ... Arguments used by the string format.
 */
#define CMP_LOGV(format, ...) ESP_LOGV(TAG, format, ##__VA_ARGS__)
/**
 * @brief Log a debug message.
 * @param format String format.
 * @param ... Arguments used by the string format.
 */
#define CMP_LOGD(format, ...) ESP_LOGD(TAG, format, ##__VA_ARGS__)
/**
 * @brief Log an informational message.
 * @param format String format.
 * @param ... Arguments used by the string format.
 */
#define CMP_LOGI(format, ...) ESP_LOGI(TAG, format, ##__VA_ARGS__)
/**
 * @brief Log a warning message.
 * @param format String format.
 * @param ... Arguments used by the string format.
 */
#define CMP_LOGW(format, ...) ESP_LOGW(TAG, format, ##__VA_ARGS__)
/**
 * @brief Log an error message.
 * @param format String format.
 * @param ... Arguments used by the string format.
 */
#define CMP_LOGE(format, ...) ESP_LOGE(TAG, format, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif
#endif
