#include "AST/ResizeNode.h"

#include "AST/ASTVisitor.h"
#include <utility>

ResizeNode::ResizeNode(Token _t, ResizeType _type,
    const ExprNode& _img, const ExprNode& _resize) :
    ExprNode(std::move(_t), ExprType::Image), resize_type(_type),
    image(std::make_unique<ExprNode>(_img)),
    resize(std::make_unique<ExprNode>(_resize))
{}

ResizeNode::ResizeNode(Token _t, ResizeType _type,
    expr_ptr _img, expr_ptr _resize) :
    ExprNode(std::move(_t), ExprType::Image), resize_type(_type),
    image(std::move(_img)),
    resize(std::move(_resize))
{}

void ResizeNode::visit(ASTVisitor& visitor)
{
    visitor.visit(*this);
}
