#include "common_infra_test.h"
#include "nextion/nextion.h"

TEST_CASE("Cannot init null context", "[core]")
{
    nex_err_t result = nextion_init(NULL);

    CHECK_NEX_FAIL(result);
}

TEST_CASE("Send command", "[core]")
{
    nex_err_t result = nextion_command_send(handle, "page 0");

    CHECK_NEX_OK(result);
}

TEST_CASE("Cannot send command with null handle", "[core]")
{
    nex_err_t result = nextion_command_send(NULL, "");

    CHECK_NEX_FAIL(result);
}

TEST_CASE("Cannot send null command", "[core]")
{
    nex_err_t result = nextion_command_send(handle, NULL);

    CHECK_NEX_FAIL(result);
}

TEST_CASE("Transparent data mode begin", "[core]")
{
    nex_err_t result = nextion_transparent_data_mode_begin(handle, 1, "wept 0,1");

    nextion_transparent_data_mode_write(handle, 0);

    nextion_transparent_data_mode_end(handle);

    CHECK_NEX_OK(result);
}

TEST_CASE("Cannot begin transparent data mode with too much data", "[core]")
{
    nex_err_t result = nextion_transparent_data_mode_begin(handle, NEX_DVC_TRANSPARENT_DATA_MAX_DATA_SIZE, "wept 0,1");

    CHECK_NEX_FAIL(result);
}

TEST_CASE("Transparent data mode write", "[core]")
{
    nextion_transparent_data_mode_begin(handle, 1, "wept 0,1");

    nex_err_t result = nextion_transparent_data_mode_write(handle, 0);

    nextion_transparent_data_mode_end(handle);

    CHECK_NEX_OK(result);
}

TEST_CASE("Cannot write on unstarted transparent data mode", "[core]")
{
    nex_err_t result = nextion_transparent_data_mode_write(handle, 0);

    CHECK_NEX_FAIL(result);
}

TEST_CASE("Transparent data mode end", "[core]")
{
    nextion_transparent_data_mode_begin(handle, 1, "wept 0,1");

    nextion_transparent_data_mode_write(handle, 0);

    nex_err_t result = nextion_transparent_data_mode_end(handle);

    CHECK_NEX_OK(result);
}

TEST_CASE("Cannot end unstarted transparent data mode", "[core]")
{
    nex_err_t result = nextion_transparent_data_mode_end(handle);

    CHECK_NEX_FAIL(result);
}

TEST_CASE("Cannot send command when in transparent data mode", "[core]")
{
    nextion_transparent_data_mode_begin(handle, 1, "wept 0,1");

    nex_err_t result = nextion_command_send(handle, "page 1");

    nextion_transparent_data_mode_write(handle, 0);
    nextion_transparent_data_mode_end(handle);

    CHECK_NEX_FAIL(result);
}

TEST_CASE("Cannot process events when in transparent data mode", "[core]")
{
    nextion_transparent_data_mode_begin(handle, 1, "wept 0,1");

    bool result = nextion_event_process(handle);

    nextion_transparent_data_mode_write(handle, 0);
    nextion_transparent_data_mode_end(handle);

    CHECK_FALSE(result);
}
