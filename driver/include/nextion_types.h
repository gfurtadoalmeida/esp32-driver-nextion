#ifndef __NEXTION_TYPES_H__
#define __NEXTION_TYPES_H__

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @typedef
     * @brief Nextion display event type.
     */
    typedef enum
    {
        NEXTION_EVENT_DEVICE, /*!< Device state change occurred. */
        NEXTION_EVENT_TOUCH   /*!< Touch event occurred. */
    } nextion_event_type_t;

    /**
     * @typedef
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
     * @typedef
     * @brief Nextion display touch state.
     */
    typedef enum
    {
        NEXTION_TOUCH_PRESSED, /*!< Component was pressed (and not released). */
        NEXTION_TOUCH_RELEASED /*!< Component was released. */
    } nextion_touch_state_t;

    /**
     * @typedef
     * @brief Nextion display touch data.
     */
    typedef struct
    {
        unsigned char page_id;       /*!< Page id were the touch happened. */
        unsigned char component_id;  /*!< Component id that was touched. */
        nextion_touch_state_t state; /*!< Touch state. */
    } nextion_touch_t;

    /**
     * @typedef
     * @brief Nextion display event.
     */
    typedef struct
    {
        nextion_event_type_t type; /*!< Event type. */
        union {
            nextion_device_state_t device_state; /*!< Device state. Use when "type == NEXTION_EVENT_DEVICE" */
            nextion_touch_t touch;               /*!< Touch state. Use when "type == NEXTION_EVENT_TOUCH" */
        };
    } nextion_event_t;

#ifdef __cplusplus
}
#endif

#endif //__NEXTION_TYPES_H__