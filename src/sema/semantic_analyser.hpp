#ifndef __SEMANTIC_ANALYSER__H__
#define __SEMANTIC_ANALYSER__H__

#include "../ast/visitor.hpp"
#include "scope.hpp"
#include <memory>
#include <set>

namespace sema {

using namespace ast;

const std::set<BinaryExpOperator> IllegalBinaryBooleanOperators = {
    BinaryExpOperator::ADD,
    BinaryExpOperator::SUB,
    BinaryExpOperator::MUL,
    BinaryExpOperator::TRUE_DIV,
    BinaryExpOperator::POW
};

const std::set<UnaryExpOperator> IllegalUnaryBooleanOperators = {
    UnaryExpOperator::ADD,
    UnaryExpOperator::SUB
};

class SemanticAnalyser : public Visitor {
private:
    std::shared_ptr<Scope> m_CurrentScope;
    Type* m_CurrentExpressionType = nullptr;

    std::unique_ptr<SimpleType> m_IntType;
    std::unique_ptr<SimpleType> m_FloatType;
    std::unique_ptr<SimpleType> m_BooleanType;
public:
    SemanticAnalyser(void);

    void VisitIntLiteral(const IntLiteral& node) override;
    void VisitFloatLiteral(const FloatLiteral& node) override;
    void VisitBooleanLiteral(const BooleanLiteral& node) override;
    void VisitIdentifierLiteral(const IdentifierLiteral& node) override;
    void VisitBinaryExpresion(const BinaryExpresion& node) override;
    void VisitUnaryExpression(const UnaryExpression& node) override;
    void VisitBlockStatement(const BlockStatement& node) override;
    void VisitVariableDeclarationStatement(const VariableDeclarationStatement& node) override;
};

}

#endif // __SEMANTIC_ANALYSER__H__