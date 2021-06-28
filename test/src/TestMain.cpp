#include <TestSuite.hpp>
#include <FactorialTests.hpp>

int testMain(int argc, const char* argv[]) {
    int fail = 0;
    
    fail += runFactorialTests();

    return fail;
}