#include "common.h"
#include "nextion/parse.h"
#include "nextion/constants.h"
#include "nextion/codes.h"

#ifdef __cplusplus
extern "C"
{
#endif

    int nextion_parse_find_message_length(const ringbuffer_handle_t ring_buffer)
    {
        NEX_CHECK((ring_buffer != NULL), "ring_buffer error(NULL)", -1);

        int end_count = 0;
        int holder = 0;
        uint8_t value = 0;

        while (ringbuffer_peek(ring_buffer, &holder, &value))
        {
            if (value == NEX_DVC_CMD_END_VALUE)
            {
                end_count++;

                if (end_count == NEX_DVC_CMD_END_LENGTH)
                {
                    return holder;
                }

                continue;
            }

            end_count = 0;
        }

        return -1;
    }

    bool nextion_parse_assemble_event(const ringbuffer_handle_t ring_buffer, const int message_length, nextion_event_t *event)
    {
        NEX_CHECK((ring_buffer != NULL), "ring_buffer error(NULL)", false);
        NEX_CHECK((message_length >= NEX_DVC_CMD_ACK_LENGTH), "message_length error(<NEX_DVC_CMD_ACK_LENGTH)", false);
        NEX_CHECK((event != NULL), "event error(NULL)", false);

        uint8_t buffer[NEX_DVC_EVT_MAX_RESPONSE_LENGTH];

        if (!ringbuffer_read_bytes(ring_buffer, buffer, message_length))
        {
            ESP_LOGE(NEXTION_TAG, "could not read from ring buffer");

            return false;
        }

        const uint8_t code = buffer[0];
        bool is_ok = true;

        switch (code)
        {
        case NEX_DVC_EVT_HARDWARE_START_RESET:
            event->type = NEXTION_EVENT_DEVICE;
            event->device_state = NEXTION_DEVICE_STARTED;
            break;

        case NEX_DVC_EVT_TOUCH_OCCURRED:
            if (message_length == 7)
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

                ESP_LOGW(NEXTION_TAG, "touch event length error(%d<>7)", message_length);
            }
            break;

        case NEX_DVC_EVT_TOUCH_COORDINATE_AWAKE:
        case NEX_DVC_EVT_TOUCH_COORDINATE_ASLEEP:
            if (message_length == 9)
            {
                event->type = NEXTION_EVENT_TOUCH_COORD;
                // Coordinates: 2 bytes and unsigned = uint16_t.
                // Sent in big endian format.
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

                ESP_LOGW(NEXTION_TAG, "touch coord length error(%d<>9)", message_length);
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
