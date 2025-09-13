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
### Boolean Conditions
**EXPECT_TRUE**

`EXPECT_TRUE(condition)`
`ASSERT_TRUE(condition)`

- Verifies that condition is true.

**EXPECT_FALSE**

`EXPECT_FALSE(condition)`
`ASSERT_FALSE(condition)`

- Verifies that condition is false.

### Binary Comparison
**EXPECT_EQ**

`EXPECT_EQ(val1, val2)`
`ASSERT_EQ(val1, val2)`

- Verifies that val1 == val2.

**EXPECT_NE**

`EXPECT_NE(val1, val2)`
`ASSERT_NE(val1, val2)`

- Verifies that val1 != val2.

**EXPECT_LT**

`EXPECT_LT(val1, val2)`
`ASSERT_LT(val1, val2)`

- Verifies that val1 < val2.

**EXPECT_LE**

`EXPECT_LE(val1, val2)`
`ASSERT_LE(val1, val2)`

- Verifies that val1 <= val2.

**EXPECT_GT**

`EXPECT_GT(val1, val2)`
`ASSERT_GT(val1, val2)`

- Verifies that val1 > val2.

**EXPECT_GE**

`EXPECT_GE(val1, val2)`
`ASSERT_GE(val1, val2)`

- Verifies that val1 >= val2.

### String Comparison
**EXPECT_STREQ**

`EXPECT_STREQ(str1,str2)`
`ASSERT_STREQ(str1,str2)`

- Verifies that the two C strings str1 and str2 have the same contents.

**EXPECT_STRNE**

`EXPECT_STRNE(str1,str2)`
`ASSERT_STRNE(str1,str2)`

- Verifies that the two C strings str1 and str2 have different contents.

**EXPECT_STRCASEEQ**

`EXPECT_STRCASEEQ(str1,str2)`
`ASSERT_STRCASEEQ(str1,str2)`

- Verifies that the two C strings str1 and str2 have the same contents, ignoring case.

**EXPECT_STRCASENE**

`EXPECT_STRCASENE(str1,str2)`
`ASSERT_STRCASENE(str1,str2)`

### Floating-Point Comparison
`EXPECT_FLOAT_EQ(actual, expected)`
`ASSERT_FLOAT_EQ(actual, expected)`

- Compares two floats with tolerance (1e-4f by default).

`EXPECT_DOUBLE_EQ(actual, expected)`
`ASSERT_DOUBLE_EQ(actual, expected)`

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
