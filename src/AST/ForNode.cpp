#include "AST/ForNode.h"

#include "AST/ASTVisitor.h"
#include <utility>

ForNode::ForNode(Token _t, bool _rec, const IdNode& _id,
    std::unique_ptr<ExprNode> _path,
    std::vector<std::unique_ptr<CommandNode>> _cmds) :
    ProgramNode(std::move(_t), std::move(_cmds)),
    recursive(_rec), iterator(std::make_unique<IdNode>(_id)),
    path(std::move(_path))
{}

void ForNode::visit(ASTVisitor& visitor)
{
    visitor.visit(*this);
}
