#ifndef MT_H
#define MT_H

#include <stddef.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define MT_EPSILON 0.000001

#define PRINT_RESULT(expected, result, func_name, state) \
    do { \
        printf("==============================================|\n"); \
        printf("| %-8s | %-8s | %-12s | %s |\n", "Expected", "Result", "Function Name", "State"); \
        printf("| %8.3f | %8.3f | %-13s | %s|\n",expected, result, func_name, state); \
        printf("==============================================|\n"); \
    } while (0)

#define EXPECT_FLOAT_EQUAL(result, expected, func_name) \
    do { \
        if (fabsf((result) - (expected)) >= MT_EPSILON) { \
            PRINT_RESULT(expected, result, func_name, "failed"); \
        } else { \
            PRINT_RESULT(expected, result, func_name, "passed"); \
        } \
    } while (0)

#define EXPECT_DOUBLE_EQUAL(result, expected, func_name) \
    do { \
        if (fabs((result) - (expected)) >= MT_EPSILON) { \
            PRINT_RESULT(expected, result, func_name, "failed"); \
        } else { \
            PRINT_RESULT(expected, result, func_name, "passed"); \
        } \
    } while (0)

#define TEST_FUNC_NAME(name) test_##name
#define TEST_FUNC(name) void TEST_FUNC_NAME(name)()

typedef void (* test_func)();

typedef struct {
    size_t count;
    size_t capacity;
    const char* name;
    test_func* tests;
} MTSuite;

void mtInitRegistry();

void mtRunAllTests();

void mtRunSuiteTests(const char* name);

MTSuite* mtAddSuite(const char* name);

void mtAddTest(MTSuite* suite, test_func funcPtr);

void mtCleanupRegistry();

#endif
