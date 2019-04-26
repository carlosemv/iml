#include "AST/ExprNode.h"
#include "AST/ASTVisitor.h"

// ExprNode::ExprNode(Token _t) : AST(_t), type(ExprType::Invalid)
// {}

ExprNode::ExprNode(Token _t, ExprType _type, bool _command) :
    CommandNode(_t), ftype(_type), command(_command)
{}

ExprNode& ExprNode::operator=(ExprNode other)
{
    this->token = other.token;
    this->ftype = other.ftype;
    return *this;
}

// ExprNode& ExprNode::operator=(ExprNode&& other)
// {

//     ptr = std::move(_obj.ptr);

//     return *this;
// }

void ExprNode::visit(ASTVisitor& visitor)
{
    visitor.visit(*this);
}