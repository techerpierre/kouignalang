#include "error.hpp"

using namespace error;

std::string Error::getTypeStr(void) const {
    switch (m_Type) {
        case ErrorType::PARSING: return "Parsing Error";
        case ErrorType::RUNTIME: return "Runtime Error";
        case ErrorType::TRANSPILE: return "Transpile Error";
        case ErrorType::SEMANTIC: return "Semantic Error";
        default: return "Unknown Error";
    }
}

Error::Error(ErrorType type, const std::string& message, const Position& position)
    : m_Type(type),
    m_Message(message),
    m_Position(position) {}

std::string Error::What() const {
    return getTypeStr()
        + ":\n"
        + m_Message
        + "\nAt "
        + std::to_string(m_Position.line)
        + ", "
        + std::to_string(m_Position.start);
}