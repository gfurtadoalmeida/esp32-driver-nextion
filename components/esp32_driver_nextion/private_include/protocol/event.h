#ifndef __ESP32_DRIVER_NEXTION_PROTO_EVT_H__
#define __ESP32_DRIVER_NEXTION_PROTO_EVT_H__

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "esp32_driver_nextion/base/types.h"
#include "protocol/parsers/parser.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @typedef event_parser_t
     * @brief Event parser.
     */
    typedef struct
    {
        parser_t base;               /** @brief Base parser. */
        size_t required_buffer_size; /** @brief Required buffer size to parse the event. */
        uint8_t event_id;            /** @brief Event id. */
    } event_parser_t;

    /**
     * @brief Try to get an event parser by event id.
     * @param[in] handle Event id to find a parser to.
     * @param[in] result_buffer Result buffer where the parsed event will be stored.
     * @param[in] result_buffer_length Result buffer length.
     * @param[out] parser Location where the retrieved parser will be stored.
     * @return Sucess or failure.
     */
    bool try_get_event_parser(uint8_t event_id,
                              void *result_buffer,
                              size_t result_buffer_length,
                              event_parser_t *parser);

#ifdef __cplusplus
}
#endif
#endif