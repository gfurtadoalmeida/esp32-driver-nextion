#include "common_infra_test.h"
#include "nextion/op_component.h"

#ifdef __cplusplus
extern "C"
{
#endif

    TEST_CASE("Can get text from a text component", "[nextion][op_comp]")
    {
        DRIVER_INSTALL()

        char text[10];
        int length;
        nex_err_t code = nextion_get_text("get t0.txt", text, &length);

        TEST_ASSERT_NEX_OK(code);
        TEST_ASSERT_EQUAL_INT(9, length);
        TEST_ASSERT_EQUAL_STRING("test text", text);
    }

    TEST_CASE("Cannot get text from an invalid text component", "[nextion][op_comp]")
    {
        DRIVER_INSTALL()

        char text[10];
        int length;
        nex_err_t code = nextion_get_text("get t99.txt", text, &length);

        TEST_ASSERT_EQUAL_INT(NEX_DVC_ERR_INVALID_VARIABLE_OR_ATTRIBUTE, code);
    }

    TEST_CASE("Can get a number from a number component", "[nextion][op_comp]")
    {
        DRIVER_INSTALL()

        int number;
        nex_err_t code = nextion_get_number("get n0.val", &number);

        TEST_ASSERT_NEX_OK(code);
        TEST_ASSERT_EQUAL_INT(50, number);
    }

    TEST_CASE("Cannot get a number from an invalid number component", "[nextion][op_comp]")
    {
        DRIVER_INSTALL()

        int number;
        nex_err_t code = nextion_get_number("get n99.val", &number);

        TEST_ASSERT_EQUAL_INT(NEX_DVC_ERR_INVALID_VARIABLE_OR_ATTRIBUTE, code);
    }

    TEST_CASE("Can refresh a component", "[nextion][op_comp]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_component_refresh("n0");

        TEST_ASSERT_NEX_OK(code);
    }

    TEST_CASE("Cannot refresh an invalid component", "[nextion][op_comp]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_component_refresh("n99");

        TEST_ASSERT_EQUAL_INT(NEX_DVC_ERR_INVALID_COMPONENT, code);
    }

#ifdef __cplusplus
}
#endif
