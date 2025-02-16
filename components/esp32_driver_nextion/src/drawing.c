#include "esp32_driver_nextion/drawing.h"
#include "protocol/parsers/responses/ack.h"
#include "protocol/protocol.h"
#include "assertion.h"

nex_err_t nextion_draw_fill_screen(nextion_t *handle, rgb565_t color)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    formated_instruction_t instruction = FORMAT_INSTRUNCTION("cls %d", color);
    parser_t parser = PARSER_ACK();

    return nextion_protocol_send_instruction(handle, instruction.text, instruction.length, &parser);
}

nex_err_t nextion_draw_fill_area(nextion_t *handle,
                                 area_t area,
                                 rgb565_t color)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    formated_instruction_t instruction = FORMAT_INSTRUNCTION(
        "fill %d,%d,%d,%d,%d",
        area.upper_left.x,
        area.upper_left.y,
        area.bottom_right.x - area.upper_left.x,
        area.bottom_right.y - area.upper_left.y,
        color);
    parser_t parser = PARSER_ACK();

    return nextion_protocol_send_instruction(handle, instruction.text, instruction.length, &parser);
}

nex_err_t nextion_draw_fill_circle(nextion_t *handle,
                                   point_t center,
                                   uint16_t radius,
                                   rgb565_t color)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    formated_instruction_t instruction = FORMAT_INSTRUNCTION(
        "cirs %d,%d,%d,%d",
        center.x,
        center.y,
        radius,
        color);
    parser_t parser = PARSER_ACK();

    return nextion_protocol_send_instruction(handle, instruction.text, instruction.length, &parser);
}

nex_err_t nextion_draw_line(nextion_t *handle,
                            area_t area,
                            rgb565_t color)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    formated_instruction_t instruction = FORMAT_INSTRUNCTION(
        "line %d,%d,%d,%d,%d",
        area.upper_left.x,
        area.upper_left.y,
        area.bottom_right.x - area.upper_left.x,
        area.bottom_right.y - area.upper_left.y,
        color);
    parser_t parser = PARSER_ACK();

    return nextion_protocol_send_instruction(handle, instruction.text, instruction.length, &parser);
}

nex_err_t nextion_draw_rectangle(nextion_t *handle,
                                 area_t area,
                                 rgb565_t color)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    formated_instruction_t instruction = FORMAT_INSTRUNCTION(
        "draw %d,%d,%d,%d,%d",
        area.upper_left.x,
        area.upper_left.y,
        area.bottom_right.x - area.upper_left.x,
        area.bottom_right.y - area.upper_left.y,
        color);
    parser_t parser = PARSER_ACK();

    return nextion_protocol_send_instruction(handle, instruction.text, instruction.length, &parser);
}

nex_err_t nextion_draw_circle(nextion_t *handle,
                              point_t center,
                              uint16_t radius,
                              rgb565_t color)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    formated_instruction_t instruction = FORMAT_INSTRUNCTION(
        "cir %d,%d,%d,%d",
        center.x,
        center.y,
        radius,
        color);
    parser_t parser = PARSER_ACK();

    return nextion_protocol_send_instruction(handle, instruction.text, instruction.length, &parser);
}

nex_err_t nextion_draw_picture(nextion_t *handle,
                               uint8_t picture_id,
                               point_t origin)

{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    formated_instruction_t instruction = FORMAT_INSTRUNCTION(
        "pic %d,%d,%d",
        origin.x,
        origin.y,
        picture_id);
    parser_t parser = PARSER_ACK();

    return nextion_protocol_send_instruction(handle, instruction.text, instruction.length, &parser);
}

nex_err_t nextion_draw_crop_picture(nextion_t *handle,
                                    uint8_t picture_id,
                                    area_t crop_area,
                                    point_t destination)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)

    formated_instruction_t instruction = FORMAT_INSTRUNCTION(
        "xpic %d,%d,%d,%d,%d,%d,%d",
        crop_area.upper_left.x,
        crop_area.upper_left.y,
        crop_area.bottom_right.x - crop_area.upper_left.x,
        crop_area.bottom_right.y - crop_area.upper_left.y,
        destination.x,
        destination.y,
        picture_id);
    parser_t parser = PARSER_ACK();

    return nextion_protocol_send_instruction(handle, instruction.text, instruction.length, &parser);
}

nex_err_t nextion_draw_text(nextion_t *handle,
                            area_t area,
                            font_t font,
                            background_t background,
                            text_alignment_t alignment,
                            const char *text)
{
    CMP_CHECK_HANDLE(handle, NEX_FAIL)
    CMP_CHECK((text != NULL), "text error(NULL)", NEX_FAIL)

    // It's not a problem having a background value if the fill mode is "none".
    uint16_t background_value = background.picture_id;

    if (background.fill_mode == BACKG_FILL_COLOR)
    {
        background_value = background.color;
    }

    formated_instruction_t instruction = FORMAT_INSTRUNCTION(
        "xstr %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\"%s\"",
        area.upper_left.x,
        area.upper_left.y,
        area.bottom_right.x - area.upper_left.x,
        area.upper_left.y,
        font.id,
        font.color,
        background_value,
        alignment.horizontal,
        alignment.vertical,
        background.fill_mode,
        text);
    parser_t parser = PARSER_ACK();

    return nextion_protocol_send_instruction(handle, instruction.text, instruction.length, &parser);
}