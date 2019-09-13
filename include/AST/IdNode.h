#ifndef __ID_NODE_H__
#define __ID_NODE_H__

#include "ExprNode.h"
#include "IdNode.h"

class ASTVisitor;
class IdNode : public ExprNode
{
public:
    explicit IdNode(Token _t);
    void visit(ASTVisitor& visitor);
};

#endif
