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

#ifdef __cplusplus
}
#endif
