#ifndef __COMMAND_NODE_H__
#define __COMMAND_NODE_H__

#include "AST.h"

class ASTVisitor;
class CommandNode : public AST
{
public:
    using AST::AST;
    void visit(ASTVisitor& visitor);
    virtual ~CommandNode() = default;
};

#endif
