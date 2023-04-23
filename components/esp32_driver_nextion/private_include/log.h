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
/**
 * @brief Log a buffer of characters, separated into 16 bytes each line,
 * at info level.
 * @note Buffer should contain only printable characters.
 * @param tag Description tag.
 * @param buffer Buffer array.
 * @param length Length of buffer in bytes.
 */
#define CMP_LOG_BUFFER_CHAR(tag, buffer, length) ESP_LOG_BUFFER_CHAR_LEVEL(tag, buffer, length, ESP_LOG_INFO)
/**
 * @brief Log a buffer of hex bytes at info level.
 * @param tag Description tag.
 * @param buffer Buffer array.
 * @param length Length of buffer in bytes.
 */
#define CMP_LOG_BUFFER_HEX(tag, buffer, length) ESP_LOG_BUFFER_HEXDUMP(tag, buffer, length, ESP_LOG_INFO)

#ifdef __cplusplus
}
#endif
#endif
