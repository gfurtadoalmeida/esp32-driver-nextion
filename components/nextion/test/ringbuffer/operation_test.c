#include "unity.h"
#include "unity_test_runner.h"
#include "ringbuffer/ringbuffer.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define FILL(ring, length)           \
    for (int i = 0; i < length; i++) \
    {                                \
        ringbuffer_write(ring, i);   \
    }

    TEST_CASE("Creation is correct", "[ringbuffer][operation]")
    {
        ringbuffer_handle_t ring = ringbuffer_create(5);

        TEST_ASSERT_TRUE(ringbuffer_empty(ring));
        TEST_ASSERT_FALSE(ringbuffer_full(ring));
        TEST_ASSERT_EQUAL(5, ringbuffer_capacity(ring));
        TEST_ASSERT_EQUAL(5, ringbuffer_remaining(ring));

        ringbuffer_free(ring);
    }

    TEST_CASE("Reset is correct", "[ringbuffer][operation]")
    {
        ringbuffer_handle_t ring = ringbuffer_create(5);

        FILL(ring, 5);

        ringbuffer_reset(ring);

        TEST_ASSERT_TRUE(ringbuffer_empty(ring));
        TEST_ASSERT_FALSE(ringbuffer_full(ring));
        TEST_ASSERT_EQUAL(5, ringbuffer_capacity(ring));
        TEST_ASSERT_EQUAL(5, ringbuffer_remaining(ring));

        ringbuffer_free(ring);
    }

    TEST_CASE("Full is correct", "[ringbuffer][operation]")
    {
        ringbuffer_handle_t ring = ringbuffer_create(5);

        FILL(ring, 5);

        TEST_ASSERT_TRUE(ringbuffer_full(ring));
        TEST_ASSERT_FALSE(ringbuffer_empty(ring));
        TEST_ASSERT_EQUAL(0, ringbuffer_remaining(ring));

        ringbuffer_free(ring);
    }

    TEST_CASE("Remaining reduces when writing", "[ringbuffer][operation]")
    {
        ringbuffer_handle_t ring = ringbuffer_create(5);

        FILL(ring, 4);

        TEST_ASSERT_EQUAL(1, ringbuffer_remaining(ring));

        ringbuffer_free(ring);
    }

    TEST_CASE("Remaining increases when reading", "[ringbuffer][operation]")
    {
        ringbuffer_handle_t ring = ringbuffer_create(5);
        uint8_t buffer[3];

        FILL(ring, 5);

        ringbuffer_read_bytes(ring, buffer, 3);

        TEST_ASSERT_EQUAL(3, ringbuffer_remaining(ring));

        ringbuffer_free(ring);
    }

    TEST_CASE("Can write when not full", "[ringbuffer][operation]")
    {
        ringbuffer_handle_t ring = ringbuffer_create(5);

        TEST_ASSERT_TRUE(ringbuffer_write(ring, 128));

        ringbuffer_free(ring);
    }

    TEST_CASE("Cannot write when full", "[ringbuffer][operation]")
    {
        ringbuffer_handle_t ring = ringbuffer_create(5);

        FILL(ring, 5);

        TEST_ASSERT_FALSE(ringbuffer_write(ring, 128));

        ringbuffer_free(ring);
    }

    TEST_CASE("Can write bytes when not full", "[ringbuffer][operation]")
    {
        ringbuffer_handle_t ring = ringbuffer_create(5);
        uint8_t buffer[2] = {1, 2};

        TEST_ASSERT_TRUE(ringbuffer_write_bytes(ring, buffer, 2));

        ringbuffer_free(ring);
    }

    TEST_CASE("Cannot write bytes when full", "[ringbuffer][operation]")
    {
        ringbuffer_handle_t ring = ringbuffer_create(5);
        uint8_t buffer[2] = {1, 2};

        FILL(ring, 5);

        TEST_ASSERT_FALSE(ringbuffer_write_bytes(ring, buffer, 2));

        ringbuffer_free(ring);
    }

    TEST_CASE("Cannot write bytes when no space for all bytes", "[ringbuffer][operation]")
    {
        ringbuffer_handle_t ring = ringbuffer_create(5);
        uint8_t buffer[2] = {1, 2};

        FILL(ring, 4);

        TEST_ASSERT_FALSE(ringbuffer_write_bytes(ring, buffer, 2));

        ringbuffer_free(ring);
    }

    TEST_CASE("Can read when not emtpy", "[ringbuffer][operation]")
    {
        ringbuffer_handle_t ring = ringbuffer_create(5);

        FILL(ring, 5);

        uint8_t value;

        TEST_ASSERT_TRUE(ringbuffer_read(ring, &value));

        ringbuffer_free(ring);
    }

    TEST_CASE("Cannot read when emtpy", "[ringbuffer][operation]")
    {
        ringbuffer_handle_t ring = ringbuffer_create(5);
        uint8_t value;

        TEST_ASSERT_FALSE(ringbuffer_read(ring, &value));

        ringbuffer_free(ring);
    }

    TEST_CASE("Can read bytes when not emtpy", "[ringbuffer][operation]")
    {
        ringbuffer_handle_t ring = ringbuffer_create(5);
        uint8_t buffer[2];

        FILL(ring, 5);

        TEST_ASSERT_TRUE(ringbuffer_read_bytes(ring, buffer, 2));

        ringbuffer_free(ring);
    }

    TEST_CASE("Cannot read bytes when emtpy", "[ringbuffer][operation]")
    {
        ringbuffer_handle_t ring = ringbuffer_create(5);
        uint8_t buffer[2];

        TEST_ASSERT_FALSE(ringbuffer_read_bytes(ring, buffer, 2));

        ringbuffer_free(ring);
    }

    TEST_CASE("Cannot read bytes when not enough bytes written", "[ringbuffer][operation]")
    {
        ringbuffer_handle_t ring = ringbuffer_create(5);
        uint8_t buffer[2];

        FILL(ring, 1);

        TEST_ASSERT_FALSE(ringbuffer_read_bytes(ring, buffer, 2));

        ringbuffer_free(ring);
    }

    TEST_CASE("What was written is correctly read", "[ringbuffer][operation]")
    {
        ringbuffer_handle_t ring = ringbuffer_create(5);
        uint8_t buffer[5];

        FILL(ring, 5);

        ringbuffer_read_bytes(ring, buffer, 5);

        for (int i = 0; i < 5; i++)
        {
            TEST_ASSERT_EQUAL(i, buffer[i]);
        }

        FILL(ring, 5); // This will wrap up.

        ringbuffer_read_bytes(ring, buffer, 5);

        for (int i = 0; i < 5; i++)
        {
            TEST_ASSERT_EQUAL(i, buffer[i]);
        }

        ringbuffer_free(ring);
    }

#ifdef __cplusplus
}
#endif
