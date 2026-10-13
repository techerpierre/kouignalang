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

void ensureIsLegalBinaryOperator(const Type* t, const BinaryExpOperator op, const Position& pos) {
    if (t->kind == NodeKind::SIMPLE_TYPE) {
        auto tp = static_cast<const SimpleType*>(t);
        if (tp->name == "bool" && IllegalBinaryBooleanOperators.find(op) != IllegalBinaryBooleanOperators.end()) {
            throw error::SemanticError("Unexpected operator for boolean value.", pos);
        }
    }

    if (t->kind == NodeKind::ARRAY_TYPE) {
        throw error::SemanticError("Cannot operate in array.", pos);
    }
}

void ensureIsLegalUnaryOperator(const Type* t, const UnaryExpOperator op, const Position& pos) {
    if (t->kind == NodeKind::SIMPLE_TYPE) {
        auto tp = static_cast<const SimpleType*>(t);
        if (tp->name == "bool" && IllegalUnaryBooleanOperators.find(op) != IllegalUnaryBooleanOperators.end()) {
            throw error::SemanticError("A boolean value cannot be signed.", pos);
        }
    }
}

std::unique_ptr<Type> cloneType(const Type* t) {
    if (t == nullptr) {
        return nullptr;
    }

    if (t->kind == NodeKind::SIMPLE_TYPE) {
        auto tp = static_cast<const SimpleType*>(t);
        auto copy = std::make_unique<SimpleType>();
        copy->name = tp->name;
        copy->position = tp->position;
        return copy;
    }

    if (t->kind == NodeKind::ARRAY_TYPE) {
        auto tp = static_cast<const ArrayType*>(t);
        auto copy = std::make_unique<ArrayType>();
        copy->elementType = cloneType(tp->elementType.get());
        copy->position = tp->position;
        return copy;
    }

    return nullptr;
}

}

SemanticAnalyser::SemanticAnalyser(void) {
    m_CurrentScope = std::make_shared<Scope>();
}

void SemanticAnalyser::VisitIntLiteral(const IntLiteral& node) {
    auto intType = std::make_unique<SimpleType>();
    intType->name = "int";
    m_CurrentExpressionType = std::move(intType);
}

void SemanticAnalyser::VisitFloatLiteral(const FloatLiteral& node) {
    auto floatType = std::make_unique<SimpleType>();
    floatType->name = "float";
    m_CurrentExpressionType = std::move(floatType);
}

void SemanticAnalyser::VisitBooleanLiteral(const BooleanLiteral& node) {
    auto boolType = std::make_unique<SimpleType>();
    boolType->name = "bool";
    m_CurrentExpressionType = std::move(boolType);
}

void SemanticAnalyser::VisitIdentifierLiteral(const IdentifierLiteral& node) {
    auto symbol = m_CurrentScope->Get(node.name, node.position);

    if (symbol.statement->kind == NodeKind::VARIABLE_DECLARATION_STATEMENT) {
        auto var = static_cast<const VariableDeclarationStatement*>(symbol.statement);
        m_CurrentExpressionType = cloneType(var->type.get());
    } else {
        m_CurrentExpressionType = nullptr;
    }
}

void SemanticAnalyser::VisitArrayLiteral(const ArrayLiteral &node) {
    if (node.elements.empty()) {
        m_CurrentExpressionType = nullptr;
        return;
    }

    Dispatch(*node.elements[0]);
    auto firstElementType = std::move(m_CurrentExpressionType);

    for (size_t i = 1; i < node.elements.size(); ++i) {
        Dispatch(*node.elements[i]);
        if (!isTypesEquals(firstElementType.get(), m_CurrentExpressionType.get())) {
            throw error::SemanticError("Incompatible types between array elements.", node.elements[i]->position);
        }
    }

    auto arrayType = std::make_unique<ArrayType>();
    arrayType->elementType = std::move(firstElementType);
    m_CurrentExpressionType = std::move(arrayType);
}

void SemanticAnalyser::VisitBinaryExpression(const BinaryExpresion& node) {
    Dispatch(*node.left);
    auto leftType = std::move(m_CurrentExpressionType);

    Dispatch(*node.right);
    auto rightType = std::move(m_CurrentExpressionType);

    ensureIsLegalBinaryOperator(leftType.get(), node.op, node.left->position);
    ensureIsLegalBinaryOperator(rightType.get(), node.op, node.right->position);

    if (!isTypesEquals(leftType.get(), rightType.get())) {
        throw error::SemanticError("Cannot operate in incompatible types.", node.position);
    }

    m_CurrentExpressionType = std::move(leftType);
}

void SemanticAnalyser::VisitUnaryExpression(const UnaryExpression& node) {
    Dispatch(*node.right);
    ensureIsLegalUnaryOperator(m_CurrentExpressionType.get(), node.op, node.right->position);
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

        if (!isTypesEquals(node.type.get(), m_CurrentExpressionType.get())) {
            throw error::SemanticError("Invalid expression type in variable declaration.", node.position);
        }
    }

    m_CurrentScope->Set(node.name, &node);
}
