#include "utils/args.hpp"
#include "commands/commands.hpp"

int main(int argc, char* argv[]) {
    auto args = arg::parseArgs(argc, argv);

    if (args->inputFile != nullptr) {
        cmd::transpile(*args);
        return 0;
    }

    cmd::console(*args);

    return 0;
}
