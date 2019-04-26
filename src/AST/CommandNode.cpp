#include "AST/CommandNode.h"
#include "AST/ASTVisitor.h"

void CommandNode::visit(ASTVisitor& visitor)
{
    visitor.visit(*this);
}