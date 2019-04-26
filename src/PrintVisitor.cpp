#include "PrintVisitor.h"
#include "ProgramLexer.h"

void PrintVisitor::visit(AssignNode& node)
{
    node.id.get()->visit(*this);
    std::cout << " = ";
    node.expr.get()->visit(*this);
}

void PrintVisitor::visit(PrintNode& node)
{
    std::cout << "print ";
    node.expr.get()->visit(*this);
}

void PrintVisitor::visit(BinOpNode& node)
{
    node.lhs.get()->visit(*this);
    if (node.token) {
        std::cout << " ";
        std::cout << node.token.value().text;
        std::cout << " ";
    } else {
        throw std::logic_error("Binary operation"\
            " has no defining token\n");
    }
    node.rhs.get()->visit(*this);
}

void PrintVisitor::visit(IdNode& node)
{
    if (node.token) {
        std::cout << node.token.value().text;
    } else {
        throw std::logic_error("Id node has no token\n");
    }
}

void PrintVisitor::visit(ProgramNode& node)
{
    for (auto& cmd : node.cmds) {
        cmd.get()->visit(*this);
        std::cout << std::endl;
    }
}

void PrintVisitor::visit(ListNode& node)
{
    std::cout << "[";
    auto expr = node.exprs.begin();
    for (; expr != node.exprs.end(); ++expr) {
        (*expr)->visit(*this);
        if (expr+1 != node.exprs.end())
            std::cout << ", ";
    }
    std::cout << "]";
}

void PrintVisitor::visit(ScalarNode& node)
{
    if (node.token) {
        std::cout << node.token.value().text;
    } else {
        throw std::logic_error("Scalar node has no token\n");
    }  
}
