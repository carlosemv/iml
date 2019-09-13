#include "AST/RotateNode.h"

#include "AST/ASTVisitor.h"
#include <utility>

RotateNode::RotateNode(Token _t,
	const ExprNode& _img, const ExprNode& _rot) :
    ExprNode(std::move(_t), ExprType::Image),
    image(std::make_unique<ExprNode>(_img)),
    rotation(std::make_unique<ExprNode>(_rot))
{}

RotateNode::RotateNode(Token _t,
	expr_ptr _img, expr_ptr _rot) :
    ExprNode(std::move(_t), ExprType::Image),
    image(std::move(_img)), rotation(std::move(_rot))
{}

void RotateNode::visit(ASTVisitor& visitor)
{
    visitor.visit(*this);
}
