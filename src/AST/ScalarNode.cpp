#include "AST/ScalarNode.h"
#include "AST/ASTVisitor.h"

void ScalarNode::visit(ASTVisitor& visitor)
{
    visitor.visit(*this);
}
