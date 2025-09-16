#include "minic.h"
#include <time.h>

bool failed = false;

typedef struct {
    size_t count;
    size_t capacity;
    MTSuite* suites;
    size_t totalTestCount;
    size_t failedCount;
} MTRegistry;

static MTRegistry registry;

static void printHeader(size_t testCount, size_t suiteCount);
static void printFooter(size_t testCount, size_t suiteCount, double ms);
static void printSuiteStart(const char* name, size_t count);
static void printSuiteEnd(const char* name, size_t count, double ms);
static void printTestStart(const char* suiteName, const char* funcName);
static void printTestEnd(const char* suiteName, const char* funcName, double ms);

static double now() {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return ts.tv_sec * 1e6 + ts.tv_nsec / 1e3;
}

void mtInitRegistry() {
    registry.failedCount = 0;
    registry.totalTestCount = 0;
    registry.count = 0;
    registry.capacity = 2;
    registry.suites = malloc(registry.capacity * sizeof(MTSuite));
}

void mtRunAllTests() {
    double totalTime = 0;
    printHeader(registry.totalTestCount, registry.count);

    for (size_t i = 0; i < registry.count; i++) {
        const MTSuite currentSuite = registry.suites[i];
        double suiteTime = 0;

        printSuiteStart(currentSuite.name, currentSuite.count);
        for (size_t j = 0; j < currentSuite.count; j++) {
            const TestFunc* func = &currentSuite.cases[j];

            printTestStart(currentSuite.name, func->name);
            const double fs = now();
            func->ptr();
            const double fe = now();
            suiteTime += fe - fs;
            printTestEnd(currentSuite.name, func->name, fe - fs);
        }
        totalTime += suiteTime;
        printSuiteEnd(currentSuite.name, currentSuite.count, suiteTime);
    }
    printFooter(registry.totalTestCount, registry.count, totalTime);
}

void mtRunSuiteTests(const char* name) {
    double suiteTime = 0;

    for (size_t i = 0; i < registry.count; i++) {
        const MTSuite currentSuite = registry.suites[i];

        if (strcmp(currentSuite.name, name) != 0)
            continue;

        printHeader(currentSuite.count, 1);
        printSuiteStart(currentSuite.name, currentSuite.count);
        for (size_t j = 0; j < currentSuite.count; j++) {
            const TestFunc* func = &currentSuite.cases[j];

            printTestStart(currentSuite.name, func->name);
            const double fs = now();
            func->ptr();
            const double fe = now();
            suiteTime += fe - fs;
            printTestEnd(currentSuite.name, func->name, fe - fs);
        }
        printSuiteEnd(currentSuite.name, currentSuite.count, suiteTime);
        printFooter(currentSuite.count, 1, suiteTime);
        break;
    }
}

MTSuite* mtAddSuite(const char* name) {
    if (registry.count == registry.capacity) {
        registry.capacity *= 2;
        registry.suites = realloc(registry.suites, registry.capacity * sizeof(MTSuite));

        if (!registry.suites) {
            fprintf(stderr, "realloc failed");
            return NULL;
        }
    }

    MTSuite suite;
    suite.name = name;
    suite.count = 0;
    suite.capacity = 2;
    suite.cases = malloc(suite.capacity * sizeof(TestFunc));

    if (!suite.cases) {
        fprintf(stderr, "malloc failed");
        return NULL;
    }

    const size_t idx = registry.count++;
    registry.suites[idx] = suite;

    return &registry.suites[idx];
}

void mtAddTest(MTSuite* suite, const char* name, const TestFuncPtr func) {
    if (suite->count == suite->capacity) {
        suite->capacity *= 2;
        suite->cases = realloc(suite->cases, suite->capacity * sizeof(TestFunc));

        if (!suite->cases) {
            fprintf(stderr, "realloc failed");
            return;
        }
    }

    suite->cases[suite->count++] = (TestFunc){.name = name, .ptr = func};
    registry.totalTestCount++;
}

void mtCleanupRegistry() {
    for (size_t i = 0; i < registry.count; i++) {
        free(registry.suites[i].cases);
    }
    free(registry.suites);
}

static void printHeader(const size_t testCount, const size_t suiteCount) {
    printf(
        COLOR_GREEN "[==========] " COLOR_RESET "Running %lu tests from %lu test suites.\n"
        COLOR_GREEN "[----------] " COLOR_RESET "Global test environment set-up.\n",
        testCount, suiteCount);
}

static void printFooter(const size_t testCount, const size_t suiteCount, const double ms) {
    printf(
        COLOR_GREEN"[----------] " COLOR_RESET"Global test environment tear-down.\n"
        COLOR_GREEN"[==========] " COLOR_RESET"%lu tests from %lu test suites ran. (%.2f ms total)\n",
        testCount, suiteCount, ms);

#define TEST_RESULT(status) status COLOR_RESET "%lu tests.\n"
    if (registry.failedCount == 0) {
        printf(TEST_RESULT(COLOR_GREEN "[  PASSED  ] "), testCount);
    } else {
        printf(
            TEST_RESULT(COLOR_GREEN "[  PASSED  ] ")
            TEST_RESULT(COLOR_RED "[  FAILED  ] "),
            testCount - registry.failedCount, registry.failedCount);
    }
#undef TEST_RESULT
}

static void printSuiteStart(const char* name, const size_t count) {
    printf(COLOR_GREEN"[----------] " COLOR_RESET"%lu tests from %s\n", count, name);
}

static void printSuiteEnd(const char* name, const size_t count, const double ms) {
    printf(COLOR_GREEN"[----------] " COLOR_RESET"%lu tests from %s (%.2f ms total)\n\n", count, name, ms);
}

static void printTestStart(const char* suiteName, const char* funcName) {
    printf(COLOR_GREEN"[ RUN      ] " COLOR_RESET"%s.%s\n", suiteName, funcName);
}

static void printTestEnd(const char* suiteName, const char* funcName, const double ms) {
    if (failed) {
        failed = false;
        registry.failedCount++;

#define TEST_END(status) status COLOR_RESET "%s.%s (%.2f ms)\n"
        printf(TEST_END(COLOR_RED "[  FAILED  ] "), suiteName, funcName, ms);
    } else {
        printf(TEST_END(COLOR_GREEN "[       OK ] "), suiteName, funcName, ms);
    }
#undef TEST_END
}


