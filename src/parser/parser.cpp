#include "parser.hpp"
#include <iostream>
#include "../core/error.hpp"

using namespace parser;

namespace {
    std::unique_ptr<Statement> parseStatement(ParsingContext& ctx);
    std::unique_ptr<Statement> parseBlockStatement(ParsingContext& ctx);
    std::unique_ptr<Statement> parseVariableDeclarationStatement(ParsingContext& ctx);
    std::unique_ptr<Expression> parseExpression(ParsingContext& ctx);
    std::unique_ptr<Expression> parseTermExpression(ParsingContext& ctx);
    std::unique_ptr<Expression> parseFactorExpression(ParsingContext& ctx);
    std::unique_ptr<Expression> parsePowerExpression(ParsingContext& ctx);
    std::unique_ptr<Expression> parseUnaryExpression(ParsingContext& ctx);
    std::unique_ptr<Expression> parseLiteralExpression(ParsingContext& ctx);
    std::unique_ptr<Expression> parseArrayLiteral(ParsingContext& ctx);
    std::unique_ptr<Type> parseType(ParsingContext& ctx);

    std::unique_ptr<Statement> parseStatement(ParsingContext& ctx) {
        if (ctx.Eat({TokenKind::OPEN_BRACE})) {
            return parseBlockStatement(ctx);
        }

        if (ctx.Eat({TokenKind::VAR})) {
            return parseVariableDeclarationStatement(ctx);
        }

        auto st = std::make_unique<ExpressionStatement>();
        st->expression = parseExpression(ctx);
        return st;
    }

    std::unique_ptr<Statement> parseBlockStatement(ParsingContext& ctx) {
        auto token = ctx.Peek();
        auto block = std::make_unique<BlockStatement>();
        block->position = token.pos;

        while (!ctx.CheckType({TokenKind::CLOSE_BRACE}) && !ctx.IsEnd()) {
            block->body.push_back(parseStatement(ctx));
        }

        ctx.EatOrFail({TokenKind::CLOSE_BRACE}, "Undelimited code block. Missing '{'.");

        return block;
    }

    std::unique_ptr<Statement> parseVariableDeclarationStatement(ParsingContext& ctx) {
        auto varKeyword = ctx.Previous();
        auto name = ctx.EatOrFail({TokenKind::ID}, "Missing variable name.");
        auto variable = std::make_unique<VariableDeclarationStatement>();
        variable->name = name.text;
        variable->type = parseType(ctx);
        variable->position = varKeyword.pos;

        if (ctx.Eat({TokenKind::ASSIGN})) {
            variable->defaultValue = parseExpression(ctx);
        }

        return variable;
    }

    std::unique_ptr<Expression> parseExpression(ParsingContext& ctx) {
        return parseTermExpression(ctx);
    }

    std::unique_ptr<Expression> parseTermExpression(ParsingContext& ctx) {
        auto expr = parseFactorExpression(ctx);

        while (ctx.Eat({TokenKind::ADD, TokenKind::SUB})) {
            auto op = ctx.Previous();
            auto right = parseFactorExpression(ctx);

            auto binaryExpr = std::make_unique<BinaryExpresion>();
            binaryExpr->left = std::move(expr);
            binaryExpr->right = std::move(right);
            binaryExpr->op = BinaryExprOperatorsMap.find(op.kind)->second;
            binaryExpr->position = binaryExpr->left->position;
            expr = std::move(binaryExpr);
        }

        return expr;
    }

    std::unique_ptr<Expression> parseFactorExpression(ParsingContext& ctx) {
        auto expr = parsePowerExpression(ctx);

        while (ctx.Eat({TokenKind::MUL, TokenKind::TRUE_DIV})) {
            auto op = ctx.Previous();

            auto binaryExpr = std::make_unique<BinaryExpresion>();
            binaryExpr->left = std::move(expr);
            binaryExpr->right = parsePowerExpression(ctx);
            binaryExpr->op = BinaryExprOperatorsMap.find(op.kind)->second;
            binaryExpr->position = binaryExpr->left->position;
            expr = std::move(binaryExpr);
        }

        return expr;
    }

