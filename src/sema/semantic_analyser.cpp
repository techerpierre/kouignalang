#include "semantic_analyser.hpp"
#include "../core/error.hpp"

using namespace sema;

namespace {

bool isTypesEquals(const Type* t1, const Type* t2) {
    if (t1 == t2) return true;
    if (t1 == nullptr || t2 == nullptr) return false;

    if (t1->kind == NodeKind::SIMPLE_TYPE && t2->kind == NodeKind::SIMPLE_TYPE) {
        auto t1p = static_cast<const SimpleType*>(t1);
        auto t2p = static_cast<const SimpleType*>(t2);
        return t1p->name == t2p->name;
    }

    if (t1->kind == NodeKind::ARRAY_TYPE && t2->kind == NodeKind::ARRAY_TYPE) {
        auto t1p = static_cast<const ArrayType*>(t1);
        auto t2p = static_cast<const ArrayType*>(t2);
        return isTypesEquals(t1p->elementType.get(), t2p->elementType.get());
    }

    return false;
}

}

SemanticAnalyser::SemanticAnalyser(void) {
    m_CurrentScope = std::make_shared<Scope>();

    m_IntType = std::make_unique<SimpleType>();
    m_IntType->name = "int";

    m_FloatType = std::make_unique<SimpleType>();
    m_FloatType->name = "float";
}

void SemanticAnalyser::VisitIntLiteral(const IntLiteral& node) {
    m_CurrentExpressionType = m_IntType.get();
}

void SemanticAnalyser::VisitFloatLiteral(const FloatLiteral& node) {
    m_CurrentExpressionType = m_FloatType.get();
}

void SemanticAnalyser::VisitIdentifierLiteral(const IdentifierLiteral& node) {
    auto symbol = m_CurrentScope->Get(node.name, node.position);

    if (symbol.statement->kind == NodeKind::VARIABLE_DECLARATION_STATEMENT) {
        auto var = static_cast<const VariableDeclarationStatement*>(symbol.statement);
        m_CurrentExpressionType = var->type.get();
    } else {
        m_CurrentExpressionType = nullptr;
    }
}

void SemanticAnalyser::VisitBinaryExpresion(const BinaryExpresion& node) {
    Dispatch(*node.left);
    auto leftType = m_CurrentExpressionType;

    Dispatch(*node.right);
    auto rightType = m_CurrentExpressionType;

    if (!isTypesEquals(leftType, rightType)) {
        throw error::SemanticError("Cannot operate in incompatible types.", node.position);
    }

    m_CurrentExpressionType = leftType;
}

void SemanticAnalyser::VisitBlockStatement(const BlockStatement& node) {
    m_CurrentScope = m_CurrentScope->Enter();
    for (const auto& stmt : node.body) {
        Dispatch(*stmt);
    }
    m_CurrentScope = m_CurrentScope->Exit();
}

void SemanticAnalyser::VisitVariableDeclarationStatement(const VariableDeclarationStatement& node) {
    if (node.defaultValue != nullptr) {
        Dispatch(*node.defaultValue);

        if (!isTypesEquals(node.type.get(), m_CurrentExpressionType)) {
            throw error::SemanticError("Invalid expression type in variable declaration.", node.position);
        }
    }

    m_CurrentScope->Set(node.name, &node);
}
