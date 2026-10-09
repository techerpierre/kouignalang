#ifndef __SEMANTIC_ANALYSER__H__
#define __SEMANTIC_ANALYSER__H__

#include "../ast/visitor.hpp"
#include "scope.hpp"
#include <memory>

namespace sema {

using namespace ast;

class SemanticAnalyser : public Visitor {
private:
    std::shared_ptr<Scope> m_CurrentScope;
    Type* m_CurrentExpressionType = nullptr;

    std::unique_ptr<SimpleType> m_IntType;
    std::unique_ptr<SimpleType> m_FloatType;
public:
    SemanticAnalyser(void);

    void VisitIntLiteral(const IntLiteral& node) override;
    void VisitFloatLiteral(const FloatLiteral& node) override;
    void VisitIdentifierLiteral(const IdentifierLiteral& node) override;
    void VisitBinaryExpresion(const BinaryExpresion& node) override;
    void VisitBlockStatement(const BlockStatement& node) override;
    void VisitVariableDeclarationStatement(const VariableDeclarationStatement& node) override;
};

}

#endif // __SEMANTIC_ANALYSER__H__