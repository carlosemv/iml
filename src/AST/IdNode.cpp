#include "AST/IdNode.h"
#include "AST/ASTVisitor.h"

IdNode::IdNode(Token _t) : ExprNode(_t, ExprType::Id)
{}

void IdNode::visit(ASTVisitor& visitor)
{
    visitor.visit(*this);
}
