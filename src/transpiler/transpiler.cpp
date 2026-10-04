#include "transpiler.hpp"
#include "../lexer/lexer.hpp"
#include "../parser/parser.hpp"
#include "../core/error.hpp"

using namespace transpiler;

Transpiler::Transpiler()
    : m_Output(std::make_unique<std::string>("")) {}

void Transpiler::tabs(void) {
    for (size_t i = 1; i <= m_Deapth; i++) {
        *m_Output += "\t";
    }
}

void Transpiler::VisitIntLiteral(const IntLiteral& node) {
    *m_Output += std::to_string(node.value);
}

void Transpiler::VisitFloatLiteral(const FloatLiteral& node) {
    *m_Output += std::to_string(node.value);
}

void Transpiler::VisitBinaryExpresion(const BinaryExpresion& node) {
    if (node.op == BinaryExpOperator::POW) {
        m_Includes.insert("cmath");
        *m_Output += "pow(";
        Dispatch(*node.left);
        *m_Output += ", ";
        Dispatch(*node.right);
        *m_Output += ")";
        return;
    }

    Dispatch(*node.left);
    *m_Output += " " + BinaryOperatorsToStringMap.find(node.op)->second + " ";
    Dispatch(*node.right);
}

void Transpiler::VisitUnaryExpression(const UnaryExpression& node) {
    *m_Output += UnaryOperatorsToStringMap.find(node.op)->second;
    Dispatch(*node.right);
}

void Transpiler::VisitExpressionStatement(const ExpressionStatement& node) {
    tabs();
    Dispatch(*node.expression);
    *m_Output += ";\n";
}

void Transpiler::VisitProgram(const Program& node) {
    *m_Output += "\nint main(void) {\n";
    m_Deapth++;

    for (const auto& stmt : node.statements) {
        Dispatch(*stmt);
    }

    tabs();
    *m_Output += "return 0;\n}\n";
    m_Deapth--;

    std::string includesText = "";

    for (const auto i : m_Includes) {
        includesText += "#include <" + i + ">\n";
    }
    m_Includes.clear();
    *m_Output = includesText + *m_Output;
}

void Transpiler::VisitBlockStatement(const BlockStatement& node) {
    *m_Output += "{\n";
    m_Deapth++;

    for (const auto& stmt : node.body) {
        Dispatch(*stmt);
    }

    m_Deapth--;
    *m_Output += "}\n";
}

std::unique_ptr<std::string> Transpiler::MoveOutput(void) {
    return std::move(m_Output);
}

std::unique_ptr<std::string> transpiler::transpile(const std::string &source)
{
    try {
         auto tokens = lexer::lex(source);
        auto ast = parser::parse(*tokens);
        Transpiler tran;
        tran.Dispatch(*ast);
        return tran.MoveOutput();
    } catch(const error::Error& err) {
        std::cout << err.What() << std::endl;
        return nullptr;
    }
}
