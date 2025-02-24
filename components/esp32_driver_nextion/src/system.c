#include "esp32_driver_nextion/system.h"
#include "protocol/protocol.h"
#include "assertion.h"

nex_err_t nextion_system_reset(nextion_t *handle)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    return nextion_protocol_send_instruction(handle, "rest", 4, NULL);
}

nex_err_t nextion_system_sleep(nextion_t *handle)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    return nextion_protocol_send_instruction_ack(handle, "sleep=1");
}

nex_err_t nextion_system_wakeup(nextion_t *handle)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    return nextion_protocol_send_instruction_ack(handle, "sleep=0");
}

nex_err_t nextion_system_get_brightness(nextion_t *handle, bool persisted, uint8_t *percentage)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK((percentage != NULL), "percentage error(NULL)", NEX_FAIL)

    int32_t value = 0;
    nex_err_t result;

    if (persisted)
    {
        result = nextion_system_get_variable_number(handle, "dims", &value);
    }
    else
    {
        result = nextion_system_get_variable_number(handle, "dim", &value);
    }

    *percentage = (uint8_t)value;

    return result;
}

nex_err_t nextion_system_set_brightness(nextion_t *handle, uint8_t percentage, bool persist)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    if (percentage > 100)
    {
        percentage = 100;
    }

    if (persist)
    {
        return nextion_system_set_variable_number(handle, "dims", percentage);
    }
    else
    {
        return nextion_system_set_variable_number(handle, "dim", percentage);
    }
}

nex_err_t nextion_system_get_sleep_on_no_touch(nextion_t *handle, uint16_t *seconds)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK((seconds != NULL), "seconds error(NULL)", NEX_FAIL)

    int32_t value = 0;
    nex_err_t code = nextion_system_get_variable_number(handle, "thsp", &value);

    *seconds = (uint16_t)value;

    return code;
}

nex_err_t nextion_system_set_sleep_on_no_touch(nextion_t *handle, uint16_t seconds)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    return nextion_system_set_variable_number(handle, "thsp", seconds);
}

nex_err_t nextion_system_get_sleep_on_no_serial(nextion_t *handle, uint16_t *seconds)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK((seconds != NULL), "seconds error(NULL)", NEX_FAIL)

    int32_t value = 0;
    nex_err_t code = nextion_system_get_variable_number(handle, "ussp", &value);

    *seconds = (uint16_t)value;

    return code;
}

nex_err_t nextion_system_set_sleep_on_no_serial(nextion_t *handle, uint16_t seconds)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    return nextion_system_set_variable_number(handle, "ussp", seconds);
}

nex_err_t nextion_system_set_wake_on_touch(nextion_t *handle, bool wake_on_touch)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    return nextion_system_set_variable_number(handle, "thup", wake_on_touch);
}

nex_err_t nextion_system_set_wake_on_serial(nextion_t *handle, bool wake_on_serial)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    return nextion_system_set_variable_number(handle, "usup", wake_on_serial);
}

nex_err_t nextion_system_set_send_xy(nextion_t *handle, bool send_xy)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    return nextion_system_set_variable_number(handle, "sendxy", send_xy);
}

nex_err_t nextion_system_get_variable_text(nextion_t *handle,
                                           const char *variable_name,
                                           char *buffer,
                                           size_t buffer_length)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK((variable_name != NULL), "variable_name error(NULL)", NEX_FAIL)
    CMP_CHECK((buffer != NULL), "text error(NULL)", NEX_FAIL)

    return nextion_protocol_send_instruction_get_text(handle,
                                                      buffer,
                                                      buffer_length,
                                                      "get %s",
                                                      variable_name);
}

nex_err_t nextion_system_get_variable_number(nextion_t *handle,
                                             const char *variable_name,
                                             int32_t *number)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK((variable_name != NULL), "variable_name error(NULL)", NEX_FAIL)
    CMP_CHECK((number != NULL), "number error(NULL)", NEX_FAIL)

    return nextion_protocol_send_instruction_get_number(handle, number, "get %s", variable_name);
}

nex_err_t nextion_system_set_variable_text(nextion_t *handle, const char *variable_name, const char *text)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK((variable_name != NULL), "variable_name error(NULL)", NEX_FAIL)
    CMP_CHECK((text != NULL), "text error(NULL)", NEX_FAIL)

    return nextion_protocol_send_instruction_ack(handle, "%s=\"%s\"", variable_name, text);
}

nex_err_t nextion_system_set_variable_number(nextion_t *handle,
                                             const char *variable_name,
                                             int32_t number)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK((variable_name != NULL), "variable_name error(NULL)", NEX_FAIL)

    return nextion_protocol_send_instruction_ack(handle, "%s=%ld", variable_name, number);
}