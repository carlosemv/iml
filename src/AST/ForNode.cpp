#include "AST/ForNode.h"
#include "AST/ASTVisitor.h"

ForNode::ForNode(Token _t, bool _rec, IdNode _id,
    std::unique_ptr<ExprNode> _path,
    std::vector<std::unique_ptr<CommandNode>> _cmds) :
    ProgramNode(_t, std::move(_cmds)),
    recursive(_rec), iterator(std::make_unique<IdNode>(_id)),
    path(std::move(_path))
{}

void ForNode::visit(ASTVisitor& visitor)
{
    visitor.visit(*this);
}
