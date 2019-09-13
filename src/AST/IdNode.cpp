#include "AST/IdNode.h"

#include "AST/ASTVisitor.h"
#include <utility>

IdNode::IdNode(Token _t) : ExprNode(std::move(_t), ExprType::Id)
{}

void IdNode::visit(ASTVisitor& visitor)
{
    visitor.visit(*this);
}
