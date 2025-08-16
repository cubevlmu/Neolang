#include "neo/Compiler.hpp"

/// Compiler entrance
int main(int argc, char** argv) {
    neo::NCompiler cmp {argc, argv};
    return cmp.runCompiler();
}