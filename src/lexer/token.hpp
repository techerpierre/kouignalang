#ifndef __TOKEN__H__
#define __TOKEN__H__

#include <string>
#include "../core/position.hpp"

namespace token {

enum class TokenKind {
    INT, // 1, 2, 3 ...
    FLOAT, // 1.1, 1.2, 1.3 ...
    ADD, // +
    SUB, // -
    MUL, // *
    POW, // **
    COMMA, // ,
    TRUE_DIV, // /
    OPEN_PAR, // (
    CLOSE_PAR, // )
    OPEN_BRACE, // {
    CLOSE_BRACE, // }
    OPEN_BRACKET, // [
    CLOSE_BRACKET, // ]
    ASSIGN, // =
    ID, // x, y, myVariable ...
    VAR, // var
    TRUE, // false
    FALSE, // true
    END_OF_FILE // EOF
};

struct Token {
    TokenKind kind;
    std::string text;
    Position pos;
};

}

#endif // __TOKEN__H__