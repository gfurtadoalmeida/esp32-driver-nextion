#ifndef __NEXTION_BUFFER_C__
#define __NEXTION_BUFFER_C__

#include "nextion_constants.h"
#include "nextion_codes.h"
#include "nextion_common.h"
#include "nextion_buffer.h"

#ifdef __cplusplus
extern "C"
{
#endif

    int nextion_buffer_find_message_length(const uint8_t *buffer, int length)
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

    bool nextion_buffer_assemble_event(uint8_t code, const uint8_t *buffer, int length, nextion_event_t *event)
    {
        NEX_CHECK((buffer != NULL), "buffer error(NULL)", false);
        NEX_CHECK((length >= NEX_DVC_CMD_ACK_LENGTH), "length error(<NEX_DVC_SERIAL_ACK_LENGTH>)", false);
        NEX_CHECK((event != NULL), "event error(NULL)", false);

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

                ESP_LOGW(NEXTION_TAG, "touch event with invalid length (%d<7)", length);
            }

            break;

        case NEX_DVC_EVT_TOUCH_COORDINATE_AWAKE:
        case NEX_DVC_EVT_TOUCH_COORDINATE_ASLEEP:
        {
            is_ok = false;

            ESP_LOGE(NEXTION_TAG, "touch coord event is not supported");
        }
        break;

        case NEX_DVC_EVT_HARDWARE_SLEEP_AUTOMATIC:
            event->type = NEXTION_EVENT_DEVICE;
            event->device_state = NEXTION_DEVICE_AUTO_SLEEP;
            break;

        case NEX_DVC_EVT_HARDWARE_WAKE_AUTOMATIC:
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
            ESP_LOGW(NEXTION_TAG, "event with unknown code %u", code);

            is_ok = false;
        }

        return is_ok;
    }

#ifdef __cplusplus
}
#endif

#endif //__NEXTION_BUFFER_C__