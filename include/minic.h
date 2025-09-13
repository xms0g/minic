#ifndef MINIC_H
#define MINIC_H

#include <stddef.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define MT_EPSILON_FLOAT 1e-4f
#define MT_EPSILON_DOUBLE 1e-12

#define COLOR_GREEN   "\033[32m"
#define COLOR_RED     "\033[31m"
#define COLOR_RESET   "\033[0m"

extern bool failed;

/* Boolean Conditions */

#define EXPECT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            failed = true; \
            printf("%s:%d: " COLOR_RED"Failure\n" COLOR_RESET"Got false, expected true\n", \
                __FILE__, __LINE__); \
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
            printf("%s:%d: " COLOR_RED"Failure\n" COLOR_RESET"Got true, expected false\n", \
                __FILE__, __LINE__); \
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
            printf("%s:%d: " COLOR_RED"Failure\n" COLOR_RESET"Got %d, expected %d\n", \
                __FILE__, __LINE__, actual, expected); \
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
            printf("%s:%d: " COLOR_RED"Failure\n" COLOR_RESET"Got %d, expected not %d\n", \
                __FILE__, __LINE__, actual, expected); \
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
            printf("%s:%d: " COLOR_RED"Failure\n" COLOR_RESET"Got %d, expected less than %d\n", \
                __FILE__, __LINE__, actual, expected); \
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
            printf("%s:%d: " COLOR_RED"Failure\n" COLOR_RESET"Got %d, expected less than or equal to %d\n", \
                __FILE__, __LINE__, actual, expected); \
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
            printf("%s:%d: " COLOR_RED"Failure\n" COLOR_RESET"Got %d, expected greater than %d\n", \
                __FILE__, __LINE__, actual, expected); \
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
            printf("%s:%d: " COLOR_RED"Failure\n" COLOR_RESET"Got %d, expected greater than or equal to %d\n", \
                __FILE__, __LINE__, actual, expected); \
        } \
    } while (0)

#define ASSERT_GE(actual, expected) \
    do { \
        EXPECT_GE(actual, expected); \
        if ((actual) < (expected)) return; \
    } while (0)

#define EXPECT_FLOAT_EQ(actual, expected) \
    do { \
        if (fabsf((actual) - (expected)) > MT_EPSILON_FLOAT) { \
            failed = true; \
            printf("%s:%d: " COLOR_RED"Failure\n" COLOR_RESET"Got %.9g, expected %.9g\n", \
                __FILE__, __LINE__, actual, expected); \
        } \
    } while (0)

#define EXPECT_DOUBLE_EQ(actual, expected) \
    do { \
        if (fabs((actual) - (expected)) > MT_EPSILON_DOUBLE) { \
            failed = true; \
            printf("%s:%d: " COLOR_RED"Failure\n" COLOR_RESET"Got %.17g, expected %.17g\n", \
                __FILE__, __LINE__, actual, expected); \
        } \
    } while (0)

#define TEST_FUNC_NAME(name) test_##name
#define TEST_FUNC(name) void TEST_FUNC_NAME(name)()

typedef void (* TestFuncPtr)();

typedef struct {
    const char* name;
    TestFuncPtr callback;
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
