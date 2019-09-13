#include "AST/ExprNode.h"

#include "AST/ASTVisitor.h"
#include <utility>

ExprNode::ExprNode(Token _t, ExprType _type, bool _command) :
    CommandNode(std::move(_t)), ftype(_type), command(_command)
{}

void ExprNode::visit(ASTVisitor& visitor)
{
    visitor.visit(*this);
}