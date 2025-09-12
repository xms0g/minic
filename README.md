# MiniC
`minic` is a minimal unit testing framework for C, inspired by GoogleTest’s output style but built to be tiny, portable, and dependency-free.

## Features
- **Lightweight** – just two files, drop into any C project.
- **Pretty output** – colored console logs (green for success, red for failures).
- **Test suites & test cases** – organize tests by suite.
- **Assertions** – compare floats and doubles with epsilon tolerance.
- **File + line reporting** – failing tests show exact location.
- **No external dependencies** – just standard C and ANSI escape codes.

## Example
```c
#include "minic.h"

float addf(const float x, const float y) {
    return x + y;
}

double addd(const double x, const double y) {
    return x + y;
}

TEST_FUNC(addf) {
    EXPECT_FLOAT_EQ(addf(3.0f, 5.0f), 8.0f);
}

TEST_FUNC(addd) {
    EXPECT_DOUBLE_EQ(addd(3.0, 5.0), 8.0);
}

int main() {
    mtInitRegistry();
    MTSuite* math = mtAddSuite("Math");

    mtAddTest(math, "addf", TEST_FUNC_NAME(addf));
    mtAddTest(math, "addd", TEST_FUNC_NAME(addd));

    mtRunAllTests();
    mtCleanupRegistry();
    return 0;
}
```
## Output
```bash
[==========] Running 2 tests from 1 test suites.
[----------] Global test environment set-up.
[----------] 2 tests from Math
[ RUN      ] Math.addf
[       OK ] Math.addf (0 ms)
[ RUN      ] Math.addd
[       OK ] Math.addd (0 ms)
[----------] 2 tests from Math (0 ms total)

[----------] Global test environment tear-down.
[==========] 2 tests from 1 test suites ran. (0 ms total)
[  PASSED  ] 2 tests.
```
## Assertions
`EXPECT_FLOAT_EQ(actual, expected)`
- Compares two floats with tolerance (1e-6f by default).

`EXPECT_DOUBLE_EQ(actual, expected)`
- Compares two doubles with tolerance (1e-12 by default).

On failure, both actual and expected values are printed, along with file and line:
```bash
[ RUN      ] Math.addf
~/minic/test/example.c:12: Failure
Got 8, expected 10
[  FAILED  ] Math.addf (0 ms)
```
## API
- `mtInitRegistry()` – initialize test registry.
- `mtAddSuite(const char* name)` – create a test suite.
- `mtAddTest(MTSuite* suite, const char* name, TestFuncPtr func)` – add test to suite.
- `mtRunAllTests()` – run all suites/tests.
- `mtRunSuiteTests(const char* name)` – run only one suite.
- `mtCleanupRegistry()` – free memory after tests.

## LICENSE
This project is licensed under the LGPL-2.1 License. See the LICENSE file for details.
