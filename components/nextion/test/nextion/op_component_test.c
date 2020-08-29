#include "common_infra_test.h"
#include "nextion/op_component.h"

#ifdef __cplusplus
extern "C"
{
#endif

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

    TEST_CASE("Can set a component visibility", "[nextion][op_comp]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_component_set_visibility("n0", false);

        nextion_component_set_visibility("n0", true);

        TEST_ASSERT_NEX_OK(code);
    }

    TEST_CASE("Cannot set an invalid component visibility", "[nextion][op_comp]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_component_set_visibility("n99", false);

        TEST_ASSERT_EQUAL_INT(NEX_DVC_ERR_INVALID_COMPONENT, code);
    }

    TEST_CASE("Can set all components visibility", "[nextion][op_comp]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_component_set_visibility_all(false);

        nextion_component_set_visibility_all(true);

        TEST_ASSERT_NEX_OK(code);
    }

    TEST_CASE("Can set a component touchability", "[nextion][op_comp]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_component_set_touchable("t0", false);

        nextion_component_set_touchable("t0", true);

        TEST_ASSERT_NEX_OK(code);
    }

    TEST_CASE("Cannot set an invalid component touchability", "[nextion][op_comp]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_component_set_touchable("t99", false);

        TEST_ASSERT_EQUAL_INT(NEX_DVC_ERR_INVALID_COMPONENT, code);
    }

    TEST_CASE("Can set all components touchability", "[nextion][op_comp]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_component_set_touchable_all(false);

        nextion_component_set_touchable_all(true);

        TEST_ASSERT_NEX_OK(code);
    }

    TEST_CASE("Can get a component text", "[nextion][op_comp]")
    {
        DRIVER_INSTALL()

        char text[10];
        int length;
        nex_err_t code = nextion_component_get_text("t0", text, &length);

        TEST_ASSERT_NEX_OK(code);
        TEST_ASSERT_EQUAL_INT(9, length);
        TEST_ASSERT_EQUAL_STRING("test text", text);
    }

    TEST_CASE("Can get a component number", "[nextion][op_comp]")
    {
        DRIVER_INSTALL()

        int number;
        nex_err_t code = nextion_component_get_number("n0", &number);

        TEST_ASSERT_NEX_OK(code);
        TEST_ASSERT_EQUAL_INT(50, number);
    }

    TEST_CASE("Can get a component boolean", "[nextion][op_comp]")
    {
        DRIVER_INSTALL()

        bool value;
        nex_err_t code = nextion_component_get_boolean("c0", &value);

        TEST_ASSERT_NEX_OK(code);
        TEST_ASSERT_TRUE(value);
    }

    TEST_CASE("Can set a component text", "[nextion][op_comp]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_component_set_text("b0", "Button!");

        TEST_ASSERT_NEX_OK(code);
    }

    TEST_CASE("Can set a component number", "[nextion][op_comp]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_component_set_number("x0", 100);

        TEST_ASSERT_NEX_OK(code);
    }

    TEST_CASE("Can set a component boolean", "[nextion][op_comp]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_component_set_boolean("r0", false);

        TEST_ASSERT_NEX_OK(code);
    }

#ifdef __cplusplus
}
#endif
