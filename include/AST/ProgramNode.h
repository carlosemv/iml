#ifndef __PROGRAM_NODE_H__
#define __PROGRAM_NODE_H__

#include <vector>
#include <memory>

#include "CommandNode.h"

class ASTVisitor;
class ProgramNode : public CommandNode
{
public:
    ProgramNode(Token::t_type _t, std::vector<CommandNode> _cmds);
    ProgramNode(Token::t_type _t, std::vector<std::unique_ptr<CommandNode>> _cmds);

    ProgramNode(Token _t, std::vector<CommandNode> _cmds);
    ProgramNode(Token _t, std::vector<std::unique_ptr<CommandNode>> _cmds);
    
    void visit(ASTVisitor& visitor);

    std::vector<std::unique_ptr<CommandNode>> cmds;
};

#endif
