#ifndef __ESP32_DRIVER_NEXTION_PROTO_PRSR_RSP_NUM_H__
#define __ESP32_DRIVER_NEXTION_PROTO_PRSR_RSP_NUM_H__

#include "protocol/parsers/parser.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Verify if the parser can parse the data.
     * @note Used to parse number responses.
     * @param[in] parser Parser context pointer.
     * @param[in] data_id Data identifier.
     * @return If can parse.
     */
    bool parser_rsp_number_can_parse(const parser_t *parser, const uint8_t data_id);

    /**
     * @brief Verify if the parser needs more data.
     * @note Used to parse number responses.
     * @param[in] parser Parser context pointer.
     * @param[in] data Data pointer to be parsed.
     * @param[in] length Data length.
     * @return Number of bytes needed.
     */
    int parser_rsp_number_need_more_bytes(const parser_t *parser, const uint8_t *data, size_t length);

    /**
     * @brief Parse the data.
     * @note Used to parse number responses.
     * @param[in] parser Parser context pointer.
     * @param[in] data Data pointer to be parsed.
     * @param[in] length Data length.
     * @return Success or failure.
     */
    bool parser_rsp_number_parse(const parser_t *parser, const uint8_t *data, size_t length);

    /**
     * @brief Parse data into a number.
     * @param[in] data Data pointer to be parsed, without the data id.
     * @return Number parsed.
     */
    int32_t parser_rsp_number_convert(const uint8_t *data);

    /**
     * @brief Create a NUMBER parser.
     * @param result_buffer_value Buffer to write the parsed number into.
     * @param result_buffer_length_value Buffer length.
     */
#define PARSER_NUMBER(result_buffer_value, result_buffer_length_value) \
    {                                                                  \
        .can_parse = parser_rsp_number_can_parse,                      \
        .need_more_bytes = parser_rsp_number_need_more_bytes,          \
        .parse = parser_rsp_number_parse,                              \
        .result_buffer = result_buffer_value,                          \
        .result_buffer_length = result_buffer_length_value}

#ifdef __cplusplus
}
#endif
#endif