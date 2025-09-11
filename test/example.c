#include "minic.h"

float add(const float x, const float y) {
    return x + y;
}

TEST_FUNC(add) {
    EXPECT_FLOAT_EQ(add(3.0f, 5.0f), 8.0f, "add");
}

int main() {
    mtInitRegistry();
    MTSuite* suite = mtAddSuite("add");
    mtAddTest(suite, TEST_FUNC_NAME(add));
    mtRunAllTests();
    mtCleanupRegistry();
    return 0;
}