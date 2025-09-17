#ifndef MINIC_H
#define MINIC_H

#include <stddef.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MT_EPSILON_FLOAT 1e-4f
#define MT_EPSILON_DOUBLE 1e-12

#define COLOR_GREEN   "\033[32m"
#define COLOR_RED     "\033[31m"
#define COLOR_RESET   "\033[0m"

#define type_fmt(x) _Generic((x),         \
    unsigned char: "%c\n",                \
    char: "%c\n",                         \
    signed char: "%c\n",                  \
    short int: "%hd\n",                   \
    unsigned short int: "%hu\n",          \
    int: "%d\n",                          \
    unsigned int: "%u\n",                 \
    long int: "%ld\n",                    \
    unsigned long int: "%lu\n",           \
    long long int: "%lld\n",              \
    unsigned long long int: "%llu\n",     \
    float: "%f\n",                        \
    double: "%f\n",                       \
    long double: "%Lf\n",                 \
    char *: "%s\n",                       \
    void *: "%p\n",                       \
    default: "%p\n")  /* fallback for anything else */

#define PRINT_FAIL(actual, expected, op_str) \
    do { \
        printf("%s:%d: " COLOR_RED"Failure\n"COLOR_RESET" Actual: ", __FILE__, __LINE__); \
        printf(type_fmt(actual), actual); \
        printf(" Expected: %s ", op_str); \
        printf(type_fmt(expected), expected); \
    } while (0)

/* Boolean Conditions */

#define EXPECT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            failed = true; \
            PRINT_FAIL("false", "true", ""); \
        } \
    } while (0)

#define ASSERT_TRUE(condition) \
    do { \
        EXPECT_TRUE(condition); \
        if (!(condition)) return; \
    } while (0)

#define EXPECT_FALSE(condition) \
    do { \
        if ((condition)) { \
            failed = true; \
            PRINT_FAIL("true", "false", ""); \
        } \
    } while (0)

#define ASSERT_FALSE(condition) \
    do { \
        EXPECT_FALSE(condition); \
        if ((condition)) return; \
    } while (0)

/* Binary Comparison */

#define EXPECT_EQ(actual, expected) \
    do { \
        if ((actual) != (expected)) { \
            failed = true; \
            PRINT_FAIL(actual, expected, ""); \
        } \
    } while (0)

#define ASSERT_EQ(actual, expected) \
    do { \
        EXPECT_EQ(actual, expected); \
        if ((actual) != (expected)) return; \
    } while (0)

#define EXPECT_NE(actual, expected) \
    do { \
        if ((actual) == (expected)) { \
            failed = true; \
            PRINT_FAIL(actual, expected, "not"); \
        } \
    } while (0)

#define ASSERT_NE(actual, expected) \
    do { \
        EXPECT_NE(actual, expected); \
        if ((actual) == (expected)) return; \
    } while (0)

#define EXPECT_LT(actual, expected) \
    do { \
        if ((actual) >= (expected)) { \
            failed = true; \
            PRINT_FAIL(actual, expected, "less than"); \
        } \
    } while (0)

#define ASSERT_LT(actual, expected) \
    do { \
        EXPECT_LT(actual, expected); \
        if ((actual) >= (expected)) return; \
    } while (0)

#define EXPECT_LE(actual, expected) \
    do { \
        if ((actual) > (expected)) { \
            failed = true; \
            PRINT_FAIL(actual, expected, "less than or equal to"); \
        } \
    } while (0)

#define ASSERT_LE(actual, expected) \
    do { \
        EXPECT_LE(actual, expected); \
        if ((actual) > (expected)) return; \
    } while (0)

#define EXPECT_GT(actual, expected) \
    do { \
        if ((actual) <= (expected)) { \
            failed = true; \
            PRINT_FAIL(actual, expected, "greater than"); \
        } \
    } while (0)

#define ASSERT_GT(actual, expected) \
    do { \
        EXPECT_GT(actual, expected); \
        if ((actual) <= (expected)) return; \
    } while (0)

