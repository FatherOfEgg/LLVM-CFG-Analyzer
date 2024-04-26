#include <iostream>

#include "cfg.hpp"

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <LLVM IR file>" << std::endl;
        return 1;
    }

    CFG cfg;
    cfg.Open(argv[1]);
    cfg.Analyze();

    return 0;
}
