#include "AST/ExprNode.h"
#include "AST/ASTVisitor.h"

ExprNode::ExprNode(Token _t, ExprType _type, bool _command) :
    CommandNode(_t), ftype(_type), command(_command)
{}

void ExprNode::visit(ASTVisitor& visitor)
{
    visitor.visit(*this);
}