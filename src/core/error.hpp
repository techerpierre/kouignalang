#ifndef __ERROR__H__
#define __ERROR__H__

#include <string>
#include "position.hpp"

namespace error {

enum class ErrorType {
    PARSING,
    RUNTIME,
    TRANSPILE,
    SEMANTIC
};

class Error {
private:
    ErrorType m_Type;
    std::string m_Message;
    Position m_Position;

    std::string getTypeStr(void) const;
public:
    Error(ErrorType type, const std::string& message, const Position& position);
    std::string What() const;
};

class ParsingError : public Error {
public:
    ParsingError(const std::string& message, const Position& position)
        : Error(ErrorType::PARSING, message, position) {}
};

class RuntimeError : public Error {
public:
    RuntimeError(const std::string& message, const Position& position)
        : Error(ErrorType::RUNTIME, message, position) {}
};

class TranspileError : public Error {
public:
    TranspileError(const std::string& message, const Position& position)
        : Error(ErrorType::TRANSPILE, message, position) {}
};

class SemanticError : public Error {
public:
    SemanticError(const std::string& message, const Position& position)
        : Error(ErrorType::SEMANTIC, message, position) {}
};


}

#endif // __ERROR__H__