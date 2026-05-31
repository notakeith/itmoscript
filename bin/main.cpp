#include <iostream>
#include <fstream>
#include "Interpreter.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <script.is>\n";
        return 1;
    }

        const char* path = argv[1];
        std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: cannot open file \"" << path << "\"\n";
        return 1;
    }

    bool ok = interpret(file, std::cout);
    if (!ok) {
        std::cerr << "Runtime error during interpretation.\n";
        return 2;
    }
    return 0;
}