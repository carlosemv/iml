#include "AST/FlipNode.h"
#include "AST/ASTVisitor.h"

FlipNode::FlipNode(Token _t, FlipDirection _dir, ExprNode _img) :
    ExprNode(_t, ExprType::Image), dir(_dir), image(std::make_unique<ExprNode>(_img))
{}

FlipNode::FlipNode(Token _t, FlipDirection _dir, expr_ptr _img) :
    ExprNode(_t, ExprType::Image), dir(_dir), image(std::move(_img))
{}

void FlipNode::visit(ASTVisitor& visitor)
{ 
    visitor.visit(*this);
}