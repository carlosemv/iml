#include "AST/ProgramNode.h"
#include "AST/ASTVisitor.h"

ProgramNode::ProgramNode(Token::t_type _t, 
    std::vector<CommandNode> _cmds) :
    CommandNode(_t)
{
    cmds.reserve(_cmds.size());
    for (auto c : _cmds)
        cmds.emplace_back(std::make_unique<CommandNode>(c));
}

ProgramNode::ProgramNode(Token::t_type _t, 
    std::vector<std::unique_ptr<CommandNode>> _cmds) :
    CommandNode(_t), cmds(std::move(_cmds))
{}

ProgramNode::ProgramNode(Token _t, 
    std::vector<CommandNode> _cmds) :
    CommandNode(_t)
{
    cmds.reserve(_cmds.size());
    for (auto c : _cmds)
        cmds.emplace_back(std::make_unique<CommandNode>(c));
}

ProgramNode::ProgramNode(Token _t, 
    std::vector<std::unique_ptr<CommandNode>> _cmds) :
    CommandNode(_t), cmds(std::move(_cmds))
{}


void ProgramNode::visit(ASTVisitor& visitor)
{
    visitor.visit(*this);
}