    std::unique_ptr<Expression> parsePowerExpression(ParsingContext& ctx) {
        auto expr = parseUnaryExpression(ctx);

        while (ctx.Eat({TokenKind::POW})) {
            auto op = ctx.Previous();

            auto binaryExpr = std::make_unique<BinaryExpresion>();
            binaryExpr->left = std::move(expr);
            binaryExpr->right = parsePowerExpression(ctx);
            binaryExpr->op = BinaryExprOperatorsMap.find(op.kind)->second;
            binaryExpr->position = binaryExpr->left->position;
            expr = std::move(binaryExpr);
        }

        return expr;
    }

    std::unique_ptr<Expression> parseUnaryExpression(ParsingContext& ctx) {
        if (ctx.Eat({TokenKind::ADD, TokenKind::SUB})) {
            auto op = ctx.Previous();

            auto unaryExpr = std::make_unique<UnaryExpression>();
            unaryExpr->right = parseUnaryExpression(ctx);
            unaryExpr->op = UnaryExprOperatorsMap.find(op.kind)->second;
            unaryExpr->position = op.pos;
            return unaryExpr;
        }

        return parseLiteralExpression(ctx);
    }

    std::unique_ptr<Expression> parseLiteralExpression(ParsingContext& ctx) {
        auto token = ctx.Peek();

        if (ctx.Eat({TokenKind::INT})) {
            auto expr = std::make_unique<IntLiteral>();
            expr->value = std::stoi(token.text);
            expr->position = token.pos;
            return expr;
        }

        if (ctx.Eat({TokenKind::FLOAT})) {
            auto expr = std::make_unique<FloatLiteral>();
            expr->value = std::stod(token.text);
            expr->position = token.pos;
            return expr;
        }

        if (ctx.Eat({TokenKind::TRUE, TokenKind::FALSE})) {
            auto expr = std::make_unique<BooleanLiteral>();
            expr->value = token.kind == TokenKind::TRUE ? true : false;
            expr->position = token.pos;
            return expr;
        }

        if (ctx.Eat({TokenKind::ID})) {
            auto expr = std::make_unique<IdentifierLiteral>();
            expr->name = token.text;
            expr->position = token.pos;
            return expr;
        }

        if (ctx.Eat({TokenKind::OPEN_BRACKET})) {
            return parseArrayLiteral(ctx);
        }

        if (ctx.Eat({TokenKind::OPEN_PAR})) {
            auto expr = parseExpression(ctx);
            auto closePar = ctx.EatOrFail({TokenKind::CLOSE_PAR}, ") is expected to close parenthesis.");
            return expr;
        }

        throw error::ParsingError(std::string("Unexpected token: ") + token.text, token.pos);
    }

    std::unique_ptr<Expression> parseArrayLiteral(ParsingContext& ctx) {
        auto arrayLit = std::make_unique<ArrayLiteral>();
        arrayLit->position = ctx.Previous().pos;

        do {
            if (ctx.Eat({TokenKind::CLOSE_BRACKET})) {
                break;
            }
            arrayLit->elements.push_back(parseExpression(ctx));
        } while (ctx.Eat({TokenKind::COMMA}));

        ctx.EatOrFail({TokenKind::CLOSE_BRACKET}, "] is expected to close array literal.");

        return arrayLit;
    }

    std::unique_ptr<Type> parseType(ParsingContext& ctx) {
        auto typeName = ctx.EatOrFail({TokenKind::ID}, "Missing type name.");
        auto type = std::make_unique<SimpleType>();
        type->name = typeName.text;
        type->position = typeName.pos;

        if (ctx.Eat({TokenKind::OPEN_BRACKET})) {
            ctx.EatOrFail({TokenKind::CLOSE_BRACKET}, "] is expected to close array type declaration.");
            auto arrayType = std::make_unique<ArrayType>();
            arrayType->elementType = std::move(type);
            arrayType->position = arrayType->elementType->position;
            return arrayType;
        }

        return type;
    }
}

std::unique_ptr<Program> parser::parse(const std::vector<Token>& tokens) {
    ParsingContext ctx(tokens);
    auto program = std::make_unique<Program>();

    while (!ctx.IsEnd() && !ctx.CheckType({ TokenKind::END_OF_FILE })) {
        program->statements.push_back(parseStatement(ctx));
    }

    return program;
}