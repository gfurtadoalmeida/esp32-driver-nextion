#ifndef __NEXTION_EVENTS_H__
#define __NEXTION_EVENTS_H__

#include <stdint.h>
#include <stdbool.h>
#include "nextion/base/codes.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @typedef nextion_device_state_t
     * @brief Nextion display device state.
     */
    typedef enum
    {
        // Values here must have the same values as those
        // defined in the instruction set manual. Will ease parsing.

        NEXTION_DEVICE_STARTED = NEX_DVC_EVT_HARDWARE_START_RESET,   /*!< Just started or was reset. */
        NEXTION_DEVICE_READY = NEX_DVC_EVT_HARDWARE_READY,           /*!< Ready to receive commands. */
        NEXTION_DEVICE_AUTO_SLEEP = NEX_DVC_EVT_HARDWARE_AUTO_SLEEP, /*!< Went to sleep automatically. */
        NEXTION_DEVICE_AUTO_WAKE = NEX_DVC_EVT_HARDWARE_AUTO_WAKE,   /*!< Woke up automatically. */
        NEXTION_DEVICE_UPGRADING = NEX_DVC_EVT_HARDWARE_UPGRADE      /*!< Is upgrading. Happens when it is powered up with a SD card inserted. */
    } nextion_device_state_t;

    /**
     * @typedef nextion_touch_state_t
     * @brief Nextion display touch state.
     */
    typedef enum
    {
        // Values here must have the same values as those
        // defined in the instruction set manual. Will ease parsing.

        NEXTION_TOUCH_PRESSED = 1U, /*!< Component was pressed (and not released). */
        NEXTION_TOUCH_RELEASED = 0U /*!< Component was released. */
    } nextion_touch_state_t;

    /**
     * @typedef event_callback_on_touch
     * @brief Callback for display touch event.
     * @param handle Nextion context pointer.
     * @param page_id Page id were the touch happened.
     * @param component_id Component id that was touched.
     * @param state Touch state
     */
    typedef void (*event_callback_on_touch)(nextion_handle_t,
                                            uint8_t,
                                            uint8_t,
                                            nextion_touch_state_t);

    /**
     * @typedef event_callback_on_touch_coord
     * @brief Callback for display touch event with coordinates.
     * @param handle Nextion context pointer.
     * @param x Page id were the touch happened.
     * @param y Component id that was touched.
     * @param state Touch state
     * @param exited_sleep If the touch happened while the device was asleep, and now it is awake.
     */
    typedef void (*event_callback_on_touch_coord)(nextion_handle_t,
                                                  uint16_t,
                                                  uint16_t,
                                                  nextion_touch_state_t,
                                                  bool);

    /**
     * @typedef event_callback_on_device
     * @brief Callback for display device event.
     * @param handle Nextion context pointer.
     * @param state Device state
     */
    typedef void (*event_callback_on_device)(nextion_handle_t, nextion_device_state_t);

    /**
     * @typedef nextion_event_callback_t
     * @brief Nextion display events callback functions.
     */
    typedef struct
    {
        event_callback_on_touch on_touch;             /*!< When a component is touch. */
        event_callback_on_touch_coord on_touch_coord; /*!< When something is touched and "sendxy=1". */
        event_callback_on_device on_device;           /*!< When a device event happens. */
    } nextion_event_callback_t;
#ifdef __cplusplus
}
#endif

#endif //__NEXTION_EVENTS_H__