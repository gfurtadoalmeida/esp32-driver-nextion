#include <limits.h>
#include "driver/uart.h"
#include "driver/gpio.h"
#include "freertos/queue.h"
#include "unity.h"
#include "nextion.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define INSTALL()                                                                          \
    nextion_driver_install(UART_NUM_2, 9600, GPIO_NUM_17, GPIO_NUM_16, 5, &p_event_queue); \
    nextion_driver_init();

#define DELETE() nextion_driver_delete();

    static QueueHandle_t p_event_queue;

    TEST_CASE("Can get text from a text component", "[get-text]")
    {
        INSTALL()

        char text[10];
        int length = nextion_get_text("get t0.txt", text);

        DELETE()

        TEST_ASSERT_EQUAL_INT(9, length);
        TEST_ASSERT_EQUAL_STRING("test text", text);
    }

    TEST_CASE("Can get a number from a number component", "[get-number]")
    {
        INSTALL()

        int number;
        bool success = nextion_get_number("get n0.val", &number);

        DELETE()

        TEST_ASSERT_TRUE(success);
        TEST_ASSERT_EQUAL_INT(50, number);
    }

#ifdef __cplusplus
}
#endif
