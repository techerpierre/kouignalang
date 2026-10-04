#include "lexing_context.hpp"

using namespace lexer;

LexingContext::LexingContext(const std::string& source)
    : m_Line(1),
    m_Cursor(0),
    m_Start(0),
    m_LastLine(0),
    m_Source(source),
    m_Tokens(std::make_unique<std::vector<Token>>()) {}

bool LexingContext::IsEnd(void) const {
    return m_Cursor >= m_Source.size() || m_Source[m_Cursor] == '\0';
}

char LexingContext::Peek(size_t offset) const {
    if (IsEnd()) return '\0';
    return m_Source[m_Cursor + offset];
}

bool LexingContext::Match(std::string expected) {
    if (IsEnd()) return false;

    for (size_t i = 0; i < expected.size(); i++) {
        if (m_Source[m_Cursor + i] != expected[i]) {
            return false;
        }
    }

    m_Cursor += expected.size();
    return true;
}

char LexingContext::Advance(void) {
    return m_Source[m_Cursor++];
}

void LexingContext::SkipEmpty(void) {
    if (Peek() == '\n') {
        m_LastLine = m_Start;
        m_Line++;
    }
    m_Cursor++;
}

void LexingContext::StartTurn(void) {
    m_Start = m_Cursor;
}

Position LexingContext::Position(void) const {
    return { m_Start, m_Cursor, m_Line };
}

void LexingContext::PushToken(TokenKind kind, const std::string& text) {
    m_Tokens->push_back({ kind, text, Position() });
}

std::unique_ptr<std::vector<Token>> LexingContext::MoveTokens(void) {
    return std::move(m_Tokens);
}