#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "common_infra_test.h"
#include "nextion/system.h"
#include "nextion/base/constants.h"

TEST_CASE("Get text from text component", "[system]")
{
    char text[10];
    size_t length = 10;
    nex_err_t code = nextion_system_get_text(handle, "get t0.txt", text, &length);

    CHECK_NEX_OK(code);
    SIZET_EQUAL(9, length);
    STRCMP_EQUAL("test text", text);
}

TEST_CASE("Cannot get text from invalid text component", "[system]")
{
    char text[10];
    size_t length = 10;
    nex_err_t code = nextion_system_get_text(handle, "get t99.txt", text, &length);

    NEX_CODES_EQUAL(NEX_DVC_ERR_INVALID_VARIABLE_OR_ATTRIBUTE, code);
}

TEST_CASE("Get number from number component", "[system]")
{
    int32_t number;
    nex_err_t code = nextion_system_get_number(handle, "get n0.val", &number);

    CHECK_NEX_OK(code);
    LONGS_EQUAL(50, number);
}

TEST_CASE("Cannot get number from invalid number component", "[system]")
{
    int32_t number;
    nex_err_t code = nextion_system_get_number(handle, "get n99.val", &number);

    NEX_CODES_EQUAL(NEX_DVC_ERR_INVALID_VARIABLE_OR_ATTRIBUTE, code);
}

TEST_CASE("Get display brightness", "[system]")
{
    uint8_t current_percentage = 0;
    uint8_t percentage = 0;

    nextion_system_get_brightness(handle, false, &current_percentage);
    nextion_system_set_brightness(handle, 50, false);

    nex_err_t code = nextion_system_get_brightness(handle, false, &percentage);

    nextion_system_set_brightness(handle, current_percentage, false);

    CHECK_NEX_OK(code);
    TEST_ASSERT_EQUAL_UINT8(50, percentage);
}

TEST_CASE("Get persisted display brightness", "[system]")
{
    uint8_t current_percentage = 0;
    uint8_t percentage = 0;

    nextion_system_get_brightness(handle, true, &current_percentage);
    nextion_system_set_brightness(handle, 50, true);

    nex_err_t code = nextion_system_get_brightness(handle, true, &percentage);

    nextion_system_set_brightness(handle, current_percentage, true);

    CHECK_NEX_OK(code);
    TEST_ASSERT_EQUAL_UINT8(50, percentage);
}

TEST_CASE("Cannot get display brightness when percentage null", "[system]")
{
    nex_err_t code = nextion_system_get_brightness(handle, false, NULL);

    CHECK_NEX_FAIL(code);
}

TEST_CASE("Set display brightness", "[system]")
{
    uint8_t percentage = 0;
    uint8_t new_percentage = 0;

    nextion_system_get_brightness(handle, false, &percentage);

    nex_err_t code = nextion_system_set_brightness(handle, 20, false);

    nextion_system_get_brightness(handle, false, &new_percentage);
    nextion_system_set_brightness(handle, percentage, false);

    CHECK_NEX_OK(code);
    TEST_ASSERT_EQUAL_UINT8(20, new_percentage);
}

TEST_CASE("Set persisted display brightness", "[system]")
{
    uint8_t percentage = 0;
    uint8_t new_percentage = 0;

    nextion_system_get_brightness(handle, true, &percentage);

    nex_err_t code = nextion_system_set_brightness(handle, 20, true);

    nextion_system_get_brightness(handle, true, &new_percentage);
    nextion_system_set_brightness(handle, percentage, true);

    CHECK_NEX_OK(code);
    TEST_ASSERT_EQUAL_UINT8(20, new_percentage);
}

TEST_CASE("Get sleep on no touch", "[system]")
{
    uint16_t seconds;
    nex_err_t code = nextion_system_get_sleep_no_touch(handle, &seconds);

    CHECK_NEX_OK(code);
}

TEST_CASE("Cannot get sleep on no touch when seconds null", "[system]")
{
    nex_err_t code = nextion_system_get_sleep_no_touch(handle, NULL);

    CHECK_NEX_FAIL(code);
}

TEST_CASE("Set sleep on no touch", "[system]")
{
    uint16_t current_sleep = 0;
    uint16_t sleep = 0;

    nextion_system_get_sleep_no_touch(handle, &current_sleep);

    nex_err_t code = nextion_system_set_sleep_no_touch(handle, 60);

    nextion_system_get_sleep_no_touch(handle, &sleep);
    nextion_system_set_sleep_no_touch(handle, current_sleep);

    CHECK_NEX_OK(code);
    TEST_ASSERT_EQUAL_UINT8(60, sleep);
}

TEST_CASE("Set wake up when touched", "[system]")
{
    nex_err_t code = nextion_system_set_wake_on_touch(handle, true);

    CHECK_NEX_OK(code);
}

TEST_CASE("Reset", "[system]")
{
    nex_err_t code = nextion_system_reset(handle);

    vTaskDelay(pdMS_TO_TICKS(NEX_DVC_RESET_WAIT_TIME_MS));

    // As all logic relies on "bkcmd=3" and this configuration
    // is lost on reset, we need to configure it again.
    nextion_init(handle);

    CHECK_NEX_OK(code);
}