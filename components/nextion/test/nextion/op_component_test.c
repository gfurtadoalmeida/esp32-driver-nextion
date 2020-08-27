#include "common_infra_test.h"
#include "nextion/op_component.h"

#ifdef __cplusplus
extern "C"
{
#endif

    TEST_CASE("Can get text from a text component", "[nextion][op_comp]")
    {
        INSTALL()

        char text[10];
        int length;
        nex_err_t code = nextion_get_text("get t0.txt", text, &length);

        DELETE()

        TEST_ASSERT_NEX_OK(code);
        TEST_ASSERT_EQUAL_INT(9, length);
        TEST_ASSERT_EQUAL_STRING("test text", text);
    }

    TEST_CASE("Can get a number from a number component", "[nextion][op_comp]")
    {
        INSTALL()

        int number;
        nex_err_t code = nextion_get_number("get n0.val", &number);

        DELETE()

        TEST_ASSERT_NEX_OK(code);
        TEST_ASSERT_EQUAL_INT(50, number);
    }

#ifdef __cplusplus
}
#endif
