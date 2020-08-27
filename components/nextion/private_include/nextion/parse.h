#ifndef __NEXTION_PARSE_H__
#define __NEXTION_PARSE_H__

#include "ringbuffer/ringbuffer.h"
#include "nextion/types.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Verifies if a buffer contains a complete message.
     *
     * @details Search from index 0 to length-1 and does not modifies the
     * ring buffer read position.
     *
     * @param ring_buffer Ring buffer to search on.
     * @return -1 if not found, otherwise the message length.
     */
    int nextion_parse_find_message_length(const ringbuffer_handle_t ring_buffer);

    /**
     * @brief Assembles a event from a buffer.
     *
     * @details It will advance the ring buffer read position.
     *
     * @param ring_buffer Ring buffer containing a message.
     * @param message_length Length of the message containing the event.
     * @param[out] event Event assembled from a message.
     * @return True when success or false.
     */
    bool nextion_parse_assemble_event(const ringbuffer_handle_t ring_buffer, const int message_length, nextion_event_t *event);

#ifdef __cplusplus
}
#endif

#endif //__NEXTION_PARSE_H__