#include "unity.h"
#include "ringbuffer/ringbuffer.h"

#ifdef __cplusplus
extern "C"
{
#endif

    TEST_CASE("Create fails when size < 1", "[ringbuffer][argument]")
    {
        TEST_ASSERT_NULL(ringbuffer_create(0));
    }

    TEST_CASE("Reset fails when handle is null", "[ringbuffer][argument]")
    {
        TEST_ASSERT_FALSE(ringbuffer_reset(NULL));
    }

    TEST_CASE("Capacity fails when handle is null", "[ringbuffer][argument]")
    {
        TEST_ASSERT_EQUAL(-1, ringbuffer_capacity(NULL));
    }

    TEST_CASE("Full fails when handle is null", "[ringbuffer][argument]")
    {
        TEST_ASSERT_TRUE(ringbuffer_full(NULL));
    }

    TEST_CASE("Empty fails when handle is null", "[ringbuffer][argument]")
    {
        TEST_ASSERT_TRUE(ringbuffer_empty(NULL));
    }

    TEST_CASE("Remaining fails when handle is null", "[ringbuffer][argument]")
    {
        TEST_ASSERT_EQUAL(-1, ringbuffer_remaining(NULL));
    }

    TEST_CASE("Free fails when handle is null", "[ringbuffer][argument]")
    {
        TEST_ASSERT_FALSE(ringbuffer_free(NULL));
    }

    TEST_CASE("Write fails when handle is null", "[ringbuffer][argument]")
    {
        TEST_ASSERT_FALSE(ringbuffer_write(NULL, 1));
    }

    TEST_CASE("Write bytes fails when handle is null", "[ringbuffer][argument]")
    {
        const uint8_t buffer[1];

        TEST_ASSERT_FALSE(ringbuffer_write_bytes(NULL, buffer, 1));
    }

    TEST_CASE("Write bytes fails when buffer is null", "[ringbuffer][argument]")
    {
        ringbuffer_handle_t ring = ringbuffer_create(1);

        TEST_ASSERT_FALSE(ringbuffer_write_bytes(ring, NULL, 1));

        ringbuffer_free(ring);
    }

    TEST_CASE("Write bytes fails when length < 1", "[ringbuffer][argument]")
    {
        ringbuffer_handle_t ring = ringbuffer_create(1);
        const uint8_t buffer[1];

        TEST_ASSERT_FALSE(ringbuffer_write_bytes(ring, buffer, 0));

        ringbuffer_free(ring);
    }

    TEST_CASE("Read fails when handle is null", "[ringbuffer][argument]")
    {
        uint8_t value;

        TEST_ASSERT_FALSE(ringbuffer_read(NULL, &value));
    }

    TEST_CASE("Read fails when value is null", "[ringbuffer][argument]")
    {
        ringbuffer_handle_t ring = ringbuffer_create(1);
        uint8_t value;

        TEST_ASSERT_FALSE(ringbuffer_read(NULL, &value));

        ringbuffer_free(ring);
    }

    TEST_CASE("Read bytes fails when handle is null", "[ringbuffer][argument]")
    {
        uint8_t buffer[1];

        TEST_ASSERT_FALSE(ringbuffer_read_bytes(NULL, buffer, 1));
    }

    TEST_CASE("Read bytes fails when buffer is null", "[ringbuffer][argument]")
    {
        ringbuffer_handle_t ring = ringbuffer_create(1);

        TEST_ASSERT_FALSE(ringbuffer_read_bytes(ring, NULL, 1));

        ringbuffer_free(ring);
    }

    TEST_CASE("Read bytes fails when length < 1", "[ringbuffer][argument]")
    {
        ringbuffer_handle_t ring = ringbuffer_create(1);
        uint8_t buffer[1];

        TEST_ASSERT_FALSE(ringbuffer_read_bytes(ring, buffer, 0));

        ringbuffer_free(ring);
    }

    TEST_CASE("Peek fails when handle is null", "[ringbuffer][argument]")
    {
        int holder;
        uint8_t value;

        TEST_ASSERT_FALSE(ringbuffer_peek(NULL, &holder, &value));
    }

    TEST_CASE("Peek fails when peek_inc_holder is null", "[ringbuffer][argument]")
    {
        ringbuffer_handle_t ring = ringbuffer_create(1);
        uint8_t value;

        TEST_ASSERT_FALSE(ringbuffer_peek(NULL, NULL, &value));

        ringbuffer_free(ring);
    }

    TEST_CASE("Peek bytes fails when value is null", "[ringbuffer][argument]")
    {
        ringbuffer_handle_t ring = ringbuffer_create(1);
        int holder;

        TEST_ASSERT_FALSE(ringbuffer_peek(NULL, &holder, NULL));

        ringbuffer_free(ring);
    }

#ifdef __cplusplus
}
#endif
