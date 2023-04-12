#include "esp32_driver_nextion/rgb565/rgb565.h"

rgb565_t rgb565_convert_from_888(uint8_t red, uint8_t green, uint8_t blue)
{
    return (rgb565_t)(((red & 0b11111000) << 8) + ((green & 0b11111100) << 3) + (blue >> 3));
}
