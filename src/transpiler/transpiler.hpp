#ifndef __TRANSPILER__H__
#define __TRANSPILER__H__

#include <memory>
#include <string>
#include <map>
#include <set>
#include <vector>
#include "../ast/visitor.hpp"

namespace transpiler {

using namespace ast;

const std::map<BinaryExpOperator, std::string> BinaryOperatorsToStringMap = {
    {BinaryExpOperator::ADD, "+"},
    {BinaryExpOperator::SUB, "-"},
    {BinaryExpOperator::MUL, "*"},
    {BinaryExpOperator::TRUE_DIV, "/"}
};

const std::map<UnaryExpOperator, std::string> UnaryOperatorsToStringMap = {
    {UnaryExpOperator::ADD, "+"},
    {UnaryExpOperator::SUB, "-"}
};

class Transpiler : public Visitor {
private:
    std::unique_ptr<std::string> m_Output;
    std::set<std::string> m_Includes;
    size_t m_Depth = 0;

    void tabs(void);
public:
    Transpiler();

    void VisitIntLiteral(const IntLiteral& node) override;
    void VisitFloatLiteral(const FloatLiteral& node) override;
    void VisitBooleanLiteral(const BooleanLiteral& node) override;
    void VisitIdentifierLiteral(const IdentifierLiteral& node) override;

    void VisitSimpleType(const SimpleType& node) override;
    void VisitArrayType(const ArrayType& node) override;

    void VisitBinaryExpresion(const BinaryExpresion& node) override;
    void VisitUnaryExpression(const UnaryExpression& node) override;

    void VisitExpressionStatement(const ExpressionStatement& node) override;
    void VisitProgram(const Program& node) override;
    void VisitBlockStatement(const BlockStatement& node) override;
    void VisitVariableDeclarationStatement(const VariableDeclarationStatement& node) override;

    std::unique_ptr<std::string> MoveOutput(void);
};

struct TranspileOptions {
    bool isPrintAst;
};

std::unique_ptr<std::string> transpile(const std::string& source, TranspileOptions opt = {});

}

#endif // __TRANSPILER__H__