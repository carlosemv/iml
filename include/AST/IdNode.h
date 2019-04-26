#ifndef __ID_NODE_H__
#define __ID_NODE_H__

#include "IdNode.h"
#include "ExprNode.h"

class ASTVisitor;
class IdNode : public ExprNode
{
public:
    IdNode(Token _t);
    void visit(ASTVisitor& visitor);
};

#endif
