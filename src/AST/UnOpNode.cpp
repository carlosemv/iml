#include "AST/UnOpNode.h"

#include "AST/ASTVisitor.h"
#include <utility>

UnOpNode::UnOpNode(Token op_tok, const ExprNode& _expr) :
    ExprNode(std::move(op_tok), ExprType::Invalid), 
    expr(std::make_unique<ExprNode>(_expr))
{}

UnOpNode::UnOpNode(Token op_tok, std::unique_ptr<ExprNode> _expr) :
    ExprNode(std::move(op_tok), ExprType::Invalid),
    expr(std::move(_expr))
{}

void UnOpNode::visit(ASTVisitor& visitor)
{
    visitor.visit(*this);
}