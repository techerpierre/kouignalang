#include "commands.hpp"
#include <iostream>
#include "../interpreter/interpreter.hpp"

using namespace cmd;

int cmd::console(const arg::Args& args) {
    std::string code;
    std::cout << "> "; 

    while (std::getline(std::cin, code)) {
        if (code.empty()) {
            std::cout << "> "; 
            continue;
        }
        interpreter::interpret(code);
        std::cout << "> "; 
    }

    return 0;
}
