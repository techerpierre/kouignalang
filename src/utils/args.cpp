#include "args.hpp"
#include <string_view>

using namespace arg;

std::unique_ptr<Args> arg::parseArgs(int argc, char* argv[]) {
    size_t cursor = 0;
    auto args = std::make_unique<Args>();

    for (size_t i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "--help") {
            args->isHelp = true;
            continue;
        }

        if (arg == "--print-ast" || arg == "-p") {
            args->isPrintAst = true;
            continue;
        }

        if ((arg.rfind("--", 0) == 0 || arg.rfind("-", 0) == 0) && i + 1 >= argc) {
            continue;
        }

        if (arg == "--input" || arg == "-i") {
            args->inputFile = std::make_unique<std::string>(argv[++i]);
            continue;
        }

        if (arg == "--output" || arg == "-o") {
            args->outputFile = std::make_unique<std::string>(argv[++i]);
            continue;
        }
    }

    return args;
}