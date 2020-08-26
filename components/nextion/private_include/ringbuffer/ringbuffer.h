#ifndef __RINGBUFFER_H__
#define __RINGBUFFER_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * Handle for a single-reader, single-writer ring buffer.
     */
    typedef void *ringbuffer_handle_t;

    /**
     * @brief Allocates a new Single-Writer-Single-Reader (SWSR) ring buffer.
     * @param size Size, in bytes, of the buffer to be allocated.
     * @return Pointer to a ring buffer instance or NULL if it failed.
     */
    ringbuffer_handle_t ringbuffer_create(int size);

    /**
     * @brief Resets a ring buffer positions and zeroes the buffer.
     * @param handle Ring buffer handle.
     * @return True if success, otherwise false.
     */
    bool ringbuffer_reset(ringbuffer_handle_t handle);

    /**
     * @brief Return how many items can be stored in a ring buffer.
     * @param handle Ring buffer handle.
     * @return How many items can be stored or -1 if it failed.
     */
    int ringbuffer_capacity(ringbuffer_handle_t handle);

    /**
     * @brief If a ring buffer is full.
     * @note If the pointer is null it will return true.
     * @param handle Ring buffer handle.
     * @return True if full, otherwise false.
     */
    bool ringbuffer_full(ringbuffer_handle_t handle);

    /**
     * @brief If a ring buffer is empty.
     * @note If the pointer is null it will return true.
     * @param handle Ring buffer handle.
     * @return True if empty, otherwise false.
     */
    bool ringbuffer_empty(ringbuffer_handle_t handle);

    /**
     * @brief How many slots are available tp be written.
     * @param handle Ring buffer handle.
     * @return Number of slots available or -1 if it failed.
     */
    int ringbuffer_remaining(ringbuffer_handle_t handle);

    /**
     * @brief Deallocates a ring buffer.
     * @param handle Ring buffer handle.
     * @return True if success, otherwise false.
     */
    bool ringbuffer_free(ringbuffer_handle_t handle);

    /**
     * @brief Writes a byte to a ring buffer and advances the write position.
     *
     * @param handle Ring buffer handle.
     * @param value Value to be written.
     *
     * @return True if success, otherwise false.
     */
    bool ringbuffer_write(ringbuffer_handle_t handle, uint8_t value);

    /**
     * @brief Writes a stream to a ring buffer and advances the write position.
     *
     * @note It either writes everything or nothing.
     *
     * @param handle Ring buffer handle.
     * @param buffer Pointer to a buffer containing the values to be written.
     * @param length How much to write.
     *
     * @return True if success, otherwise false.
     */
    bool ringbuffer_write_bytes(ringbuffer_handle_t handle, const uint8_t *buffer, int length);

    /**
     * @brief Reads a byte from a ring buffer and advances the read position.
     *
     * @param handle Ring buffer handle.
     * @param value Pointer to where a value will be stored.
     *
     * @return True if success, otherwise false.
     */
    bool ringbuffer_read(ringbuffer_handle_t handle, uint8_t *value);

    /**
     * @brief Read a stream from a ring buffer and advances the read position.
     *
     * @note It either reads everything or nothing.
     *
     * @param handle Ring buffer handle.
     * @param buffer Storage where the values will be written.
     * @param length How much to read.
     *
     * @return True if success, otherwise false.
     */
    bool ringbuffer_read_bytes(ringbuffer_handle_t handle, uint8_t *buffer, int length);

    /**
     * @brief Peek a value from a ring buffer, without modifying the read position.
     *
     * @details Each peek will increment the "peek_inc_holder".
     *
     * @param handle Ring buffer handle.
     * @param peek_inc_holder Storage for the "read" count.
     * @param value Pointer to where the value will be stored.
     *
     * @return True if success, otherwise false.
     */
    bool ringbuffer_peek(ringbuffer_handle_t handle, int *peek_inc_holder, uint8_t *value);

#ifdef __cplusplus
}
#endif

#endif //__RINGBUFFER_H__