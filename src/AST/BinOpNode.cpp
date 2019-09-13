#include "AST/BinOpNode.h"

#include "AST/ASTVisitor.h"
#include <utility>

BinOpNode::BinOpNode(const ExprNode& _lhs, Token op_tok, const ExprNode& _rhs) :
    ExprNode(std::move(op_tok), ExprType::Invalid),
    lhs(std::make_unique<ExprNode>(_lhs)),
    rhs(std::make_unique<ExprNode>(_rhs))
{}

BinOpNode::BinOpNode(std::unique_ptr<ExprNode> _lhs, Token op_tok,
    std::unique_ptr<ExprNode> _rhs) :
	ExprNode(std::move(op_tok), ExprType::Invalid),
    lhs(std::move(_lhs)), rhs(std::move(_rhs))
{}

void BinOpNode::visit(ASTVisitor& visitor)
{
    visitor.visit(*this);
}
