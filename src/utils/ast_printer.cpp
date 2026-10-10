#include "ast_printer.hpp"
#include <iostream>
#include <cstdint>

using namespace ast;

namespace {
    std::string withColor(AstColor color, const std::string& str) {
        return AstAnsiiColorsMap.find(color)->second + str + RESET_COLOR;
    }

    std::string tabs(size_t depth) {
        std::string text = "";
        for (size_t i = depth; i > 0; i--) {
            text += withColor(AstColor::BLACK, i == 1 ? "|---" : "|   ");
        }
        return text;
    }

    AstColor colorizeByDepth(
        size_t depth,
        AstColor color0 = AstColor::YELLOW,
        AstColor color1 = AstColor::PURPLE,
        AstColor color2 = AstColor::BLUE
    ) {
        switch(depth % 3) {
            case 0: return color0;
            case 1: return color1;
            case 2: return color2;
            default: return color0;
        }
    }

    std::string brace(size_t depth, int8_t closure = 0) {
        std::string braceText = closure ? "}" : "{";
        return withColor(colorizeByDepth(depth), braceText + "\n");
    }

    std::string parenthesis(size_t depth, int8_t closure = 0) {
        std::string parenthesisText = closure ? ")" : "(";
        return withColor(
            colorizeByDepth(depth, AstColor::BLUE, AstColor::YELLOW, AstColor::PURPLE),
            parenthesisText
        );
    }

    std::string statement(const std::string& name) {
        return withColor(AstColor::GREEN, name);
    }

    std::string expression(const std::string& name) {
        return withColor(AstColor::CYAN, name);
    }

    std::string key(const std::string& text) {
        return withColor(AstColor::CYAN, text);
    }

    std::string symbol(const std::string& text) {
        return withColor(AstColor::PURPLE, text);
    }
}

void AstPrinter::VisitIntLiteral(const IntLiteral& node) {
    std::cout << tabs(m_Depth) << expression("IntLiteral") << parenthesis(m_Depth) << node.value << parenthesis(m_Depth, 1) << "\n";
}

void AstPrinter::VisitFloatLiteral(const FloatLiteral& node) {
    std::cout << tabs(m_Depth) << expression("FloatLiteral") << parenthesis(m_Depth) << node.value << parenthesis(m_Depth, 1) << "\n";
}

void AstPrinter::VisitBooleanLiteral(const BooleanLiteral& node) {
    std::cout << tabs(m_Depth) << expression("BooleanLiteral") << parenthesis(m_Depth) << (node.value ? "true" : "false") << parenthesis(m_Depth, 1) << "\n";
}


void AstPrinter::VisitIdentifierLiteral(const IdentifierLiteral& node) {
    std::cout << tabs(m_Depth) << expression("IdentifierLiteral") << parenthesis(m_Depth) << node.name << parenthesis(m_Depth, 1) << "\n";
}

void AstPrinter::VisitSimpleType(const SimpleType& node) {
    std::cout << node.name;
}

void AstPrinter::VisitArrayType(const ArrayType& node) {
    Dispatch(*node.elementType);
    std::cout << "[]";
}

void AstPrinter::VisitBinaryExpression(const BinaryExpresion& node) {
    std::cout << tabs(m_Depth) << expression("BinaryExpresion") << brace(m_Depth);
    m_Depth++;
    Dispatch(*node.left);
    std::cout << tabs(m_Depth) << BinaryOperatorSymbolsMap.find(node.op)->second << "\n";
    Dispatch(*node.right);
    m_Depth--;
    std::cout << tabs(m_Depth) << brace(m_Depth, 1);
}

void AstPrinter::VisitUnaryExpression(const UnaryExpression& node) {
    std::cout << tabs(m_Depth) << expression("UnaryExpression") << brace(m_Depth);
    m_Depth++;
    std::cout << tabs(m_Depth) << UnaryOperatorSymbolsMap.find(node.op)->second << "\n";
    Dispatch(*node.right);
    m_Depth--;
    std::cout << tabs(m_Depth) << brace(m_Depth, 1);
}

void AstPrinter::VisitExpressionStatement(const ExpressionStatement& node) {
    std::cout << tabs(m_Depth) << statement("ExpressionStatement") << brace(m_Depth);
    m_Depth++;
    Dispatch(*node.expression);
    m_Depth--;
    std::cout << tabs(m_Depth) << brace(m_Depth, 1);
}

void AstPrinter::VisitProgram(const Program& node) {
    std::cout << tabs(m_Depth) << statement("Program") << brace(m_Depth);
    m_Depth++;
    for (const auto& stmt : node.statements) {
        Dispatch(*stmt);
    }
    m_Depth--;
    std::cout << tabs(m_Depth) << brace(m_Depth, 1);
}

void AstPrinter::VisitBlockStatement(const BlockStatement& node) {
    std::cout << tabs(m_Depth) << statement("BlockStatement") << brace(m_Depth);
    m_Depth++;
    for (const auto& stmt : node.body) {
        Dispatch(*stmt);
    }
    m_Depth--;
    std::cout << tabs(m_Depth) << brace(m_Depth, 1);
}

void AstPrinter::VisitVariableDeclarationStatement(const VariableDeclarationStatement& node) {
    std::cout << tabs(m_Depth)
        << statement("VariableDeclarationStatement")
        << parenthesis(m_Depth)
        << key("name") << symbol("=") << node.name
        << " "
        << key("type") << symbol("=");
    Dispatch(*node.type);
    std::cout << parenthesis(m_Depth, 1);

    if (node.defaultValue != nullptr) {
        std::cout << brace(m_Depth);
        m_Depth++;
        Dispatch(*node.defaultValue);
        m_Depth--;
        std::cout << tabs(m_Depth) << brace(m_Depth, 1);
        return;
    }

    std::cout << "\n";
}

void ast::print(const Node& node) {
    AstPrinter printer;
    printer.Dispatch(node);
}
