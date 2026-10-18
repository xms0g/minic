#include "minic.h"
#include <time.h>

bool failed = false;

typedef struct {
    size_t count;
    size_t capacity;
    minic_suite* suites;
    size_t total_test_count;
    size_t failed_count;
} minic_registry;

static minic_registry registry;

static void print_header(size_t test_count, size_t suite_count);
static void print_footer(size_t test_count, size_t suite_count, double ms);
static void print_suite_start(const char* name, size_t count);
static void print_suite_end(const char* name, size_t count, double ms);
static void print_test_start(const char* suite_name, const char* func_name);
static void print_test_end(const char* suite_name, const char* func_name, double ms);

static double now() {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return ts.tv_sec * 1e6 + ts.tv_nsec / 1e3;
}

void minic_init_registry() {
    registry.failed_count = 0;
    registry.total_test_count = 0;
    registry.count = 0;
    registry.capacity = 2;
    registry.suites = malloc(registry.capacity * sizeof(minic_suite));
}

void minic_run_all_tests() {
    double total_time = 0;
    print_header(registry.total_test_count, registry.count);

    for (size_t i = 0; i < registry.count; i++) {
        const minic_suite current_suite = registry.suites[i];
        double suite_time = 0;

        print_suite_start(current_suite.name, current_suite.count);
        for (size_t j = 0; j < current_suite.count; j++) {
            const test_func* func = &current_suite.cases[j];

            print_test_start(current_suite.name, func->name);
            const double fs = now();
            func->ptr();
            const double fe = now();
            suite_time += fe - fs;
            print_test_end(current_suite.name, func->name, fe - fs);
        }
        total_time += suite_time;
        print_suite_end(current_suite.name, current_suite.count, suite_time);
    }
    print_footer(registry.total_test_count, registry.count, total_time);
}

void minic_run_suite_tests(const char* name) {
    double suite_time = 0;

    for (size_t i = 0; i < registry.count; i++) {
        const minic_suite current_suite = registry.suites[i];

        if (strcmp(current_suite.name, name) != 0)
            continue;

        print_header(current_suite.count, 1);
        print_suite_start(current_suite.name, current_suite.count);
        for (size_t j = 0; j < current_suite.count; j++) {
            const test_func* func = &current_suite.cases[j];

            print_test_start(current_suite.name, func->name);
            const double fs = now();
            func->ptr();
            const double fe = now();
            suite_time += fe - fs;
            print_test_end(current_suite.name, func->name, fe - fs);
        }
        print_suite_end(current_suite.name, current_suite.count, suite_time);
        print_footer(current_suite.count, 1, suite_time);
        break;
    }
}

minic_suite* minic_add_suite(const char* name) {
    if (registry.count == registry.capacity) {
        registry.capacity *= 2;
        registry.suites = realloc(registry.suites, registry.capacity * sizeof(minic_suite));

        if (!registry.suites) {
            fprintf(stderr, "realloc failed");
            return NULL;
        }
    }

    minic_suite suite;
    suite.name = name;
    suite.count = 0;
    suite.capacity = 2;
    suite.cases = malloc(suite.capacity * sizeof(test_func));

    if (!suite.cases) {
        fprintf(stderr, "malloc failed");
        return NULL;
    }

    const size_t idx = registry.count++;
    registry.suites[idx] = suite;

    return &registry.suites[idx];
}

void minic_add_test(minic_suite* suite, const char* name, const test_func_ptr func) {
    if (suite->count == suite->capacity) {
        suite->capacity *= 2;
        suite->cases = realloc(suite->cases, suite->capacity * sizeof(test_func));

        if (!suite->cases) {
            fprintf(stderr, "realloc failed");
            return;
        }
    }

    suite->cases[suite->count++] = (test_func){.name = name, .ptr = func};
    registry.total_test_count++;
}

void minic_cleanup_registry() {
    for (size_t i = 0; i < registry.count; i++) {
        free(registry.suites[i].cases);
    }
    free(registry.suites);
}

static void print_header(const size_t test_count, const size_t suite_count) {
    printf(
        COLOR_GREEN "[==========] " COLOR_RESET "Running %lu tests from %lu test suites.\n"
        COLOR_GREEN "[----------] " COLOR_RESET "Global test environment set-up.\n",
        test_count, suite_count);
}

static void print_footer(const size_t test_count, const size_t suite_count, const double ms) {
    printf(
        COLOR_GREEN"[----------] " COLOR_RESET"Global test environment tear-down.\n"
        COLOR_GREEN"[==========] " COLOR_RESET"%lu tests from %lu test suites ran. (%.2f ms total)\n",
        test_count, suite_count, ms);

#define TEST_RESULT(status) status COLOR_RESET "%lu tests.\n"
    if (registry.failed_count == 0) {
        printf(TEST_RESULT(COLOR_GREEN "[  PASSED  ] "), test_count);
    } else {
        printf(
            TEST_RESULT(COLOR_GREEN "[  PASSED  ] ")
            TEST_RESULT(COLOR_RED "[  FAILED  ] "),
            test_count - registry.failed_count, registry.failed_count);
    }
#undef TEST_RESULT
}

static void print_suite_start(const char* name, const size_t count) {
    printf(COLOR_GREEN"[----------] " COLOR_RESET"%lu tests from %s\n", count, name);
}

static void print_suite_end(const char* name, const size_t count, const double ms) {
    printf(COLOR_GREEN"[----------] " COLOR_RESET"%lu tests from %s (%.2f ms total)\n\n", count, name, ms);
}

static void print_test_start(const char* suite_name, const char* func_name) {
    printf(COLOR_GREEN"[ RUN      ] " COLOR_RESET"%s.%s\n", suite_name, func_name);
}

static void print_test_end(const char* suite_name, const char* func_name, const double ms) {
    if (failed) {
        failed = false;
        registry.failed_count++;

#define TEST_END(status) status COLOR_RESET "%s.%s (%.2f ms)\n"
        printf(TEST_END(COLOR_RED "[  FAILED  ] "), suite_name, func_name, ms);
    } else {
        printf(TEST_END(COLOR_GREEN "[       OK ] "), suite_name, func_name, ms);
    }
#undef TEST_END
}


