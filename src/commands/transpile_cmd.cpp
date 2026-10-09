#include "commands.hpp"
#include <fstream>
#include <iostream>
#include "../transpiler/transpiler.hpp"

using namespace cmd;

int cmd::transpile(const arg::Args& args) {
    std::ifstream in(*args.inputFile, std::ios::in | std::ios::binary);

    if (!in.is_open()) {
        std::cerr << "Cannot find open input file";
        return 1;
    }

    auto code = std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
    in.close();

    transpiler::TranspileOptions tranpilerOpt;
    tranpilerOpt.isPrintAst = args.isPrintAst;

    auto outCode = transpiler::transpile(code, tranpilerOpt);

    if (outCode == nullptr) {
        return 1;
    }

    if (args.outputFile == nullptr) {
        std::cout << *outCode << std::endl;
        return 0;
    }

    std::ofstream out;
    out.open(*args.outputFile);

    if (!out.is_open()) {
        std::cerr << "Cannot open output file";
        return 1;
    }

    out << *outCode;
    out.close();

    return 0;
}