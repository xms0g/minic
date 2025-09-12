#ifndef MT_H
#define MT_H

#include <stddef.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define MT_EPSILON 0.000001

#define COLOR_GREEN   "\033[32m"
#define COLOR_RED     "\033[31m"
#define COLOR_RESET   "\033[0m"

#define EXPECT_FLOAT_EQ(result, expected, func_name) \
    do { \
        printf(COLOR_GREEN"[ RUN      ] " COLOR_RESET"%s\n", func_name); \
        if (fabsf((result) - (expected)) >= MT_EPSILON) { \
            printf(COLOR_RED "[  FAILED  ] " COLOR_RESET "%s (%d ms)\n", func_name, 0); \
            return 1;\
        } else { \
            printf(COLOR_GREEN "[       OK ] " COLOR_RESET "%s (%d ms)\n", func_name, 0); \
            return 0;\
        } \
    } while (0)

#define EXPECT_DOUBLE_EQ(result, expected, func_name) \
    do { \
        printf(COLOR_GREEN"[ RUN      ] " COLOR_RESET"%s\n", func_name); \
        if (fabs((result) - (expected)) >= MT_EPSILON) { \
            printf(COLOR_RED "[  FAILED  ] " COLOR_RESET "%s (%d ms)\n", func_name, 0); \
        } else { \
            printf(COLOR_GREEN "[       OK ] " COLOR_RESET "%s (%d ms)\n", func_name, 0); \
        } \
    } while (0)

#define TEST_FUNC_NAME(name) test_##name
#define TEST_FUNC(name) int TEST_FUNC_NAME(name)()

typedef int (* test_func)();

typedef struct {
    struct {
        size_t count;
        size_t capacity;
        test_func* data;
    } tests;

    const char* name;
} MTSuite;

void mtInitRegistry();

void mtRunAllTests();

void mtRunSuiteTests(const char* name);

MTSuite* mtAddSuite(const char* name);

void mtAddTest(MTSuite* suite, test_func funcPtr);

void mtCleanupRegistry();

#endif
