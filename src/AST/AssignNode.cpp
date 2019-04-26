#include "AST/AssignNode.h"
#include "AST/ASTVisitor.h"

AssignNode::AssignNode(Token _t, IdNode _id, ExprNode _expr) :
    CommandNode(_t), id(std::make_unique<IdNode>(_id)),
    expr(std::make_unique<ExprNode>(_expr))
{}

AssignNode::AssignNode(Token _t, IdNode _id, 
    std::unique_ptr<ExprNode> _expr) : CommandNode(_t), 
    id(std::make_unique<IdNode>(_id)), expr(std::move(_expr))
{}

void AssignNode::visit(ASTVisitor& visitor)
{ 
    visitor.visit(*this);
}