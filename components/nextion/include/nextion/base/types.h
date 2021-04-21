#ifndef __NEXTION_TYPES_H__
#define __NEXTION_TYPES_H__

#ifdef __cplusplus
extern "C"
{
#endif
    /**
     * @typedef nextion_t
     * @brief Nextion context.
     */
    typedef struct nextion_t nextion_t;

    /**
     * @typedef nextion_handle_t
     * @brief Pointer to a Nextion context.
     */
    typedef nextion_t *nextion_handle_t;

    /**
     * @typedef nextion_baud_rate_t
     * @brief Supported baud rates.
     */
    typedef enum
    {
        NEXTION_BAUD_RATE_2400 = 2400U,
        NEXTION_BAUD_RATE_4800 = 4800U,
        NEXTION_BAUD_RATE_9600 = 9600U,
        NEXTION_BAUD_RATE_19200 = 19200U,
        NEXTION_BAUD_RATE_31250 = 31250U,
        NEXTION_BAUD_RATE_38400 = 38400U,
        NEXTION_BAUD_RATE_57600 = 57600U,
        NEXTION_BAUD_RATE_115200 = 115200U,
        NEXTION_BAUD_RATE_230400 = 230400U,
        NEXTION_BAUD_RATE_250000 = 250000U,
        NEXTION_BAUD_RATE_256000 = 256000U,
        NEXTION_BAUD_RATE_512000 = 512000U,
        NEXTION_BAUD_RATE_921600 = 921600U
    } nextion_baud_rate_t;

#ifdef __cplusplus
}
#endif

#endif //__NEXTION_TYPES_H__