#ifndef __RINGBUFFER_C__
#define __RINGBUFFER_C__

#include <string.h>
#include "malloc.h"
#include "common.h"
#include "ringbuffer/ringbuffer.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define RINGBUFFER_CHECK(condition, message, return_value) NEX_CHECK(condition, message, return_value)
#define RINGBUFFER_CHECK_HANDLE(handle, return_value) NEX_CHECK((handle != NULL), "handle error(NULL)", return_value);

    typedef struct
    {
        int length;
        int read_pos;
        int write_pos;
        int written;
        uint8_t *buffer;
        uint8_t buffer_area;
    } ringbuffer_t;

    ringbuffer_handle_t ringbuffer_create(int size)
    {
        RINGBUFFER_CHECK((size > 0), "size error(<1)", NULL);

        ringbuffer_t *ring = (ringbuffer_t *)malloc(sizeof(ringbuffer_t) + size);
        ring->length = size;

        // For locality, I'd rather have everything together and
        // "waste" a byte (or 24+ bits in case of padding) than
        // allocating memory far from my struct.

        ring->buffer = &ring->buffer_area;

        ringbuffer_reset(ring);

        return ring;
    }

    bool ringbuffer_reset(ringbuffer_handle_t handle)
    {
        RINGBUFFER_CHECK_HANDLE(handle, false);

        ringbuffer_t *ring = (ringbuffer_t *)handle;

        bzero(ring->buffer, ring->length);

        ring->read_pos = 0;
        ring->write_pos = 0;
        ring->written = 0;

        return true;
    }

    int ringbuffer_capacity(ringbuffer_handle_t handle)
    {
        RINGBUFFER_CHECK_HANDLE(handle, -1);

        return ((ringbuffer_t *)handle)->length;
    }

    bool ringbuffer_full(ringbuffer_handle_t handle)
    {
        // I do not have much options here. Return true
        // to "prevent" the user from writing on the buffer.
        RINGBUFFER_CHECK_HANDLE(handle, true);

        const ringbuffer_t *ring = (ringbuffer_t *)handle;

        return ring->written == ring->length;
    }

    bool ringbuffer_empty(ringbuffer_handle_t handle)
    {
        // I do not have much options here. Return true
        // to "prevent" the user from reading from the buffer.
        RINGBUFFER_CHECK_HANDLE(handle, true);

        return ((ringbuffer_t *)handle)->written == 0;
    }

    int ringbuffer_remaining(ringbuffer_handle_t handle)
    {
        RINGBUFFER_CHECK_HANDLE(handle, -1);

        const ringbuffer_t *ring = (ringbuffer_t *)handle;

        return ring->length - ring->written;
    }

    bool ringbuffer_free(ringbuffer_handle_t handle)
    {
        RINGBUFFER_CHECK_HANDLE(handle, false);

        free(handle);

        return true;
    }

    bool ringbuffer_write(ringbuffer_handle_t handle, uint8_t value)
    {
        RINGBUFFER_CHECK_HANDLE(handle, false);

        if (ringbuffer_full(handle))
            return false;

        ringbuffer_t *ring = (ringbuffer_t *)handle;
        const int write_pos = ring->write_pos;

        ring->buffer[write_pos] = value;
        ring->write_pos = (write_pos + 1) % ring->length;
        ring->written += 1;

        return true;
    }

    bool ringbuffer_write_bytes(ringbuffer_handle_t handle, const uint8_t *buffer, int length)
    {
        RINGBUFFER_CHECK_HANDLE(handle, false);
        RINGBUFFER_CHECK((buffer != NULL), "buffer error(NULL)", false);
        RINGBUFFER_CHECK((length > 0), "length error(<1)", false);

        if (ringbuffer_full(handle) || ringbuffer_remaining(handle) < length)
            return false;

        ringbuffer_t *ring = (ringbuffer_t *)handle;
        const int ring_length = ring->length;
        int write_pos = ring->write_pos;

        for (int i = 0; i < length; i++)
        {
            ring->buffer[write_pos] = buffer[i];
            write_pos = (write_pos + 1) % ring_length;
        }

        ring->write_pos = write_pos;
        ring->written += length;

        return true;
    }

    bool ringbuffer_read(ringbuffer_handle_t handle, uint8_t *value)
    {
        RINGBUFFER_CHECK_HANDLE(handle, false);

        if (ringbuffer_empty(handle))
            return false;

        ringbuffer_t *ring = (ringbuffer_t *)handle;
        const int read_pos = ring->read_pos;

        *value = ring->buffer[read_pos];

        ring->read_pos = (read_pos + 1) % ring->length;
        ring->written -= 1;

        return value;
    }

    bool ringbuffer_read_bytes(ringbuffer_handle_t handle, uint8_t *buffer, int length)
    {
        RINGBUFFER_CHECK_HANDLE(handle, false);
        RINGBUFFER_CHECK((buffer != NULL), "buffer error(NULL)", false);
        RINGBUFFER_CHECK((length > 0), "length error(<1)", false);

        ringbuffer_t *ring = (ringbuffer_t *)handle;

        if (ringbuffer_empty(handle) || ring->written < length)
            return false;

        const int ring_length = ring->length;
        int read_pos = ring->read_pos;

        for (int i = 0; i < length; i++)
        {
            buffer[i] = ring->buffer[read_pos];
            read_pos = (read_pos + 1) % ring_length;
        }

        ring->read_pos = read_pos;
        ring->written -= length;

        return true;
    }

    bool ringbuffer_peek(ringbuffer_handle_t handle, int *peek_inc_holder, uint8_t *value)
    {
        RINGBUFFER_CHECK_HANDLE(handle, false);
        RINGBUFFER_CHECK((peek_inc_holder != NULL), "peek_inc_holder error(NULL)", false);
        RINGBUFFER_CHECK((value != NULL), "value error(NULL)", false);

        ringbuffer_t *ring = (ringbuffer_t *)handle;

        if (ringbuffer_empty(handle) || ring->written - *peek_inc_holder == 0)
            return false;

        *value = ring->buffer[(ring->read_pos + *peek_inc_holder) % ring->length];
        *peek_inc_holder = *peek_inc_holder + 1;

        return true;
    }

#ifdef __cplusplus
}
#endif

#endif //__RINGBUFFER_C__