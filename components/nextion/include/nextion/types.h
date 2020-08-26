#ifndef __NEXTION_TYPES_H__
#define __NEXTION_TYPES_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @typedef nextion_event_type_t
     * @brief Nextion display event type.
     */
    typedef enum
    {
        NEXTION_EVENT_DEVICE,     /*!< Device state change occurred. */
        NEXTION_EVENT_TOUCH,      /*!< Touch event occurred. */
        NEXTION_EVENT_TOUCH_COORD /*!< Touch event, with coordinates, occurred. */
    } nextion_event_type_t;

    /**
     * @typedef nextion_device_state_t
     * @brief Nextion display device state.
     */
    typedef enum
    {
        NEXTION_DEVICE_STARTED,              /*!< Just started or was reset. */
        NEXTION_DEVICE_READY,                /*!< Ready to receive commands. */
        NEXTION_DEVICE_AUTO_SLEEP,           /*!< Went to sleep automatically. */
        NEXTION_DEVICE_AUTO_WAKE,            /*!< Woke up automatically. */
        NEXTION_DEVICE_UPGRADING,            /*!< Is upgrading. Happens when it is powered up with a SD card inserted. */
        NEXTION_DEVICE_TRANSP_DATA_READY,    /*!< Ready to receive data using "Transparent Data" mode. */
        NEXTION_DEVICE_TRANSP_DATA_FINISHED, /*!< Finished receiving data using "Transparent Data" mode. */
    } nextion_device_state_t;

    /**
     * @typedef nextion_touch_state_t
     * @brief Nextion display touch state.
     */
    typedef enum
    {
        NEXTION_TOUCH_PRESSED, /*!< Component was pressed (and not released). */
        NEXTION_TOUCH_RELEASED /*!< Component was released. */
    } nextion_touch_state_t;

    /**
     * @typedef nextion_touch_t
     * @brief Nextion display touch data.
     */
    typedef struct
    {
        uint8_t page_id;             /*!< Page id were the touch happened. */
        uint8_t component_id;        /*!< Component id that was touched. */
        nextion_touch_state_t state; /*!< Touch state. */
    } nextion_touch_t;

    /**
     * @typedef nextion_touch_coord_t
     * @brief Nextion display touch coordinate data.
     */
    typedef struct
    {
        uint16_t x;                  /*!< X coordinate. */
        uint16_t y;                  /*!< Y coordinate. */
        bool exited_sleep;           /*!< If the touch happened while the device was asleep, and now it is awake. */
        nextion_touch_state_t state; /*!< Touch state. */
    } nextion_touch_coord_t;

    /**
     * @typedef nextion_event_t
     * @brief Nextion display event.
     */
    typedef struct
    {
        nextion_event_type_t type; /*!< Event type. */
        union {
            nextion_device_state_t device_state; /*!< Device state. Use when "type == NEXTION_EVENT_DEVICE" */
            nextion_touch_t touch;               /*!< Touch state. Use when "type == NEXTION_EVENT_TOUCH"  */
            nextion_touch_coord_t touch_coord;   /*!< Touch state. Use when "type == NEXTION_EVENT_TOUCH_COORD" */
        };
    } nextion_event_t;

#ifdef __cplusplus
}
#endif

#endif //__NEXTION_TYPES_H__