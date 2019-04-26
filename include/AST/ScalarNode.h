#ifndef __SCALAR_NODE_H__
#define __SCALAR_NODE_H__

#include "ExprNode.h"

class ASTVisitor;
class ScalarNode : public ExprNode
{
public:
    using ExprNode::ExprNode;
    void visit(ASTVisitor& visitor);
};

#endif
