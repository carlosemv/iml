#include "AST/SectionNode.h"
#include "AST/ASTVisitor.h"

SectionNode::SectionNode(Token::t_type _t, ExprNode _left, ExprNode _upper,
    ExprNode _right, ExprNode _lower) : ExprNode(_t, ExprType::Section),
    left(std::make_unique<ExprNode>(_left)),
    upper(std::make_unique<ExprNode>(_upper)),
    right(std::make_unique<ExprNode>(_right)),
    lower(std::make_unique<ExprNode>(_lower))
{}

SectionNode::SectionNode(Token::t_type _t, expr_ptr _left, expr_ptr _upper,
    expr_ptr _right, expr_ptr _lower) : ExprNode(_t, ExprType::Section),
    left(std::move(_left)), upper(std::move(_upper)),
    right(std::move(_right)), lower(std::move(_lower))
{}

void SectionNode::visit(ASTVisitor& visitor)
{
    visitor.visit(*this);
}