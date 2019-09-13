#include "AST/FlipNode.h"

#include "AST/ASTVisitor.h"
#include <utility>

FlipNode::FlipNode(Token _t,
	FlipDirection _dir, const ExprNode& _img) :
    ExprNode(std::move(_t), ExprType::Image),
    dir(_dir), image(std::make_unique<ExprNode>(_img))
{}

FlipNode::FlipNode(Token _t,
	FlipDirection _dir, expr_ptr _img) :
    ExprNode(std::move(_t), ExprType::Image),
    dir(_dir), image(std::move(_img))
{}

void FlipNode::visit(ASTVisitor& visitor)
{
    visitor.visit(*this);
}
