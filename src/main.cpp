#include <iostream>
#include "utils/args.hpp"
#include "commands/commands.hpp"

int main(int argc, char* argv[]) {
    auto args = arg::parseArgs(argc, argv);

    if (args->inputFile != nullptr) {
        cmd::transpile(*args);
        return 0;
    }

    std::cerr << "You should specify the --input parameter." << std::endl;

    return 0;
}
