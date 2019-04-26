#include "AST/UnOpNode.h"
#include "AST/ASTVisitor.h"

UnOpNode::UnOpNode(Token op_tok, ExprNode _expr) :
    ExprNode(op_tok, ExprType::Invalid), 
    expr(std::make_unique<ExprNode>(_expr))
{}

UnOpNode::UnOpNode(Token op_tok, std::unique_ptr<ExprNode> _expr) :
    ExprNode(op_tok, ExprType::Invalid),
    expr(std::move(_expr))
{}

void UnOpNode::visit(ASTVisitor& visitor)
{
    visitor.visit(*this);
}