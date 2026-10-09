#include "lexer.hpp"
#include "lexing_context.hpp"
#include <cctype>
#include "../core/error.hpp"
#include "../utils/utils.hpp"

using namespace lexer;

namespace {
    void scanToken(LexingContext& ctx);
    template<typename T>
    bool scanRules(LexingContext& ctx, const std::vector<std::pair<std::string, T>>& rules);
    void scanNumber(LexingContext& ctx);
    void scanIdentifier(LexingContext& ctx);

    void scanToken(LexingContext& ctx) {
        if (isspace(ctx.Peek())) {
            ctx.SkipEmpty();
            return;
        }

        if (scanRules(ctx, ReservedSymbols)) return;

        if (std::isdigit(ctx.Peek())) {
            scanNumber(ctx);
            return;
        }

        if (isAlpha(ctx.Peek())) {
            scanIdentifier(ctx);
            return;
        }

        throw error::ParsingError(std::string("Unexpected token: ") + ctx.Peek(), ctx.Position());
    }

    template<typename T>
    bool scanRules(LexingContext& ctx, const std::vector<std::pair<std::string, T>>& rules) {
        for (const auto& [token, kind] : rules) {
            if (ctx.Match(token)) {
                ctx.PushToken(kind, token);
                return true;
            }
        }
        return false;
    }

    void scanNumber(LexingContext& ctx) {
        std::string text;
        TokenKind kind = TokenKind::INT;

        while (!ctx.IsEnd() && std::isdigit(ctx.Peek())) {
            text.push_back(ctx.Advance());
        }

        if (ctx.Peek() == '.' && isdigit(ctx.Peek(1))) {
            kind = TokenKind::FLOAT;
            text.push_back(ctx.Advance());

            while (!ctx.IsEnd() && std::isdigit(ctx.Peek())) {
                text.push_back(ctx.Advance());
            }
        }

        if (!ctx.IsEnd() && ctx.Peek() == 'f') {
            kind = TokenKind::FLOAT;
            ctx.Advance();
        }

        ctx.PushToken(kind, text);
    }

    void scanIdentifier(LexingContext& ctx) {
        std::string text = "";
        TokenKind kind = TokenKind::ID;

        while (!ctx.IsEnd() && isAlnum(ctx.Peek())) {
            text += ctx.Peek();
            ctx.Advance();
        }

        auto reserved = ReservedIdentifiers.find(text);

        if (reserved != ReservedIdentifiers.end()) {
            kind = reserved->second;
        }

        ctx.PushToken(kind, text);
    }
}

#include <iostream>

std::unique_ptr<std::vector<Token>> lexer::lex(const std::string& source) {
    LexingContext ctx(source);

    while (!ctx.IsEnd()) {
        ctx.StartTurn();
        scanToken(ctx);
    }

    ctx.PushToken(TokenKind::END_OF_FILE);
    return ctx.MoveTokens();
}

