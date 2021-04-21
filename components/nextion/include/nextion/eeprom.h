#ifndef __NEXTION_EEPROM_H__
#define __NEXTION_EEPROM_H__

#include <stdint.h>
#include "base/codes.h"
#include "base/types.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Writes a text on the device EEPROM.
     *
     * @param handle Nextion context pointer.
     * @param address Starting address to write the text. Range: 0-NEX_DVC_EEPROM_MAX_ADDRESS
     * @param text Text to be written.
     * @param text_length Text length.
     *
     * @return NEX_OK if success, otherwise NEX_FAIL.
     */
    nex_err_t nextion_eeprom_write_text(nextion_handle_t handle,
                                        uint16_t address,
                                        const char *text,
                                        size_t text_length);

    /**
     * @brief Writes a number on the device EEPROM.
     *
     * @param handle Nextion context pointer.
     * @param address Starting address to write the text. Range: 0-NEX_DVC_EEPROM_MAX_ADDRESS
     * @param value Number to be written.
     *
     * @return NEX_OK if success, otherwise NEX_FAIL.
     */
    nex_err_t nextion_eeprom_write_number(nextion_handle_t handle,
                                          uint16_t address,
                                          int32_t value);

    /**
     * @brief Reads a text from the device EEPROM.
     *
     * @details It's the caller responsibility to allocate a buffer big enough to
     * hold the text returned.
     *
     * @param handle Nextion context pointer.
     * @param address Starting address to read from. Range: 0-NEX_DVC_EEPROM_MAX_ADDRESS
     * @param text Buffer with enough capacity for the entire text + null terminator.
     * @param text_length Text length.
     *
     * @return NEX_OK if success, otherwise NEX_FAIL.
     */
    nex_err_t nextion_eeprom_read_text(nextion_handle_t handle,
                                       uint16_t address,
                                       char *text,
                                       size_t text_length);

    /**
     * @brief Reads a number from the device EEPROM.
     *
     * @param handle Nextion context pointer.
     * @param address Starting address to read from. Range: 0-NEX_DVC_EEPROM_MAX_ADDRESS
     * @param value Location where the value will be stored.
     *
     * @return NEX_OK if success, otherwise NEX_FAIL.
     */
    nex_err_t nextion_eeprom_read_number(nextion_handle_t handle,
                                         uint16_t address,
                                         int32_t *value);

    /**
     * @brief Reads raw bytes from the device EEPROM.
     *
     * @param handle Nextion context pointer.
     * @param address Starting address to read from. Range: 0-NEX_DVC_EEPROM_MAX_ADDRESS
     * @param buffer Buffer with enough capacity for the bytes retrieved.
     * @param buffer_length Buffer length.
     *
     * @return NEX_OK if success, otherwise NEX_FAIL.
     */
    nex_err_t nextion_eeprom_read_bytes(nextion_handle_t handle,
                                        uint16_t address,
                                        uint8_t *buffer,
                                        size_t buffer_length);

    /**
     * @brief Begins the EEPROM data streaming.
     *
     * @details When in this mode, the device will "hang" until all
     * data is sent; no event or other commands will be processed.
     *
     * @param handle Nextion context pointer.
     * @param address Starting address to write the bytes. Range: 0-NEX_DVC_EEPROM_MAX_ADDRESS
     * @param value_count How many values will be written. "value_count < (NEX_DVC_TRANSPARENT_DATA_MAX_DATA_SIZE - 20)"
     *
     * @return NEX_OK if success, otherwise NEX_FAIL.
     */
    nex_err_t nextion_eeprom_stream_begin(nextion_handle_t handle, uint16_t address, size_t value_count);

    /**
     * @brief Writes a value onto the EEPROM stream.
     *
     * @param handle Nextion context pointer.
     * @param value Value to be written.
     *
     * @return NEX_OK if success, otherwise NEX_FAIL.
     */
    nex_err_t nextion_eeprom_stream_write(nextion_handle_t handle, uint8_t value);

    /**
     * @brief Ends the EEPROM streaming.
     * @param handle Nextion context pointer.
     * @return NEX_OK if success, otherwise NEX_FAIL.
     */
    nex_err_t nextion_eeprom_stream_end(nextion_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif //__NEXTION_EEPROM_H__