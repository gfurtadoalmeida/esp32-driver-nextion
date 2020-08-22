#ifndef __RINGBUFFER_C__
#define __RINGBUFFER_C__

#include <string.h>
#include "esp_heap_caps.h"
#include "ringbuffer.h"

#ifdef __cplusplus
extern "C"
{
#endif

    // This implementation was based on https://embeddedartistry.com/blog/2017/05/17/creating-a-circular-buffer-in-c-and-c/.

    typedef struct
    {
        int length;
        int read_pos;
        int write_pos;
        int written;
        uint8_t *buffer;     // <-- It was going to do padding anyway, so at least make the pad
        uint8_t buffer_area; // <-- space useful and keep everything on the same memory range (for locality).
    } ringbuffer_t;

    ringbuffer_handle_t ringbuffer_create(int size)
    {
        ringbuffer_t *ring = (ringbuffer_t *)malloc(sizeof(ringbuffer_t) + size);
        ring->length = size;
        ring->buffer = &ring->buffer_area;

        ringbuffer_reset(ring);

        return ring;
    }

    void ringbuffer_reset(ringbuffer_handle_t handle)
    {
        ringbuffer_t *ring = (ringbuffer_t *)handle;

        bzero(ring->buffer, ring->length);

        ring->read_pos = 0;
        ring->write_pos = 0;
        ring->written = 0;
    }

    int ringbuffer_capacity(ringbuffer_handle_t handle)
    {
        return ((ringbuffer_t *)handle)->length;
    }

    bool ringbuffer_full(ringbuffer_handle_t handle)
    {
        ringbuffer_t *ring = (ringbuffer_t *)handle;

        return ring->written == ring->length;
    }

    bool ringbuffer_empty(ringbuffer_handle_t handle)
    {
        return ((ringbuffer_t *)handle)->written == 0;
    }

    int ringbuffer_remaining(ringbuffer_handle_t handle)
    {
        ringbuffer_t *ring = (ringbuffer_t *)handle;

        return ring->length - ring->written;
    }

    void ringbuffer_free(ringbuffer_handle_t handle)
    {
        free(handle);
    }

    bool ringbuffer_write(ringbuffer_handle_t handle, uint8_t value)
    {
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