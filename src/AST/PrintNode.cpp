#include "AST/PrintNode.h"
#include "AST/ASTVisitor.h"

PrintNode::PrintNode(Token _t, ExprNode _expr) :
    CommandNode(_t), expr(std::make_unique<ExprNode>(_expr))
{}

PrintNode::PrintNode(Token _t, std::unique_ptr<ExprNode> _expr) :
    CommandNode(_t), expr(std::move(_expr))
{}


void PrintNode::visit(ASTVisitor& visitor)
{ 
    visitor.visit(*this);
}