#include <iostream>
#include <sstream>
#include <cstdint>

#include <Factorial.hpp>

void usage() {
    std::cerr << "Usage: factorial <N>" << std::endl;
    exit(1);
}

int mainImpl(int argc, const char* argv[]) {
    if (argc != 2) {
        usage();
    }
    
    uint32_t n;
    std::stringstream s;

    s << argv[1];
    s >> n;
        
    std::cout << "Factorial of " << n << " is " << factorial(n) << std::endl;

    return 0;
}