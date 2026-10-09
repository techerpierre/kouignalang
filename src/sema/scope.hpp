#ifndef __SCOPE__H__
#define __SCOPE__H__

#include <memory>
#include <string>
#include <map>
#include <utility>
#include "../ast/ast.hpp"

namespace sema {

struct Symbol {
    std::string name;
    const ast::Statement* statement;
};

class Scope : public std::enable_shared_from_this<Scope> {
private:
    std::shared_ptr<Scope> m_Parent;
    std::map<std::string, std::unique_ptr<Symbol>> m_Symbols;
public:
    Scope(std::shared_ptr<Scope> parent = nullptr);

    const Symbol& Get(const std::string& name, const Position& position) const;
    bool Exists(const std::string& name) const;
    void Set(const std::string& name, const ast::Statement* statement);
    std::shared_ptr<Scope> Enter();
    std::shared_ptr<Scope> Exit() const;
};

}

#endif // __SCOPE__H__