#ifndef __LEXER__H__
#define __LEXER__H__

#include <string>
#include <utility>
#include <memory>
#include <vector>
#include <map>
#include "token.hpp"
#include "../core/position.hpp"

namespace lexer {

using namespace token;

const std::vector<std::pair<std::string, TokenKind>> ReservedSymbols = {
    {"**", TokenKind::POW},
    {"+", TokenKind::ADD},
    {"-", TokenKind::SUB},
    {"*", TokenKind::MUL},
    {"/", TokenKind::TRUE_DIV},
    {"(", TokenKind::OPEN_PAR},
    {")", TokenKind::CLOSE_PAR},
    {"{", TokenKind::OPEN_BRACE},
    {"}", TokenKind::CLOSE_BRACE},
    {"[", TokenKind::OPEN_BRACKET},
    {"]", TokenKind::CLOSE_BRACKET},
    {"=", TokenKind::ASSIGN},
    {",", TokenKind::COMMA},
};

const std::map<std::string, TokenKind> ReservedIdentifiers = {
    {"var", TokenKind::VAR},
    {"true", TokenKind::TRUE},
    {"false", TokenKind::FALSE},
};

std::unique_ptr<std::vector<Token>> lex(const std::string& source);

}

#endif // __LEXER__H__