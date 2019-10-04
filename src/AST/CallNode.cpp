#include "AST/CallNode.h"

#include "AST/ASTVisitor.h"
#include <utility>

CallNode::CallNode(Token _t, const IdNode& _func,
	std::vector<expr_ptr> _args) :
	ExprNode(std::move(_t), ExprType::Id),
	func(std::make_unique<IdNode>(_func)),
	args(std::move(_args))
{}

void CallNode::visit(ASTVisitor& visitor)
{
    visitor.visit(*this);
}
