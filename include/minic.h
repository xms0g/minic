#ifndef MT_H
#define MT_H

#include <stddef.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define MT_EPSILON_FLOAT 1e-6f
#define MT_EPSILON_DOUBLE 1e-12

#define COLOR_GREEN   "\033[32m"
#define COLOR_RED     "\033[31m"
#define COLOR_RESET   "\033[0m"

extern int failed;

#define EXPECT_FLOAT_EQ(actual, expected) \
    do { \
        if (fabsf((actual) - (expected)) > MT_EPSILON_FLOAT) { \
            ++failed; \
            printf("%s:%d: " COLOR_RED"Failure\n" COLOR_RESET"Got %.9g, expected %.9g\n", \
                __FILE__, __LINE__, actual, expected); \
        } \
    } while (0)

#define EXPECT_DOUBLE_EQ(actual, expected) \
    do { \
        if (fabs((actual) - (expected)) > MT_EPSILON_DOUBLE) { \
            ++failed; \
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
