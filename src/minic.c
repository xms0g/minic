#include "minic.h"
#include <string.h>

int failed = 0;

typedef struct {
    size_t count;
    size_t capacity;
    MTSuite* suites;
    size_t totalTestCount;
    size_t failedCount;
} MTRegistry;

static MTRegistry registry;

void mtInitRegistry() {
    registry.totalTestCount = 0;
    registry.count = 0;
    registry.capacity = 2;
    registry.suites = malloc(registry.capacity * sizeof(MTSuite));
}

void mtRunAllTests() {
    printf(COLOR_GREEN"[==========] " COLOR_RESET"Running %lu tests from %lu test suites.\n",
           registry.totalTestCount, registry.count);
    printf(COLOR_GREEN"[----------] " COLOR_RESET"Global test environment set-up.\n");


    for (size_t i = 0; i < registry.count; i++) {
        const MTSuite currentSuite = registry.suites[i];

        printf(COLOR_GREEN"[----------] " COLOR_RESET"%lu tests from %s\n",
               currentSuite.count, currentSuite.name);

        for (size_t j = 0; j < currentSuite.count; j++) {
            const TestFunc func = currentSuite.cases[j];

            printf(COLOR_GREEN"[ RUN      ] " COLOR_RESET"%s.%s\n", currentSuite.name, func.name);

            func.callback();

            if (failed) {
                registry.failedCount++;

                printf(COLOR_RED "[  FAILED  ] " COLOR_RESET "%s.%s (%d ms)\n", currentSuite.name, func.name, 0);
                failed = 0;
            } else {
                printf(COLOR_GREEN "[       OK ] " COLOR_RESET "%s.%s (%d ms)\n", currentSuite.name, func.name, 0);
            }
        }

        printf(COLOR_GREEN"[----------] " COLOR_RESET"%lu tests from %s (%d ms total)\n\n",
               currentSuite.count, currentSuite.name, 0);
    }

    printf(COLOR_GREEN"[----------] " COLOR_RESET"Global test environment tear-down.\n");
    printf(COLOR_GREEN"[==========] " COLOR_RESET"%lu tests from %lu test suites ran. (%d ms total)\n",
           registry.totalTestCount, registry.count, 0);

    if (registry.failedCount == 0) {
        printf(COLOR_GREEN "[  PASSED  ] " COLOR_RESET "%lu tests.\n", registry.totalTestCount);
    } else {
        printf(COLOR_GREEN "[  PASSED  ] " COLOR_RESET "%lu tests.\n", registry.totalTestCount - registry.failedCount);
        printf(COLOR_RED "[  FAILED  ] " COLOR_RESET "%lu tests.\n", registry.failedCount);
    }
}

void mtRunSuiteTests(const char* name) {
    for (size_t i = 0; i < registry.count; i++) {
        const MTSuite currentSuite = registry.suites[i];

        if (strcmp(currentSuite.name, name) != 0)
            continue;
        printf(COLOR_GREEN"[==========] " COLOR_RESET"Running %lu tests from 1 test suites.\n", currentSuite.count);
        printf(COLOR_GREEN"[----------] " COLOR_RESET"Global test environment set-up.\n");
        printf(COLOR_GREEN"[----------] " COLOR_RESET"%lu tests from %s\n",
              currentSuite.count, currentSuite.name);

        for (size_t j = 0; j < currentSuite.count; j++) {
            const TestFunc func = currentSuite.cases[j];

            printf(COLOR_GREEN"[ RUN      ] " COLOR_RESET"%s.%s\n", currentSuite.name, func.name);

            func.callback();

            if (failed) {
                registry.failedCount++;

                printf(COLOR_RED "[  FAILED  ] " COLOR_RESET "%s.%s (%d ms)\n", currentSuite.name, func.name, 0);
                failed = 0;
            } else {
                printf(COLOR_GREEN "[       OK ] " COLOR_RESET "%s.%s (%d ms)\n", currentSuite.name, func.name, 0);
            }
        }
        printf(COLOR_GREEN"[----------] " COLOR_RESET"%lu tests from %s (%d ms total)\n\n",
              currentSuite.count, currentSuite.name, 0);
        printf(COLOR_GREEN"[----------] " COLOR_RESET"Global test environment tear-down.\n");
        printf(COLOR_GREEN"[==========] " COLOR_RESET"%lu tests from 1 test suites ran. (%d ms total)\n",
               currentSuite.count, 0);

        if (registry.failedCount == 0) {
            printf(COLOR_GREEN "[  PASSED  ] " COLOR_RESET "%lu tests.\n", currentSuite.count);
        } else {
            printf(COLOR_GREEN "[  PASSED  ] " COLOR_RESET "%lu tests.\n", currentSuite.count - registry.failedCount);
            printf(COLOR_RED "[  FAILED  ] " COLOR_RESET "%lu tests.\n", registry.failedCount);
        }
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
