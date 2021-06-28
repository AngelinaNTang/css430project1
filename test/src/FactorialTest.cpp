#include <Factorial.hpp>
#include <TestSuite.hpp>
#include <Assert.hpp>
#include <TestSuite.hpp>

void factorial0() {
    ASSERT_EQ(1, factorial(0));
}

void factorial1() {
    ASSERT_EQ(1, factorial(1));
}

void factorial4() {
    ASSERT_EQ(24, factorial(4));
}

int runFactorialTests() {
    TestSuite suite;

    TEST(suite, factorial0);
    TEST(suite, factorial1);
    TEST(suite, factorial4);

    return suite.run();
}