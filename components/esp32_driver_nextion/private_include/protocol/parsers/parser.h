#ifndef __ESP32_DRIVER_NEXTION_PROTO_PRSR_H__
#define __ESP32_DRIVER_NEXTION_PROTO_PRSR_H__

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "esp32_driver_nextion/base/types.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @typedef parser_t
     * @brief Response parser.
     */
    typedef struct parser_t parser_t;

    /**
     * @typedef parser_can_parse
     * @brief Function contract to verify if the parser can parse the data.
     * @param[in] parser Parser context pointer.
     * @param[in] data_id Data identifier.
     * @return If can parse.
     */
    typedef bool (*parser_can_parse)(const parser_t *parser, const uint8_t data_id);

    /**
     * @typedef parser_can_parse
     * @brief Function contract to verify if the parser needs more data.
     * @param[in] parser Parser context pointer.
     * @param[in] data Data pointer to be parsed.
     * @param[in] length Data length.
     */
    typedef int (*parser_need_more_bytes)(const parser_t *parser, const uint8_t *data, size_t length);

    /**
     * @typedef parser_can_parse
     * @brief Function contract to parse the data.
     * @param[in] parser Parser context pointer.
     * @param[in] data Data pointer to be parsed.
     * @param[in] length Data length.
     */
    typedef bool (*parser_parse)(const parser_t *parser, const uint8_t *data, size_t length);

    /**
     * @struct parser_t
     * @brief Response parser.
     */
    struct parser_t
    {
        parser_can_parse can_parse;             /** @brief Function to verify if the parser can parse the data. */
        parser_need_more_bytes need_more_bytes; /** @brief Function to verify if the parser needs more data. */
        parser_parse parse;                     /** @brief Function to parse the data. */
        void *result_buffer;                    /** @brief Buffer to write the parsed data into. */
        size_t result_buffer_length;            /** @brief Buffer length. */
    };

#ifdef __cplusplus
}
#endif
#endif