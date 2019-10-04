#include "AST/ReturnNode.h"

#include "AST/ASTVisitor.h"
#include <utility>

ReturnNode::ReturnNode(Token _t, FullType _func_type,
	std::unique_ptr<ExprNode> _expr) :
    CommandNode(std::move(_t)),
    function_type(_func_type),
    expr(std::move(_expr))
{}

void ReturnNode::visit(ASTVisitor& visitor)
{
    visitor.visit(*this);
}