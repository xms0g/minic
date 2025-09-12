#include "minic.h"
#include <string.h>

bool failed = false;

typedef struct {
    size_t count;
    size_t capacity;
    MTSuite* suites;
    size_t totalTestCount;
    size_t failedCount;
} MTRegistry;

static MTRegistry registry;

void mtInitRegistry() {
    registry.failedCount = 0;
    registry.totalTestCount = 0;
    registry.count = 0;
    registry.capacity = 2;
    registry.suites = malloc(registry.capacity * sizeof(MTSuite));
}

static void printHeader(size_t testCount, size_t suiteCount);
static void printFooter(size_t testCount, size_t suiteCount);
static void printSuiteStart(const char* name, size_t count);
static void printSuiteEnd(const char* name, size_t count, int ms);
static void printTestStart(const char* suiteName, const char* funcName);
static void printTestEnd(const char* suiteName, const char* funcName);

void mtRunAllTests() {
    printHeader(registry.totalTestCount, registry.count);

    for (size_t i = 0; i < registry.count; i++) {
        const MTSuite currentSuite = registry.suites[i];

        printSuiteStart(currentSuite.name, currentSuite.count);
        for (size_t j = 0; j < currentSuite.count; j++) {
            const TestFunc* func = &currentSuite.cases[j];

            printTestStart(currentSuite.name, func->name);
            func->callback();
            printTestEnd(currentSuite.name, func->name);
        }
        printSuiteEnd(currentSuite.name, currentSuite.count, 0);
    }
    printFooter(registry.totalTestCount, registry.count);
}

void mtRunSuiteTests(const char* name) {
    for (size_t i = 0; i < registry.count; i++) {
        const MTSuite currentSuite = registry.suites[i];

        if (strcmp(currentSuite.name, name) != 0)
            continue;

        printHeader(currentSuite.count, 1);
        printSuiteStart(currentSuite.name, currentSuite.count);
        for (size_t j = 0; j < currentSuite.count; j++) {
            const TestFunc* func = &currentSuite.cases[j];

            printTestStart(currentSuite.name, func->name);
            func->callback();
            printTestEnd(currentSuite.name, func->name);
        }
        printSuiteEnd(currentSuite.name, currentSuite.count, 0);
        printFooter(currentSuite.count, 1);
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

    suite->cases[suite->count++] = (TestFunc){name, func};
    registry.totalTestCount++;
}

void mtCleanupRegistry() {
    for (size_t i = 0; i < registry.count; i++) {
        free(registry.suites[i].cases);
    }
    free(registry.suites);
}

static void printHeader(const size_t testCount, const size_t suiteCount) {
    printf(COLOR_GREEN "[==========] " COLOR_RESET "Running %lu tests from %lu test suites.\n",
           testCount, suiteCount);
    printf(COLOR_GREEN "[----------] " COLOR_RESET "Global test environment set-up.\n");
}

static void printFooter(const size_t testCount, const size_t suiteCount) {
    printf(COLOR_GREEN"[----------] " COLOR_RESET"Global test environment tear-down.\n");
    printf(COLOR_GREEN"[==========] " COLOR_RESET"%lu tests from %lu test suites ran. (%d ms total)\n",
           testCount, suiteCount, 0);

    if (registry.failedCount == 0) {
        printf(COLOR_GREEN "[  PASSED  ] " COLOR_RESET "%lu tests.\n", testCount);
    } else {
        printf(COLOR_GREEN "[  PASSED  ] " COLOR_RESET "%lu tests.\n", testCount - registry.failedCount);
        printf(COLOR_RED "[  FAILED  ] " COLOR_RESET "%lu tests.\n", registry.failedCount);
    }
}

static void printSuiteStart(const char* name, const size_t count) {
    printf(COLOR_GREEN"[----------] " COLOR_RESET"%lu tests from %s\n", count, name);
}

static void printSuiteEnd(const char* name, const size_t count, const int ms) {
    printf(COLOR_GREEN"[----------] " COLOR_RESET"%lu tests from %s (%d ms total)\n\n", count, name, ms);
}

static void printTestStart(const char* suiteName, const char* funcName) {
    printf(COLOR_GREEN"[ RUN      ] " COLOR_RESET"%s.%s\n", suiteName, funcName);
}

static void printTestEnd(const char* suiteName, const char* funcName) {
    if (failed) {
        failed = false;
        registry.failedCount++;

        printf(COLOR_RED "[  FAILED  ] " COLOR_RESET "%s.%s (%d ms)\n", suiteName, funcName, 0);
    } else {
        printf(COLOR_GREEN "[       OK ] " COLOR_RESET "%s.%s (%d ms)\n", suiteName, funcName, 0);
    }
}


