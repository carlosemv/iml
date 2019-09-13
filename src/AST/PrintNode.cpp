#include "AST/PrintNode.h"

#include "AST/ASTVisitor.h"
#include <utility>

PrintNode::PrintNode(Token _t, const ExprNode& _expr) :
    CommandNode(std::move(_t)), expr(std::make_unique<ExprNode>(_expr))
{}

PrintNode::PrintNode(Token _t, std::unique_ptr<ExprNode> _expr) :
    CommandNode(std::move(_t)), expr(std::move(_expr))
{}


void PrintNode::visit(ASTVisitor& visitor)
{
    visitor.visit(*this);
}