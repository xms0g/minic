#include "mt.h"
#include <string.h>

typedef struct {
    struct {
        size_t count;
        size_t capacity;
        MTSuite* data;
    } suites;
} MTRegistry;

static MTRegistry registry;

void mtInitRegistry() {
    registry.suites.count = 0;
    registry.suites.capacity = 2;
    registry.suites.data = malloc(registry.suites.capacity * sizeof(MTSuite));
}

void mtRunAllTests() {
    for (size_t i = 0; i < registry.suites.count; i++) {
        const MTSuite currentSuite = registry.suites.data[i];
        
        for (size_t j = 0; j < currentSuite.tests.count; j++) {
            currentSuite.tests.data[j]();
        }
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
}

void mtCleanupRegistry() {
    free(registry.suites.data->tests.data);
    free(registry.suites.data);
}