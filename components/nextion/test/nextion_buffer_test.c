#include "unity.h"
#include "nextion_constants.h"
#include "nextion_buffer.h"

#ifdef __cplusplus
extern "C"
{
#endif

    TEST_CASE("Can find a message in a buffer", "[nexbuff-find-msg]")
    {
        const uint8_t buffer[8] = {0x01, NEX_DVC_CMD_END_VALUE, 0x01, NEX_DVC_CMD_END_VALUE, NEX_DVC_CMD_END_VALUE, NEX_DVC_CMD_END_VALUE, 0x05, 0x05};

        int length = nextion_buffer_find_message_length(buffer, 8);

        TEST_ASSERT_EQUAL_INT(6, length);
    }

#ifdef __cplusplus
}
#endif
