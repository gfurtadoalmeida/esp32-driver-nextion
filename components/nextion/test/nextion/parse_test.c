#include "unity.h"
#include "nextion/constants.h"
#include "nextion/codes.h"
#include "nextion/parse.h"
#include "ringbuffer/ringbuffer.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define TEST_ASSERT_EVENT_ASSEMBLE(buffer, message_length, event)                        \
    ringbuffer_handle_t ring_buffer = ringbuffer_create(message_length);                 \
    ringbuffer_write_bytes(ring_buffer, buffer, message_length);                         \
    TEST_ASSERT_TRUE(nextion_parse_assemble_event(ring_buffer, message_length, &event)); \
    ringbuffer_free(ring_buffer);

#define TEST_ASSERT_EVENT_ASSEMBLE_DEVICE_STATE(event_code, expected_device_state)                               \
    nextion_event_t event;                                                                                       \
    const uint8_t buffer[4] = {event_code, NEX_DVC_CMD_END_VALUE, NEX_DVC_CMD_END_VALUE, NEX_DVC_CMD_END_VALUE}; \
    TEST_ASSERT_EVENT_ASSEMBLE(buffer, 4, event);                                                                \
    TEST_ASSERT_EQUAL(NEXTION_EVENT_DEVICE, event.type);                                                         \
    TEST_ASSERT_EQUAL(expected_device_state, event.device_state);

    TEST_CASE("Can find a message in a buffer", "[nextion][parse][msg]")
    {
        const uint8_t buffer[8] = {0x01, NEX_DVC_CMD_END_VALUE, 0x01, NEX_DVC_CMD_END_VALUE, NEX_DVC_CMD_END_VALUE, NEX_DVC_CMD_END_VALUE, 0x05, 0x05};
        ringbuffer_handle_t ring_buffer = ringbuffer_create(8);

        ringbuffer_write_bytes(ring_buffer, buffer, 8);

        int length = nextion_parse_find_message_length(ring_buffer);

        ringbuffer_free(ring_buffer);

        TEST_ASSERT_EQUAL_INT(6, length);
    }

    TEST_CASE("Can assembly NEXTION_EVENT_TOUCH", "[nextion][parse][event]")
    {
        nextion_event_t event;
        const uint8_t buffer[7] = {NEX_DVC_EVT_TOUCH_OCCURRED, 0x01, 0x02, 0x01, NEX_DVC_CMD_END_VALUE, NEX_DVC_CMD_END_VALUE, NEX_DVC_CMD_END_VALUE};

        TEST_ASSERT_EVENT_ASSEMBLE(buffer, 7, event);
        TEST_ASSERT_EQUAL(NEXTION_EVENT_TOUCH, event.type);
        TEST_ASSERT_EQUAL(1, event.touch.page_id);
        TEST_ASSERT_EQUAL(2, event.touch.component_id);
        TEST_ASSERT_EQUAL(NEXTION_TOUCH_PRESSED, event.touch.state);
    }

    TEST_CASE("Can assembly NEXTION_EVENT_TOUCH_COORD-AWAKE", "[nextion][parse][event]")
    {
        nextion_event_t event;
        const uint8_t buffer[9] = {NEX_DVC_EVT_TOUCH_COORDINATE_AWAKE, 0x00, 0x7A, 0x00, 0x1E, 0x01, NEX_DVC_CMD_END_VALUE, NEX_DVC_CMD_END_VALUE, NEX_DVC_CMD_END_VALUE};

        TEST_ASSERT_EVENT_ASSEMBLE(buffer, 9, event);
        TEST_ASSERT_EQUAL(NEXTION_EVENT_TOUCH_COORD, event.type);
        TEST_ASSERT_EQUAL(122, event.touch_coord.x);
        TEST_ASSERT_EQUAL(30, event.touch_coord.y);
        TEST_ASSERT_FALSE(event.touch_coord.exited_sleep);
        TEST_ASSERT_EQUAL(NEXTION_TOUCH_PRESSED, event.touch_coord.state);
    }

    TEST_CASE("Can assembly NEXTION_EVENT_TOUCH_COORD-ASLEEP", "[nextion][parse][event]")
    {
        nextion_event_t event;
        const uint8_t buffer[9] = {NEX_DVC_EVT_TOUCH_COORDINATE_ASLEEP, 0x00, 0x7A, 0x00, 0x1E, 0x01, NEX_DVC_CMD_END_VALUE, NEX_DVC_CMD_END_VALUE, NEX_DVC_CMD_END_VALUE};

        TEST_ASSERT_EVENT_ASSEMBLE(buffer, 9, event);
        TEST_ASSERT_EQUAL(NEXTION_EVENT_TOUCH_COORD, event.type);
        TEST_ASSERT_EQUAL(122, event.touch_coord.x);
        TEST_ASSERT_EQUAL(30, event.touch_coord.y);
        TEST_ASSERT_TRUE(event.touch_coord.exited_sleep);
        TEST_ASSERT_EQUAL(NEXTION_TOUCH_PRESSED, event.touch_coord.state);
    }

    TEST_CASE("Can assembly NEXTION_DEVICE_STARTED", "[nextion][parse][event]")
    {
        TEST_ASSERT_EVENT_ASSEMBLE_DEVICE_STATE(NEX_DVC_EVT_HARDWARE_START_RESET, NEXTION_DEVICE_STARTED);
    }

    TEST_CASE("Can assembly NEXTION_DEVICE_AUTO_SLEEP", "[nextion][parse][event]")
    {
        TEST_ASSERT_EVENT_ASSEMBLE_DEVICE_STATE(NEX_DVC_EVT_HARDWARE_AUTO_SLEEP, NEXTION_DEVICE_AUTO_SLEEP);
    }

    TEST_CASE("Can assembly NEXTION_DEVICE_AUTO_WAKE", "[nextion][parse][event]")
    {
        TEST_ASSERT_EVENT_ASSEMBLE_DEVICE_STATE(NEX_DVC_EVT_HARDWARE_AUTO_WAKE, NEXTION_DEVICE_AUTO_WAKE);
    }

    TEST_CASE("Can assembly NEXTION_DEVICE_READY", "[nextion][parse][event]")
    {
        TEST_ASSERT_EVENT_ASSEMBLE_DEVICE_STATE(NEX_DVC_EVT_HARDWARE_READY, NEXTION_DEVICE_READY);
    }

    TEST_CASE("Can assembly NEXTION_DEVICE_UPGRADING", "[nextion][parse][event]")
    {
        TEST_ASSERT_EVENT_ASSEMBLE_DEVICE_STATE(NEX_DVC_EVT_HARDWARE_UPGRADE, NEXTION_DEVICE_UPGRADING);
    }

    TEST_CASE("Can assembly NEXTINEXTION_DEVICE_TRANSP_DATA_FINISHED", "[nextion][parse][event]")
    {
        TEST_ASSERT_EVENT_ASSEMBLE_DEVICE_STATE(NEX_DVC_EVT_TRANSPARENT_DATA_FINISHED, NEXTION_DEVICE_TRANSP_DATA_FINISHED);
    }

    TEST_CASE("Can assembly NEXTION_DEVICE_TRANSP_DATA_READY", "[nextion][parse][event]")
    {
        TEST_ASSERT_EVENT_ASSEMBLE_DEVICE_STATE(NEX_DVC_EVT_TRANSPARENT_DATA_READY, NEXTION_DEVICE_TRANSP_DATA_READY);
    }

    TEST_CASE("Cannot assembly a unknown event", "[nextion][parse][event]")
    {
        ringbuffer_handle_t ring_buffer = ringbuffer_create(4);
        const uint8_t buffer[4] = {UINT8_MAX, NEX_DVC_CMD_END_VALUE, NEX_DVC_CMD_END_VALUE, NEX_DVC_CMD_END_VALUE};
        nextion_event_t event;

        ringbuffer_write_bytes(ring_buffer, buffer, 4);

        TEST_ASSERT_FALSE(nextion_parse_assemble_event(ring_buffer, 4, &event));

        ringbuffer_free(ring_buffer);
    }

#ifdef __cplusplus
}
#endif
