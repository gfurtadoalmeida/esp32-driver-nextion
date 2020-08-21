#ifndef __NEXTION_PARSE_H__
#define __NEXTION_PARSE_H__

#include <stdint.h>
#include "nextion_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Verifies if a buffer contains a complete message.
     *
     * @details Search from index 0 to length-1.
     *
     * @param buffer Buffer to search on.
     * @param length Buffer length.
     * @return -1 if not found, otherwise the message length.
     */
    int nextion_parse_find_message_length(const uint8_t *buffer, int length);

    /**
     * @brief Assembles a event from a buffer.
     *
     * @details The buffer must contain a complete message.
     *
     * @param code Message code; the first byte of a message.
     * @param buffer Buffer containing a message.
     * @param length Buffer length.
     * @param[out] event Event assembled from a message.
     * @return True when success or false.
     */
    bool nextion_parse_assemble_event(uint8_t code, const uint8_t *buffer, int length, nextion_event_t *event);

#ifdef __cplusplus
}
#endif

#endif //__NEXTION_PARSE_H__