#ifndef __ARGS__H__
#define __ARGS__H__

#include <string>
#include <memory>

namespace arg {

struct Args {
    std::unique_ptr<std::string> inputFile;
    std::unique_ptr<std::string> outputFile;
    bool isPrintAst;
    bool isHelp;
};

std::unique_ptr<Args> parseArgs(int argc, char* argv[]);

}

#endif // __ARGS__H__