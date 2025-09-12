#include "minic.h"
#include <string.h>

typedef struct {
    struct {
        size_t count;
        size_t capacity;
        MTSuite* data;
    } suites;

    size_t totalTestCount;
    size_t failed;
} MTRegistry;

static MTRegistry registry;

void mtInitRegistry() {
    registry.totalTestCount = 0;
    registry.suites.count = 0;
    registry.suites.capacity = 2;
    registry.suites.data = malloc(registry.suites.capacity * sizeof(MTSuite));
}

void mtRunAllTests() {
    printf(COLOR_GREEN"[==========] " COLOR_RESET"Running %lu tests from %lu test suites.\n",
           registry.totalTestCount, registry.suites.count);
    printf(COLOR_GREEN"[----------] " COLOR_RESET"Global test environment set-up.\n");


    for (size_t i = 0; i < registry.suites.count; i++) {
        const MTSuite currentSuite = registry.suites.data[i];

        printf(COLOR_GREEN"[----------] " COLOR_RESET"%lu tests from %s\n",
            currentSuite.tests.count, currentSuite.name);

        for (size_t j = 0; j < currentSuite.tests.count; j++) {
            if (currentSuite.tests.data[j]()) {
                registry.failed++;
            }
        }

        printf(COLOR_GREEN"[----------] " COLOR_RESET"%lu tests from %s (%d ms total)\n\n",
            currentSuite.tests.count, currentSuite.name, 0);
    }

    printf(COLOR_GREEN"[----------] " COLOR_RESET"Global test environment tear-down.\n");
    printf(COLOR_GREEN"[==========] " COLOR_RESET"%lu tests from %lu test suites ran. (%d ms total)\n",
           registry.totalTestCount, registry.suites.count, 0);

    if (registry.failed == 0) {
        printf(COLOR_GREEN "[  PASSED  ] " COLOR_RESET "%lu tests.\n", registry.totalTestCount);
    } else {
        printf(COLOR_GREEN "[  PASSED  ] " COLOR_RESET "%lu tests.\n", registry.totalTestCount - registry.failed);
        printf(COLOR_RED   "[  FAILED  ] " COLOR_RESET "%lu tests.\n", registry.failed);
    }
}

void mtRunSuiteTests(const char* name) {
    for (size_t i = 0; i < registry.suites.count; i++) {
        const MTSuite currentSuite = registry.suites.data[i];

        if (strcmp(currentSuite.name, name) == 0)
            continue;

        for (size_t j = 0; j < currentSuite.tests.count; j++) {
            currentSuite.tests.data[j]();
        }

        break;
    }
}

MTSuite* mtAddSuite(const char* name) {
    if (registry.suites.count == registry.suites.capacity) {
        registry.suites.capacity *= 2;
        registry.suites.data = realloc(registry.suites.data, registry.suites.capacity * sizeof(MTSuite));

        if (!registry.suites.data) {
            fprintf(stderr, "realloc failed");
            return NULL;
        }
    }

    MTSuite suite;
    suite.name = name;
    suite.tests.count = 0;
    suite.tests.capacity = 2;

    suite.tests.data = malloc(suite.tests.capacity * sizeof(test_func));
    if (!suite.tests.data) {
        fprintf(stderr, "malloc failed");
        return NULL;
    }

    const size_t idx = registry.suites.count++;
    registry.suites.data[idx] = suite;

    return &registry.suites.data[idx];
}

void mtAddTest(MTSuite* suite, const test_func funcPtr) {
    if (suite->tests.count == suite->tests.capacity) {
        suite->tests.capacity *= 2;
        suite->tests.data = realloc(suite->tests.data, suite->tests.capacity * sizeof(test_func));

        if (!suite->tests.data) {
            fprintf(stderr, "realloc failed");
            return;
        }
    }

    suite->tests.data[suite->tests.count++] = funcPtr;
    registry.totalTestCount++;
}

void mtCleanupRegistry() {
    free(registry.suites.data->tests.data);
    free(registry.suites.data);
}
