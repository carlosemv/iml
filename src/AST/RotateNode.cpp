#include "AST/RotateNode.h"
#include "AST/ASTVisitor.h"

RotateNode::RotateNode(Token _t, ExprNode _img, ExprNode _rot) :
    ExprNode(_t, ExprType::Image), image(std::make_unique<ExprNode>(_img)),
    rotation(std::make_unique<ExprNode>(_rot))
{}

RotateNode::RotateNode(Token _t, expr_ptr _img, expr_ptr _rot) :
    ExprNode(_t, ExprType::Image), image(std::move(_img)), rotation(std::move(_rot))
{}

void RotateNode::visit(ASTVisitor& visitor)
{ 
    visitor.visit(*this);
}