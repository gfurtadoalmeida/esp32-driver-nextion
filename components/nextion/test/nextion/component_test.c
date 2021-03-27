#include "common_infra_test.h"
#include "nextion/component.h"

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
    nex_err_t code = nextion_component_get_text(handle, "t0", text, &length);

    CHECK_NEX_OK(code);
    SIZET_EQUAL(9, length);
    STRCMP_EQUAL("test text", text);
}

TEST_CASE("Get component number", "[component]")
{
    int number;
    nex_err_t code = nextion_component_get_number(handle, "n0", &number);

    CHECK_NEX_OK(code);
    LONGS_EQUAL(50, number);
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
    nex_err_t code = nextion_component_set_text(handle, "b0", "Button!");

    CHECK_NEX_OK(code);
}

TEST_CASE("Set component number", "[component]")
{
    nex_err_t code = nextion_component_set_number(handle, "x0", 100);

    CHECK_NEX_OK(code);
}

TEST_CASE("Set component boolean", "[component]")
{
    nex_err_t code = nextion_component_set_boolean(handle, "r0", false);

    CHECK_NEX_OK(code);
}
