#ifndef __ESP32_DRIVER_NEXTION_PROTO_PRSR_RSP_ACK_H__
#define __ESP32_DRIVER_NEXTION_PROTO_PRSR_RSP_ACK_H__

#include "protocol/parsers/parser.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Verify if the parser can parse the data.
     * @note Used to parse common responses.
     * @param[in] parser Parser context pointer.
     * @param[in] data_id Data identifier.
     * @return If can parse.
     */
    bool parser_rsp_ack_can_parse(const parser_t *parser, const uint8_t data_id);

    /**
     * @brief Verify if the parser needs more data.
     * @note Used to parse common responses.
     * @param[in] parser Parser context pointer.
     * @param[in] data Data pointer to be parsed.
     * @param[in] length Data length.
     * @return Number of bytes needed.
     */
    int parser_rsp_ack_need_more_bytes(const parser_t *parser, const uint8_t *data, size_t length);

    /**
     * @brief Parse the data.
     * @note Used to parse common responses.
     * @param[in] parser Parser context pointer.
     * @param[in] data Data pointer to be parsed.
     * @param[in] length Data length.
     * @return Success or failure.
     */
    bool parser_rsp_ack_parse(const parser_t *parser, const uint8_t *data, size_t length);

    /**
     * @brief Create an ACK parser.
     */
#define PARSER_ACK()                                       \
    {                                                      \
        .can_parse = parser_rsp_ack_can_parse,             \
        .need_more_bytes = parser_rsp_ack_need_more_bytes, \
        .parse = parser_rsp_ack_parse}

#ifdef __cplusplus
}
#endif
#endif