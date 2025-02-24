#include "protocol/protocol.h"
#include "common_infra_test.h"

TEST_CASE("Format fails if buffer insufficient", "[protocol]")
{
    formated_instruction_t instruction;

    bool result = nextion_protocol_format_instruction(handle,
                                                      &instruction,
                                                      "This text has more than %d characters, meaning it is bigger than the configured test buffer size. Filling spaces:                 ",
                                                      128);

    CHECK_FALSE(result);
}

TEST_CASE("Format succeeds", "[protocol]")
{
    formated_instruction_t instruction;

    bool result = nextion_protocol_format_instruction(handle,
                                                      &instruction,
                                                      "Sample text: %d",
                                                      128);

    CHECK_TRUE(result);
    STRCMP_EQUAL("Sample text: 128", instruction.text);
    LONGS_EQUAL(16, instruction.length);
}