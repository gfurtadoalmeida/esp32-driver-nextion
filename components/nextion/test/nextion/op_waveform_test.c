#include "common_infra_test.h"
#include "nextion/op_waveform.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define TEST_WAVEFORM_ID 7U

    TEST_CASE("Can start waveform refresh", "[nextion][op_waveform]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_waveform_start_refesh();

        TEST_ASSERT_NEX_OK(code);
    }

    TEST_CASE("Can stop waveform refresh", "[nextion][op_waveform]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_waveform_stop_refesh();

        nextion_waveform_start_refesh();

        TEST_ASSERT_NEX_OK(code);
    }

    TEST_CASE("Can add value to a waveform", "[nextion][op_waveform]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_waveform_add_value(TEST_WAVEFORM_ID, 0, 50);

        TEST_ASSERT_NEX_OK(code);
    }

    TEST_CASE("Cannot add value to invalid waveform", "[nextion][op_waveform]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_waveform_add_value(50, 0, 50);

        TEST_ASSERT_EQUAL_INT(NEX_DVC_ERR_INVALID_WAVEFORM, code);
    }

     TEST_CASE("Cannot add value to invalid waveform channel", "[nextion][op_waveform]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_waveform_add_value(TEST_WAVEFORM_ID, 5, 50);

        TEST_ASSERT_EQUAL_INT(NEX_DVC_ERR_INVALID_WAVEFORM, code);
    }

    TEST_CASE("Can clear a waveform channel", "[nextion][op_waveform]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_waveform_clear_channel(TEST_WAVEFORM_ID, 0);

        TEST_ASSERT_NEX_OK(code);
    }

    TEST_CASE("Cannot clear a waveform channel of an invalid waveform", "[nextion][op_waveform]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_waveform_clear_channel(50, 0);

        TEST_ASSERT_EQUAL_INT(NEX_DVC_ERR_INVALID_WAVEFORM, code);
    }

    TEST_CASE("Cannot clear an invalid waveform channel", "[nextion][op_waveform]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_waveform_clear_channel(TEST_WAVEFORM_ID, 5);

        TEST_ASSERT_EQUAL_INT(NEX_DVC_ERR_INVALID_WAVEFORM, code);
    }

    TEST_CASE("Can clear a waveform", "[nextion][op_waveform]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_waveform_clear(TEST_WAVEFORM_ID);

        TEST_ASSERT_NEX_OK(code);
    }

    TEST_CASE("Cannot clear an invalid waveform", "[nextion][op_waveform]")
    {
        DRIVER_INSTALL()

        nex_err_t code = nextion_waveform_clear(50);

        TEST_ASSERT_EQUAL_INT(NEX_DVC_ERR_INVALID_WAVEFORM, code);
    }

#ifdef __cplusplus
}
#endif
