#include "scope.hpp"
#include "../core/error.hpp"

using namespace sema;

Scope::Scope(std::shared_ptr<Scope> parent)
    : m_Parent(parent) {}

const Symbol& Scope::Get(const std::string& name, const Position& position) const {
    auto symbol = m_Symbols.find(name);
    if (symbol == m_Symbols.end()) {
        if (m_Parent != nullptr) {
            return m_Parent->Get(name, position);
        }
        throw error::SemanticError("Statement with name \"" + name + "\" not found.", position);
    }
    return *symbol->second;
}

bool Scope::Exists(const std::string& name) const {
    bool isExists = m_Symbols.find(name) != m_Symbols.end();
    if (!isExists && m_Parent != nullptr) {
        return m_Parent->Exists(name);
    }
    return isExists;
}

void Scope::Set(const std::string& name, const ast::Statement* statement) {
    if (Exists(name)) {
        throw error::SemanticError("Redeclared statement.", statement->position);
    }

    auto symbol = std::make_unique<Symbol>();
    symbol->name = name;
    symbol->statement = statement;
    m_Symbols.insert({name, std::move(symbol)});
}

std::shared_ptr<Scope> Scope::Enter() {
    return std::make_shared<Scope>(shared_from_this());
}

std::shared_ptr<Scope> Scope::Exit() const {
    return m_Parent;
}