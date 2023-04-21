#ifndef __COMMON_INFRA_TEST_H__
#define __COMMON_INFRA_TEST_H__

#include "unity.h"
#include "config.h"
#include "esp32_driver_nextion/nextion.h"

#ifdef __cplusplus
extern "C"
{
#endif

    extern nextion_t *handle;

    // Checks

#define CHECK_NEX_OK(code) TEST_ASSERT_EQUAL_INT32(NEX_OK, code)
#define CHECK_NEX_FAIL(code) TEST_ASSERT_EQUAL_INT32(NEX_FAIL, code)
#define CHECK_NULL(pointer) TEST_ASSERT(pointer == NULL)
#define CHECK_NOT_NULL(pointer) TEST_ASSERT(pointer != NULL)
#define CHECK_TRUE(condition) TEST_ASSERT_TRUE(condition)
#define CHECK_FALSE(condition) TEST_ASSERT_FALSE(condition)

    // Equals

#define SIZET_EQUAL(a, b) TEST_ASSERT_EQUAL_UINT32(a, b)
#define RGB565_EQUAL(a, b) TEST_ASSERT_EQUAL_UINT16(a, b)
#define NEX_CODES_EQUAL(a, b) TEST_ASSERT_EQUAL_INT32(a, b)
#define NEX_TOUCH_STATES_EQUAL(a, b) TEST_ASSERT_EQUAL_UINT8(a, b)
#define LONGS_EQUAL(expected, actual) TEST_ASSERT_EQUAL_INT(expected, actual)
#define STRCMP_EQUAL(expected, actual) TEST_ASSERT_EQUAL_STRING(expected, actual)
#define FAIL_TEST(message) TEST_FAIL_MESSAGE(message)

#ifdef __cplusplus
}
#endif
#endif