#include "AST/FunctionNode.h"

#include "AST/ASTVisitor.h"
#include <utility>

FunctionNode::FunctionNode(Token _t, const IdNode& _id,
	std::vector<std::unique_ptr<IdNode>> _params,
    std::vector<std::unique_ptr<CommandNode>> _cmds) :
    ProgramNode(std::move(_t), std::move(_cmds)),
    name(std::make_unique<IdNode>(_id)),
    params(std::move(_params))
{}

void FunctionNode::visit(ASTVisitor& visitor)
{
    visitor.visit(*this);
}
