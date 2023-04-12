#ifndef __ESP32_DRIVER_NEXTION_LOG_H__
#define __ESP32_DRIVER_NEXTION_LOG_H__

#include "esp_log.h"

#ifdef __cplusplus
extern "C"
{
#endif

    static const char *TAG = "nextion";

#define CMP_LOGD(format, ...) ESP_LOGD(TAG, format, ##__VA_ARGS__)
#define CMP_LOGV(format, ...) ESP_LOGV(TAG, format, ##__VA_ARGS__)
#define CMP_LOGI(format, ...) ESP_LOGI(TAG, format, ##__VA_ARGS__)
#define CMP_LOGW(format, ...) ESP_LOGW(TAG, format, ##__VA_ARGS__)
#define CMP_LOGE(format, ...) ESP_LOGE(TAG, format, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif
#endif
