#ifndef __NEXTION_PARSE_C__
#define __NEXTION_PARSE_C__

#include "nextion_constants.h"
#include "nextion_codes.h"
#include "nextion_common.h"
#include "nextion_parse.h"

#ifdef __cplusplus
extern "C"
{
#endif

    int nextion_parse_find_message_length(const uint8_t *buffer, int length)
    {
        NEX_CHECK((buffer != NULL), "buffer error(NULL)", -1);

        int end_count = 0;

        for (int i = 0; i < length; i++)
        {
            if (buffer[i] == NEX_DVC_CMD_END_VALUE)
            {
                end_count++;

                if (end_count == NEX_DVC_CMD_END_LENGTH)
                {
                    return i + 1;
                }

                continue;
            }

            end_count = 0;
        }

        return -1;
    }

    bool nextion_parse_assemble_event(const uint8_t *buffer, int length, nextion_event_t *event)
    {
        NEX_CHECK((buffer != NULL), "buffer error(NULL)", false);
        NEX_CHECK((length >= NEX_DVC_CMD_ACK_LENGTH), "length error(<NEX_DVC_CMD_ACK_LENGTH)", false);
        NEX_CHECK((event != NULL), "event error(NULL)", false);

        const uint8_t code = buffer[0];
        bool is_ok = true;

        switch (code)
        {
        case NEX_DVC_EVT_HARDWARE_START_RESET:
            event->type = NEXTION_EVENT_DEVICE;
            event->device_state = NEXTION_DEVICE_STARTED;
            break;

        case NEX_DVC_EVT_TOUCH_OCCURRED:
            if (length == 7)
            {
                event->type = NEXTION_EVENT_TOUCH;
                event->touch.page_id = buffer[1];
                event->touch.component_id = buffer[2];

                if (buffer[3] == 1U)
                {
                    event->touch.state = NEXTION_TOUCH_PRESSED;
                }
                else
                {
                    event->touch.state = NEXTION_TOUCH_RELEASED;
                }
            }
            else
            {
                is_ok = false;

                ESP_LOGW(NEXTION_TAG, "touch event length error(%d<>7)", length);
            }
            break;

        case NEX_DVC_EVT_TOUCH_COORDINATE_AWAKE:
        case NEX_DVC_EVT_TOUCH_COORDINATE_ASLEEP:
            if (length == 9)
            {
                event->type = NEXTION_EVENT_TOUCH_COORD;
                event->touch_coord.x = ((uint16_t)buffer[1] << 8) | (uint16_t)buffer[2];
                event->touch_coord.y = ((uint16_t)buffer[3] << 8) | (uint16_t)buffer[4];
                event->touch_coord.exited_sleep = code == NEX_DVC_EVT_TOUCH_COORDINATE_ASLEEP;

                if (buffer[5] == 1U)
                {
                    event->touch_coord.state = NEXTION_TOUCH_PRESSED;
                }
                else
                {
                    event->touch_coord.state = NEXTION_TOUCH_RELEASED;
                }
            }
            else
            {
                is_ok = false;

                ESP_LOGW(NEXTION_TAG, "touch coord length error(%d<>9)", length);
            }
            break;

        case NEX_DVC_EVT_HARDWARE_AUTO_SLEEP:
            event->type = NEXTION_EVENT_DEVICE;
            event->device_state = NEXTION_DEVICE_AUTO_SLEEP;
            break;

        case NEX_DVC_EVT_HARDWARE_AUTO_WAKE:
            event->type = NEXTION_EVENT_DEVICE;
            event->device_state = NEXTION_DEVICE_AUTO_WAKE;
            break;

        case NEX_DVC_EVT_HARDWARE_READY:
            event->type = NEXTION_EVENT_DEVICE;
            event->device_state = NEXTION_DEVICE_READY;
            break;

        case NEX_DVC_EVT_HARDWARE_UPGRADE:
            event->type = NEXTION_EVENT_DEVICE;
            event->device_state = NEXTION_DEVICE_UPGRADING;
            break;

        case NEX_DVC_EVT_TRANSPARENT_DATA_FINISHED:
            event->type = NEXTION_EVENT_DEVICE;
            event->device_state = NEXTION_DEVICE_TRANSP_DATA_FINISHED;
            break;

        case NEX_DVC_EVT_TRANSPARENT_DATA_READY:
            event->type = NEXTION_EVENT_DEVICE;
            event->device_state = NEXTION_DEVICE_TRANSP_DATA_READY;
            break;

        default:
            ESP_LOGE(NEXTION_TAG, "event code unknown %u", code);

            is_ok = false;
        }

        return is_ok;
    }

#ifdef __cplusplus
}
#endif

#endif //__NEXTION_PARSE_C__