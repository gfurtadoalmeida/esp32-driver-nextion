#include "esp32_driver_nextion/waveform.h"
#include "common_infra_test.h"

#define TEST_WAVEFORM_ID 7U

TEST_CASE("Start waveform refresh", "[waveform]")
{
    nex_err_t code = nextion_waveform_start_refesh(handle);

    CHECK_NEX_OK(code);
}

TEST_CASE("Stop waveform refresh", "[waveform]")
{
    nex_err_t code = nextion_waveform_stop_refesh(handle);

    nextion_waveform_start_refesh(handle);

    CHECK_NEX_OK(code);
}

TEST_CASE("Add value to waveform", "[waveform]")
{
    nex_err_t code = nextion_waveform_add_value(handle, TEST_WAVEFORM_ID, 0, 50);

    CHECK_NEX_OK(code);
}

TEST_CASE("Cannot add value to invalid waveform", "[waveform]")
{
    nex_err_t code = nextion_waveform_add_value(handle, 50, 0, 50);

    NEX_CODES_EQUAL(NEX_DVC_ERR_INVALID_WAVEFORM, code);
}

TEST_CASE("Cannot add value to invalid waveform channel", "[waveform]")
{
    nex_err_t code = nextion_waveform_add_value(handle, TEST_WAVEFORM_ID, 5, 50);

    NEX_CODES_EQUAL(NEX_DVC_ERR_INVALID_WAVEFORM, code);
}

TEST_CASE("Clear waveform channel", "[waveform]")
{
    nex_err_t code = nextion_waveform_clear_channel(handle, TEST_WAVEFORM_ID, 0);

    CHECK_NEX_OK(code);
}

TEST_CASE("Cannot clear waveform channel of invalid waveform", "[waveform]")
{
    nex_err_t code = nextion_waveform_clear_channel(handle, 50, 0);

    NEX_CODES_EQUAL(NEX_DVC_ERR_INVALID_WAVEFORM, code);
}

TEST_CASE("Cannot clear invalid waveform channel", "[waveform]")
{
    nex_err_t code = nextion_waveform_clear_channel(handle, TEST_WAVEFORM_ID, 5);

    NEX_CODES_EQUAL(NEX_DVC_ERR_INVALID_WAVEFORM, code);
}

TEST_CASE("Clear waveform", "[waveform]")
{
    nex_err_t code = nextion_waveform_clear(handle, TEST_WAVEFORM_ID);

    CHECK_NEX_OK(code);
}

TEST_CASE("Cannot clear invalid waveform", "[waveform]")
{
    nex_err_t code = nextion_waveform_clear(handle, 50);

    NEX_CODES_EQUAL(NEX_DVC_ERR_INVALID_WAVEFORM, code);
}

TEST_CASE("Stream works", "[waveform]")
{
    if (nextion_waveform_stream_begin(handle, TEST_WAVEFORM_ID, 0, 50) != NEX_OK)
    {
        FAIL_TEST("could not start streaming");
    }

    for (int i = 0; i < 50; i++)
    {
        if (nextion_waveform_stream_write(handle, i) == NEX_FAIL)
        {
            nextion_waveform_stream_end(handle);

            FAIL_TEST("could not write value to stream");
        }
    }

    nextion_waveform_stream_end(handle);
}

TEST_CASE("Cannot start stream with invalid waveform", "[waveform]")
{
    nex_err_t code = nextion_waveform_stream_begin(handle, 50, 0, 50);

    NEX_CODES_EQUAL(NEX_DVC_ERR_INVALID_WAVEFORM, code);
}

TEST_CASE("Cannot start stream with invalid waveform channel", "[waveform]")
{
    nex_err_t code = nextion_waveform_stream_begin(handle, 50, 5, 50);

    NEX_CODES_EQUAL(NEX_DVC_ERR_INVALID_WAVEFORM, code);
}

TEST_CASE("Cannot start stream with invalid value count", "[waveform]")
{
    nex_err_t code = nextion_waveform_stream_begin(handle, TEST_WAVEFORM_ID, 0, NEX_DVC_TRANSPARENT_DATA_MAX_DATA_SIZE);

    CHECK_NEX_FAIL(code);
}

TEST_CASE("Cannot write to unstarted stream", "[waveform]")
{
    nex_err_t code = nextion_waveform_stream_write(handle, 100);

    CHECK_NEX_FAIL(code);
}

TEST_CASE("Cannot end unstarted stream", "[waveform]")
{
    nex_err_t code = nextion_waveform_stream_end(handle);

    CHECK_NEX_FAIL(code);
}