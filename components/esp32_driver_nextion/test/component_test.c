#include "esp32_driver_nextion/component.h"
#include "common_infra_test.h"

TEST_CASE("Refresh component", "[component]")
{
    nex_err_t code = nextion_component_refresh(handle, "n0");

    CHECK_NEX_OK(code);
}

TEST_CASE("Cannot refresh invalid component", "[component]")
{
    nex_err_t code = nextion_component_refresh(handle, "n99");

    NEX_CODES_EQUAL(NEX_DVC_ERR_INVALID_COMPONENT, code);
}

TEST_CASE("Set component visibility", "[component]")
{
    nex_err_t code = nextion_component_set_visibility(handle, "n0", false);

    nextion_component_set_visibility(handle, "n0", true);

    CHECK_NEX_OK(code);
}

TEST_CASE("Cannot set invalid component visibility", "[component]")
{
    nex_err_t code = nextion_component_set_visibility(handle, "n99", false);

    NEX_CODES_EQUAL(NEX_DVC_ERR_INVALID_COMPONENT, code);
}

TEST_CASE("Set all components visibility", "[component]")
{
    nex_err_t code = nextion_component_set_visibility_all(handle, false);

    nextion_component_set_visibility_all(handle, true);

    CHECK_NEX_OK(code);
}

TEST_CASE("Set component touchability", "[component]")
{
    nex_err_t code = nextion_component_set_touchable(handle, "t0", false);

    nextion_component_set_touchable(handle, "t0", true);

    CHECK_NEX_OK(code);
}

TEST_CASE("Cannot set invalid component touchability", "[component]")
{
    nex_err_t code = nextion_component_set_touchable(handle, "t99", false);

    NEX_CODES_EQUAL(NEX_DVC_ERR_INVALID_COMPONENT, code);
}

TEST_CASE("Set all components touchability", "[component]")
{
    nex_err_t code = nextion_component_set_touchable_all(handle, false);

    nextion_component_set_touchable_all(handle, true);

    CHECK_NEX_OK(code);
}

TEST_CASE("Get component text", "[component]")
{
    char text[10];
    size_t length = 10;
    nex_err_t code = nextion_component_get_text(handle, "t0", text, length);

    CHECK_NEX_OK(code);
    STRCMP_EQUAL("test text", text);
}

TEST_CASE("Get component value", "[component]")
{
    int32_t number;
    nex_err_t code = nextion_component_get_value(handle, "n0", &number);

    CHECK_NEX_OK(code);
    LONGS_EQUAL(-5, number);
}

TEST_CASE("Get component boolean", "[component]")
{
    bool value;
    nex_err_t code = nextion_component_get_boolean(handle, "c0", &value);

    CHECK_NEX_OK(code);
    CHECK_TRUE(value);
}

TEST_CASE("Set component text", "[component]")
{
    char text[8];
    size_t length = 8;
    nex_err_t code = nextion_component_set_text(handle, "b0", (char *)"Button!");

    nextion_component_get_text(handle, "b0", text, length);

    CHECK_NEX_OK(code);
    STRCMP_EQUAL("Button!", text);
}

TEST_CASE("Set component value", "[component]")
{
    int32_t number = 0;
    nex_err_t code = nextion_component_set_value(handle, "x0", 100);

    nextion_component_get_value(handle, "x0", &number);

    CHECK_NEX_OK(code);
    LONGS_EQUAL(100, number);
}

TEST_CASE("Set component boolean", "[component]")
{
    bool value = false;
    nex_err_t code = nextion_component_set_boolean(handle, "r0", true);

    nextion_component_get_boolean(handle, "r0", &value);

    CHECK_NEX_OK(code);
    CHECK_TRUE(value);
}

TEST_CASE("Get component property text", "[component]")
{
    char text[10];
    size_t length = 10;
    nex_err_t code = nextion_component_get_property_text(handle, "t0", "txt", text, length);

    CHECK_NEX_OK(code);
    STRCMP_EQUAL("test text", text);
}

TEST_CASE("Get component property number", "[component]")
{
    int32_t number;
    nex_err_t code = nextion_component_get_property_number(handle, "n0", "val", &number);

    CHECK_NEX_OK(code);
    LONGS_EQUAL(-5, number);
}

TEST_CASE("Set component property text", "[component]")
{
    char text[8];
    size_t length = 8;
    nex_err_t code = nextion_component_set_property_text(handle, "b0", "txt", (char *)"Button!");

    nextion_component_get_text(handle, "b0", text, length);

    CHECK_NEX_OK(code);
    STRCMP_EQUAL("Button!", text);
}

TEST_CASE("Set component property number", "[component]")
{
    int32_t number = 0;
    nex_err_t code = nextion_component_set_property_number(handle, "x0", "val", 100);

    nextion_component_get_value(handle, "x0", &number);

    CHECK_NEX_OK(code);
    LONGS_EQUAL(100, number);
}