#define EXPECT_GE(actual, expected) \
    do { \
        if ((actual) < (expected)) { \
            failed = true; \
            PRINT_FAIL(actual, expected, "greater than or equal to"); \
        } \
    } while (0)

#define ASSERT_GE(actual, expected) \
    do { \
        EXPECT_GE(actual, expected); \
        if ((actual) < (expected)) return; \
    } while (0)

/* String Comparison */

#define EXPECT_STREQ(actual, expected) \
    do { \
        if (strcmp((actual), (expected)) != 0) { \
            failed = true; \
            PRINT_FAIL(actual, expected, ""); \
        } \
    } while (0)

#define ASSERT_STREQ(actual, expected) \
    do { \
        EXPECT_STREQ(actual, expected); \
        if (strcmp((actual), (expected)) != 0) return; \
    } while (0)

#define EXPECT_STRNE(actual, expected) \
    do { \
        if (strcmp((actual), (expected)) == 0) { \
            failed = true; \
            PRINT_FAIL(actual, expected, "not"); \
        } \
    } while (0)

#define ASSERT_STRNE(actual, expected) \
    do { \
        EXPECT_STRNE(actual, expected); \
        if (strcmp((actual), (expected)) == 0) return; \
    } while (0)

#define EXPECT_STRCASEEQ(actual, expected) \
    do { \
        if (strcasecmp((actual), (expected)) != 0) { \
            failed = true; \
            PRINT_FAIL(actual, expected, ""); \
        } \
    } while (0)

#define ASSERT_STRCASEEQ(actual, expected) \
    do { \
        EXPECT_STRCASEEQ(actual, expected); \
        if (strcasecmp((actual), (expected)) != 0) return; \
    } while (0)

#define EXPECT_STRCASENE(actual, expected) \
    do { \
        if (strcasecmp((actual), (expected)) == 0) { \
            failed = true; \
            PRINT_FAIL(actual, expected, "not"); \
        } \
    } while (0)

#define ASSERT_STRCASENE(actual, expected) \
    do { \
        EXPECT_STRCASENE(actual, expected); \
        if (strcasecmp((actual), (expected)) == 0) return; \
    } while (0)

/* Floating-Point Comparison  */

#define EXPECT_FLOAT_EQ(actual, expected) \
    do { \
        if (fabsf((actual) - (expected)) > MT_EPSILON_FLOAT) { \
            failed = true; \
            PRINT_FAIL(actual, expected, ""); \
        } \
    } while (0)

#define ASSERT_FLOAT_EQ(actual, expected) \
    do { \
        EXPECT_FLOAT_EQ(actual, expected); \
        if (fabsf((actual) - (expected)) > MT_EPSILON_FLOAT) return; \
    } while (0)

#define EXPECT_DOUBLE_EQ(actual, expected) \
    do { \
        if (fabs((actual) - (expected)) > MT_EPSILON_DOUBLE) { \
            failed = true; \
            PRINT_FAIL(actual, expected, ""); \
        } \
    } while (0)

#define ASSERT_DOUBLE_EQ(actual, expected) \
    do { \
        EXPECT_DOUBLE_EQ(actual, expected); \
        if (fabs((actual) - (expected)) > MT_EPSILON_DOUBLE) return; \
    } while (0)

#define TEST_FUNC_NAME(name) test_##name
#define TEST_FUNC(name) void TEST_FUNC_NAME(name)()

extern bool failed;

typedef void (* TestFuncPtr)();

typedef struct {
    const char* name;
    TestFuncPtr ptr;
} TestFunc;

typedef struct {
    size_t count;
    size_t capacity;
    TestFunc* cases;
    const char* name;
} MTSuite;

void mtInitRegistry();

void mtRunAllTests();

void mtRunSuiteTests(const char* name);

MTSuite* mtAddSuite(const char* name);

void mtAddTest(MTSuite* suite, const char* name, TestFuncPtr func);

void mtCleanupRegistry();

#endif
