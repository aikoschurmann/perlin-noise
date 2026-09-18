#include "unity.h"
#include "core.h"

void setUp(void) {}
void tearDown(void) {}

void test_addition(void) {
    TEST_ASSERT_EQUAL(4, add(2, 2));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_addition);
    return UNITY_END();
}
