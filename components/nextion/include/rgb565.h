#ifndef __RGB565_H__
#define __RGB565_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * RGB565: uses 16 bits (2 bytes) to encode a color.
     *
     * MSB 11111 000000 11111 LSB
     *      Red  Green  Blue
     *    5 bits 6 bits 5 bits
     *
     * To better understand RGB565 I highly recommend this
     * article: http://www.barth-dev.de/online/rgb565-color-picker/#
     */

#define RGB565_COLOR_BLACK ((uint16_t)0b0000000000000000)
#define RGB565_COLOR_BLUE ((uint16_t)0b0000000000011111)
#define RGB565_COLOR_GREEN ((uint16_t)0b0000011111100000)
#define RGB565_COLOR_GRAY ((uint16_t)0b1000010000110000)
#define RGB565_COLOR_BROWN ((uint16_t)0b1011110001000000)
#define RGB565_COLOR_RED ((uint16_t)0b1111100000000000)
#define RGB565_COLOR_YELLOW ((uint16_t)0b1111111111100000)
#define RGB565_COLOR_WHITE ((uint16_t)0b1111111111111111)

    uint16_t rgb565_convert_from_888(uint8_t red, uint8_t green, uint8_t blue);

#ifdef __cplusplus
}
#endif

#endif //__RGB565_H__