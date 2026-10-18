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
    minic_init_registry();
    minic_suite* math = minic_add_suite("Math");

    minic_add_test(math, "addf", TEST_FUNC_NAME(addf));
    minic_add_test(math, "addd", TEST_FUNC_NAME(addd));

    minic_run_all_tests();
    minic_cleanup_registry();
    return 0;
}