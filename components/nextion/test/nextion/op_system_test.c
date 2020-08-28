#include "common_infra_test.h"
#include "nextion/op_system.h"

#ifdef __cplusplus
extern "C"
{
#endif

    TEST_CASE("Can get text from a text component", "[nextion][op_sys]")
    {
        DRIVER_INSTALL()

        char text[10];
        int length;
        nex_err_t code = nextion_system_get_text("get t0.txt", text, &length);

        TEST_ASSERT_NEX_OK(code);
        TEST_ASSERT_EQUAL_INT(9, length);
        TEST_ASSERT_EQUAL_STRING("test text", text);
    }

    TEST_CASE("Cannot get text from an invalid text component", "[nextion][op_sys]")
    {
        DRIVER_INSTALL()

        char text[10];
        int length;
        nex_err_t code = nextion_system_get_text("get t99.txt", text, &length);

        TEST_ASSERT_EQUAL_INT(NEX_DVC_ERR_INVALID_VARIABLE_OR_ATTRIBUTE, code);
    }

    TEST_CASE("Can get a number from a number component", "[nextion][op_sys]")
    {
        DRIVER_INSTALL()

        int number;
        nex_err_t code = nextion_system_get_number("get n0.val", &number);

        TEST_ASSERT_NEX_OK(code);
        TEST_ASSERT_EQUAL_INT(50, number);
    }

    TEST_CASE("Cannot get a number from an invalid number component", "[nextion][op_sys]")
    {
        DRIVER_INSTALL()

        int number;
        nex_err_t code = nextion_system_get_number("get n99.val", &number);

        TEST_ASSERT_EQUAL_INT(NEX_DVC_ERR_INVALID_VARIABLE_OR_ATTRIBUTE, code);
    }

    TEST_CASE("Can get the display brightness", "[nextion][op_sys]")
    {
        DRIVER_INSTALL()

        int percentage;
        nex_err_t code = nextion_system_get_brightness(false, &percentage);

        TEST_ASSERT_NEX_OK(code);
    }

    TEST_CASE("Can get the persisted display brightness", "[nextion][op_sys]")
    {
        DRIVER_INSTALL()

        int percentage;
        nex_err_t code = nextion_system_get_brightness(true, &percentage);

        TEST_ASSERT_NEX_OK(code);
    }

    TEST_CASE("Cannot get the display brightness when percentage is null", "[nextion][op_sys]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_system_get_brightness(false, NULL);

        TEST_ASSERT_NEX_FAIL(code);
    }

    TEST_CASE("Can set the display brightness", "[nextion][op_sys]")
    {
        DRIVER_INSTALL()

        int percentage;

        nextion_system_get_brightness(false, &percentage);

        nex_err_t code = nextion_system_set_brightness(percentage, false);

        TEST_ASSERT_NEX_OK(code);
    }

    TEST_CASE("Can set and persist the display brightness", "[nextion][op_sys]")
    {
        DRIVER_INSTALL()

        int percentage;

        nextion_system_get_brightness(true, &percentage);

        nex_err_t code = nextion_system_set_brightness(percentage, true);

        TEST_ASSERT_NEX_OK(code);
    }

    TEST_CASE("Can set sleep on no touch", "[nextion][op_sys]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_system_set_sleep_no_touch(60);

        TEST_ASSERT_NEX_OK(code);
    }

    TEST_CASE("Can set wake up when touched", "[nextion][op_sys]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_system_set_wake_on_touch(true);

        TEST_ASSERT_NEX_OK(code);
    }

#ifdef __cplusplus
}
#endif
