#include "AST/AssignNode.h"

#include "AST/ASTVisitor.h"
#include <utility>

AssignNode::AssignNode(Token _t, const IdNode& _id, const ExprNode& _expr) :
    CommandNode(std::move(_t)), id(std::make_unique<IdNode>(_id)),
    expr(std::make_unique<ExprNode>(_expr))
{}

AssignNode::AssignNode(Token _t, const IdNode& _id,
    std::unique_ptr<ExprNode> _expr) : CommandNode(std::move(_t)),
    id(std::make_unique<IdNode>(_id)), expr(std::move(_expr))
{}

void AssignNode::visit(ASTVisitor& visitor)
{
    visitor.visit(*this);
}
