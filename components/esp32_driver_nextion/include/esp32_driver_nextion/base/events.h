#ifndef __ESP32_DRIVER_NEXTION_BASE_EVENTS_H__
#define __ESP32_DRIVER_NEXTION_BASE_EVENTS_H__

#include <stdint.h>
#include <stdbool.h>
#include "codes.h"

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

        NEXTION_DEVICE_STARTED = NEX_DVC_EVT_HARDWARE_START_RESET,   /** @brief Just started or was reset. */
        NEXTION_DEVICE_READY = NEX_DVC_EVT_HARDWARE_READY,           /** @brief Ready to receive commands. */
        NEXTION_DEVICE_AUTO_SLEEP = NEX_DVC_EVT_HARDWARE_AUTO_SLEEP, /** @brief Went to sleep automatically. */
        NEXTION_DEVICE_AUTO_WAKE = NEX_DVC_EVT_HARDWARE_AUTO_WAKE,   /** @brief Woke up automatically. */
        NEXTION_DEVICE_UPGRADING = NEX_DVC_EVT_HARDWARE_UPGRADE      /** @brief Is upgrading. Happens when it is powered up with a SD card inserted. */
    } nextion_device_state_t;

    /**
     * @typedef nextion_touch_state_t
     * @brief Nextion display touch state.
     */
    typedef enum
    {
        // Values here must have the same values as those
        // defined in the instruction set manual. Will ease parsing.

        NEXTION_TOUCH_PRESSED = 1U, /** @brief Component was pressed (and not released). */
        NEXTION_TOUCH_RELEASED = 0U /** @brief Component was released. */
    } nextion_touch_state_t;

    /**
     * @typedef nextion_on_touch_event_t
     * @brief Touch event data.
     */
    typedef struct
    {
        nextion_t *handle;           /** @brief Nextion context pointer. */
        uint8_t page_id;             /** @brief Page id were the touch happened. */
        uint8_t component_id;        /** @brief Component id that was touched. */
        nextion_touch_state_t state; /** @brief Touch state */
    } nextion_on_touch_event_t;

    /**
     * @typedef nextion_on_touch_coord_event_t
     * @brief Touch event data with coordinates.
     */
    typedef struct
    {
        nextion_t *handle;           /** @brief Nextion context pointer. */
        uint16_t x;                  /** @brief Page id were the touch happened. */
        uint16_t y;                  /** @brief Component id that was touched. */
        nextion_touch_state_t state; /** @brief Touch state. */
        bool exited_sleep;           /** @brief If the touch happened while the device was asleep, and now it is awake. */
    } nextion_on_touch_coord_event_t;

    /**
     * @typedef nextion_on_device_event_t
     * @brief Device event data.
     */
    typedef struct
    {
        nextion_t *handle;            /** @brief Nextion context pointer. */
        nextion_device_state_t state; /** @brief Device state. */
    } nextion_on_device_event_t;

    /**
     * @typedef event_callback_on_touch
     * @brief Callback for display touch event.
     */
    typedef void (*event_callback_on_touch)(nextion_on_touch_event_t);

    /**
     * @typedef event_callback_on_touch_coord
     * @brief Callback for display touch event with coordinates.
     */
    typedef void (*event_callback_on_touch_coord)(nextion_on_touch_coord_event_t);

    /**
     * @typedef event_callback_on_device
     * @brief Callback for display device event.
     */
    typedef void (*event_callback_on_device)(nextion_on_device_event_t);

#ifdef __cplusplus
}
#endif
#endif