#ifndef __AST_PRINTER__H__
#define __AST_PRINTER__H__

#include "../ast/visitor.hpp"
#include <map>
#include <string>

namespace ast {

const std::map<BinaryExpOperator, std::string> BinaryOperatorSymbolsMap = {
    {BinaryExpOperator::ADD, "+"},
    {BinaryExpOperator::SUB, "-"},
    {BinaryExpOperator::MUL, "*"},
    {BinaryExpOperator::TRUE_DIV, "/"},
    {BinaryExpOperator::POW, "**"},
};

const std::map<UnaryExpOperator, std::string> UnaryOperatorSymbolsMap = {
    {UnaryExpOperator::ADD, "+"},
    {UnaryExpOperator::SUB, "-"},
};

enum class AstColor {
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    PURPLE,
    CYAN,
    WHITE,
};

const std::string RESET_COLOR = "\e[0m";

const std::map<AstColor, std::string> AstAnsiiColorsMap = {
    {AstColor::BLACK, "\e[0;30m"},
    {AstColor::RED, "\e[0;31m"},
    {AstColor::GREEN, "\e[0;32m"},
    {AstColor::YELLOW, "\e[0;33m"},
    {AstColor::BLUE, "\e[0;34m"},
    {AstColor::PURPLE, "\e[0;35m"},
    {AstColor::CYAN, "\e[0;36m"},
    {AstColor::WHITE, "\e[0;37m"}
};

class AstPrinter : public Visitor {
private:
    size_t m_Depth = 0;
public:
    void VisitIntLiteral(const IntLiteral& node) override;
    void VisitFloatLiteral(const FloatLiteral& node) override;
    void VisitIdentifierLiteral(const IdentifierLiteral& node) override;

    void VisitSimpleType(const SimpleType& node) override;
    void VisitArrayType(const ArrayType& node) override;

    void VisitBinaryExpresion(const BinaryExpresion& node) override;
    void VisitUnaryExpression(const UnaryExpression& node) override;

    void VisitExpressionStatement(const ExpressionStatement& node) override;
    void VisitProgram(const Program& node) override;
    void VisitBlockStatement(const BlockStatement& node) override;
    void VisitVariableDeclarationStatement(const VariableDeclarationStatement& node) override;
};

void print(const Node& node);

}

#endif // __AST_PRINTER__H__