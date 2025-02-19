#ifndef __ESP32_DRIVER_NEXTION_EEPROM_H__
#define __ESP32_DRIVER_NEXTION_EEPROM_H__

#include <stdint.h>
#include <stddef.h>
#include "base/codes.h"
#include "base/types.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Write a text on the device EEPROM.
     * @param[in] handle Nextion context pointer.
     * @param[in] address Starting address to write the text. Range: 0-NEX_DVC_EEPROM_MAX_ADDRESS
     * @param[in] text Text to be written.
     * @param[in] text_length Text length.
     * @return NEX_OK if success, otherwise NEX_FAIL.
     */
    nex_err_t nextion_eeprom_write_text(nextion_t *handle,
                                        uint16_t address,
                                        const char *text,
                                        size_t text_length);

    /**
     * @brief Write a number on the device EEPROM.
     * @param[in] handle Nextion context pointer.
     * @param[in] address Starting address to write the text. Range: 0-NEX_DVC_EEPROM_MAX_ADDRESS
     * @param[in] value Number to be written.
     * @return NEX_OK if success, otherwise NEX_FAIL.
     */
    nex_err_t nextion_eeprom_write_number(nextion_t *handle,
                                          uint16_t address,
                                          int32_t value);

    /**
     * @brief Read a text from the device EEPROM.
     * @note It's the caller responsibility to allocate a buffer big enough to
     * hold the text returned.
     * @param[in] handle Nextion context pointer.
     * @param[in] address Starting address to read from. Range: 0-NEX_DVC_EEPROM_MAX_ADDRESS
     * @param[out] text Buffer with enough capacity for the entire text + null terminator.
     * @param[in] text_length Text length.
     * @return NEX_OK if success, otherwise NEX_FAIL.
     */
    nex_err_t nextion_eeprom_read_text(nextion_t *handle,
                                       uint16_t address,
                                       char *text,
                                       size_t text_length);

    /**
     * @brief Read a number from the device EEPROM.
     * @param[in] handle Nextion context pointer.
     * @param[in] address Starting address to read from. Range: 0-NEX_DVC_EEPROM_MAX_ADDRESS
     * @param[out] value Location where the value will be stored.
     * @return NEX_OK if success, otherwise NEX_FAIL.
     */
    nex_err_t nextion_eeprom_read_number(nextion_t *handle,
                                         uint16_t address,
                                         int32_t *value);

    /**
     * @brief Read raw bytes from the device EEPROM.
     * @param[in] handle Nextion context pointer.
     * @param[in] address Starting address to read from. Range: 0-NEX_DVC_EEPROM_MAX_ADDRESS
     * @param[out] buffer Buffer with enough capacity for the bytes retrieved.
     * @param[in] buffer_length Buffer length.
     * @return NEX_OK if success, otherwise NEX_FAIL.
     */
    nex_err_t nextion_eeprom_read_bytes(nextion_t *handle,
                                        uint16_t address,
                                        uint8_t *buffer,
                                        size_t buffer_length);

    /**
     * @brief Begin the EEPROM data streaming.
     * @note When in this mode, the device will "hang" until all
     * data is sent; no event or other commands will be processed.
     * @param[in] handle Nextion context pointer.
     * @param[in] address Starting address to write the bytes. Range: 0-NEX_DVC_EEPROM_MAX_ADDRESS
     * @param[in] value_count How many values will be written. "value_count < (NEX_DVC_TRANSPARENT_DATA_MAX_DATA_SIZE - 20)"
     * @return NEX_OK if success, otherwise NEX_FAIL.
     */
    nex_err_t nextion_eeprom_stream_begin(nextion_t *handle, uint16_t address, size_t value_count);

    /**
     * @brief Write a value onto the EEPROM stream.
     * @param[in] handle Nextion context pointer.
     * @param[in] value Value to be written.
     * @return NEX_OK if success, otherwise NEX_FAIL.
     */
    nex_err_t nextion_eeprom_stream_write(const nextion_t *handle, uint8_t value);

#ifdef __cplusplus
}
#endif
#endif