#include "AST/DimensionsNode.h"
#include "AST/ASTVisitor.h"

DimensionsNode::DimensionsNode(Token::t_type _t, const ExprNode& _width, const ExprNode& _height) :
    ExprNode(_t, ExprType::Dimensions),
    width(std::make_unique<ExprNode>(_width)),
    height(std::make_unique<ExprNode>(_height))
{}

DimensionsNode::DimensionsNode(Token::t_type _t, expr_ptr _width, expr_ptr _height) :
    ExprNode(_t, ExprType::Dimensions),
    width(std::move(_width)),
    height(std::move(_height))
{}

void DimensionsNode::visit(ASTVisitor& visitor)
{
    visitor.visit(*this);
}